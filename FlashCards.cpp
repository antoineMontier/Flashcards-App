#include "FlashCards.hpp"

using namespace std;

FlashCards::FlashCards(){
    screen = HOME;
    s = new SDL_Screen(1080, 720, "FlashCards", 60);
    s->setFont(&global, "Roboto_m.ttf", 30);
    s->setFont(&small, "Roboto_m.ttf", 18);
    s->setFont(&medium, "Roboto_m.ttf", 22);

    title = {59, 44, 53, 255};
    background = {86, 157, 229, 255};
    buttonColor = {93, 80, 88, 255};
    buttonFontColor = {43, 80, 170, 255};
    answerColor = {80, 21, 55, 255};
    hintColor = {60, 27, 67, 255};
    nextColor = {58, 90, 64, 255};

    typingAllowed = false;
    typingNow = 0;
    buffers[0] = "";
    buffers[1] = "";
    buffers[2] = "";
    buffers_limits[0] = 0;
    buffers_limits[1] = 0;
    buffers_limits[2] = 0;
    packages = new LinkedList<Package*>();
    package_advancement = 0;
    package_testing = -1;
    hint_shown = false;
    answer_shown = false;
    focus = -1;
}

FlashCards::~FlashCards(){
    TTF_CloseFont(global);
    TTF_CloseFont(small);
    TTF_CloseFont(medium);
    delete(s);
    s = nullptr;
    for(int i=0; i<packages->size() ; i++)
        delete(packages->get(i));
    delete(packages);
    packages = nullptr;
}

void FlashCards::run(){
    readDocument("one.flash");//to be removed
    while(s->isRunning()){//main loop
        switch(screen){
            
            case HOME:
                update_underline();
                homeScreen();
                break;

            case SETTINGS:
                settingsScreen();
                break;

            case CREATION:
                creationScreen();
                break;

            default:
                if(screen >= TEST_OFFSET && screen < TEST_OFFSET + MAX_PACKAGES){
                    testScreen(package_testing);
                }else
                    s->stopRunning();//a bug happened
                break;
        }
        
        while (SDL_PollEvent(&e)){ // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (e.type){

            case SDL_TEXTINPUT:
                if(typingAllowed && (int)(buffers[buffer_id].length()) <= buffers_limits[buffer_id])
                    buffers[buffer_id] += e.text.text;
                break;


            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    s->updateSize();
                break;

            case SDL_QUIT:
                s->stopRunning(); // quit the program if the user closes the window
                break;

            case SDL_MOUSEMOTION:
                m_x = e.button.x;
                m_y = e.button.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                focus = -1;
                int tmp_w, tmp_y;
                //border
                TTF_SizeText(global, "home", &tmp_w, &tmp_y);
                switch(screen){
                    case HOME:
                        if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.01, s->H()*.07, s->H()*.07)){
                            buffer_id = 1;
                            buffers[buffer_id] = "";
                            buffers_limits[buffer_id] = file_max_lenght;
                            screen = SETTINGS;
                            focus = -1;
                        }else if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.47, 120, 3*TTF_FontHeight(medium))){
                            buffer_id = 1;
                            typingAllowed = true;
                            buffers[buffer_id] = "";
                            buffers_limits[buffer_id] = file_max_lenght;
                            screen = CREATION;
                            focus = -1;
                        }else 
                            openUnderlined();
                        break;

                    case SETTINGS:
                        if(s->rollover(e.button.x, e.button.y, 75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1)){//return
                            typingAllowed = false;
                            typingNow = false;
                            buffers[buffer_id = 0] = "";//clean buffer
                            screen = HOME;
                            focus = -1;
                        }else if(s->rollover(e.button.x, e.button.y, s->W()*0.5 - TTF_FontHeight(global)*1 + tmp_w, s->H()*.45, 60, 30)){
                            readDocument(buffers[buffer_id] + ".flash");
                            buffers[buffer_id] = "";
                        }else if(s->rollover(e.button.x, e.button.y, s->W()*0.5 - TTF_FontHeight(global)*3.5, s->H()*0.45, tmp_w + 2 + 2.5/(buffers[1].length() + 0.2), tmp_h))
                            typingAllowed = true;
                        else
                            typingAllowed = false;
                        break;
                    
                    case CREATION:
                        if(s->rollover(e.button.x, e.button.y, 75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1)){//return
                            typingAllowed = false;
                            typingNow = false;
                            buffers[buffer_id = 0] = "";//clean buffer
                            screen = HOME;
                            focus = -1;
                        }
                        break;

                    default:
                        if(screen >= TEST_OFFSET && screen < TEST_OFFSET + MAX_PACKAGES){
                            if(s->rollover(e.button.x, e.button.y, 75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1)){//return button
                                screen = HOME;
                                focus = -1;
                                package_testing = -1;
                                package_advancement = 0;
                            }else if(s->rollover(e.button.x, e.button.y, s->W()*.25 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h)){//hint button
                                hint_shown = true;
                            }else if(s->rollover(e.button.x, e.button.y, s->W()*.5 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h)){//next button
                                package_advancement = (package_advancement + 1) % packages->get(package_testing)->question_count();
                                hint_shown = answer_shown = false;
                            }else if(s->rollover(e.button.x, e.button.y, s->W()*.75 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h)){//answer button
                                answer_shown = true;
                            }
                        }
                        break;
                }
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (e.key.keysym.sym){ // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_TAB:
                    switch(screen){
                        case HOME:
                            focus = (focus + 1) % (2 + packages->size());//settings, creation, 1 per package
                            break;

                        case SETTINGS:
                            focus = (focus + 1) % (2);//return, text input
                            break;

                        case CREATION:
                            focus = (focus + 1) % (2);//return, text input
                            break;

                        default:
                            if(screen >= TEST_OFFSET){
                                focus = (focus + 1) % (4);//return, hint, answer, next
                            }
                            break;
                    }
                    break;

                case SDLK_h:
                    if(screen >= TEST_OFFSET)
                        hint_shown = true;
                    break;                

                case SDLK_a:
                    if(screen >= TEST_OFFSET)
                        answer_shown = true;
                    break;                

                case SDLK_SPACE:
                    if(!hint_shown && packages->get(package_testing)->get_question(package_advancement)->hint.length() > 0)
                        hint_shown = true;
                    else if(!answer_shown)
                        answer_shown = true;
                    else{//next
                        package_advancement = (package_advancement + 1) % packages->get(package_testing)->question_count();
                        hint_shown = answer_shown = false;
                    }                            
                    break;

                case SDLK_BACKSPACE:
                    if(buffer_id == 0)
                        break;
                    if(buffers[buffer_id].length() != 0)
                        buffers[buffer_id].pop_back();
                    break;

                case SDLK_RETURN:
                    if(screen == SETTINGS){
                        readDocument(buffers[buffer_id] + ".flash");
                        buffers[buffer_id] = "";
                    }else if(screen == HOME){
                        if(focus == packages->size()){
                            buffer_id = 1;
                            buffers[buffer_id] = "";
                            buffers_limits[buffer_id] = file_max_lenght;
                            screen = SETTINGS;
                            focus = -1;
                        }else if(focus == packages->size() + 1){
                            buffer_id = 1;
                            typingAllowed = true;
                            buffers[buffer_id] = "";
                            buffers_limits[buffer_id] = file_max_lenght;
                            screen = CREATION;
                            focus = -1;
                        }else{
                            openUnderlined();
                            focus = -1;
                        }
                    }
                    break;


                case SDLK_ESCAPE:
                    s->stopRunning(); // escape the program if user presses esc
                    break;

                default:
                    break;
                }

            case SDL_KEYUP:
                switch (e.key.keysym.sym){

                    default:
                        break;
                }

            default:
                break;
            }
        }
        //s->displayPortions(3, 20);
        s->refresh();
    }
    //print the packages loaded in memory 
    printPackages();

}

void FlashCards::displaySettingsButton(){
    s->emptyRect(s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07, 5, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    //settings logo
    s->filledCircle(s->W()*0.01 + s->H()*0.07/2, s->H()*0.01 + s->H()*0.07/2, s->H()*0.07*0.37, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    s->filledCircle(s->W()*0.01 + s->H()*0.07/2, s->H()*0.01 + s->H()*0.07/2, s->H()*0.07*0.25, background.r, background.g, background.b);
    int nb_spikes = 7;
    for(int i=0; i<nb_spikes; i++){
        double angle = 2*PI*i/(double)nb_spikes;
        s->filledCircle(s->W()*.01 + s->H()*.07/2 + cos(angle)*s->H()*.07*.32,
                        s->H()*.01 + s->H()*.07/2 + sin(angle)*s->H()*.07*.32,
                        s->H()*.07*0.1,
                        buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    }
    if(s->rollover(m_x, m_y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07) || (screen == HOME && focus == packages->size()))
        s->filledRect(s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07, 5, buttonColor.r*.4, buttonColor.g*.4, buttonColor.b*.4, buttonColor.a*.2);
}

void FlashCards::displayReturnButton(){
    int tmp_w, tmp_y;
    //border
    TTF_SizeText(global, "home", &tmp_w, &tmp_y);
    s->text(75 - tmp_w/2, 40 - tmp_y/2, "home", global, buttonFontColor.r, buttonFontColor.g, buttonFontColor.b, buttonFontColor.a);
    s->emptyRect(75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1, 10, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    if(s->rollover(m_x, m_y, 75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1))
        s->filledRect(75 - tmp_w*.55, 40 - tmp_y*.55, tmp_w*1.1, tmp_y*1.1, 10, buttonColor.r*.4, buttonColor.g*.4, buttonColor.b*.4, buttonColor.a*.2);

}

void FlashCards::displayCreateButton(){
    s->emptyRect(s->W()*.01, s->H()*.47, 120, 3*TTF_FontHeight(medium), 5, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    s->paragraph(s->W()*.01 + 120*.5, s->H()*.47, 200, "Create\na new\npackage", medium, CENTER, buttonFontColor.r, buttonFontColor.g, buttonFontColor.b, buttonFontColor.a);
    if(s->rollover(m_x, m_y, s->W()*.01, s->H()*.47, 120, 3*TTF_FontHeight(medium)) || (screen == HOME && focus == packages->size() + 1))
        s->filledRect(s->W()*.01, s->H()*.47, 120, 3*TTF_FontHeight(medium), 5, buttonColor.r*.4, buttonColor.g*.4, buttonColor.b*.4, buttonColor.a*.2);
}

void FlashCards::homeScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displaySettingsButton();
    displayCreateButton();
    s->setColor(150, 160, 170);
    s->text(s->W()*0.5 - TTF_FontHeight(global)*1.1, 10, "Home", global, title.r, title.g, title.b, title.a);
    displayPackagesNames();
}

void FlashCards::settingsScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displayReturnButton();
    // display instructions
    s->text(s->W()*0.5 - TTF_FontHeight(global)*1.5, 10, "Settings", global, title.r, title.g, title.b, title.a);
    s->text(s->W()*0.5 - TTF_FontHeight(global)*1.9, s->H()*0.33, "Load a file", global, title.r, title.g, title.b, title.a);
    
    // display text input + extension
    s->text(s->W()*0.5 - TTF_FontHeight(global)*3.4 + 2.5/(buffers[1].length() + 0.2), s->H()*0.45,
                (buffers[1].substr(0, 14) + ".flash").c_str(), global,
                title.r, title.g, title.b, title.a);
    // box
    TTF_SizeText(global, (buffers[1].substr(0, 14)).c_str(), &tmp_w, &tmp_h);
    if((int)(buffers[1].length()) <= file_max_lenght)
        s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*3.45, s->H()*0.45,// x , y
                    tmp_w + 2 + 2.5/(buffers[1].length() + 0.2), tmp_h,// w , h
                    10, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    else// red coloration if the text input is at it's max size
        s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*3.45, s->H()*0.45,// x , y
                        tmp_w + 2 + 2.5/(buffers[1].length() + 0.2), tmp_h,// w , h
                        10, buttonColor.r, buttonColor.g/2, buttonColor.b/2, buttonColor.a);
    
    //darken background if text focus is on the text input window
    if(typingAllowed)
        s->filledRect(s->W()*0.5 - TTF_FontHeight(global)*3.45, s->H()*0.45,// x , y
                        tmp_w + 2 + 2.5/(buffers[1].length() + 0.2), tmp_h,// w , h
                        10, 0, 0, 0, 50);//round red green blue alpha

    //add a load button to validate
    s->filledRect(s->W()*0.5 - TTF_FontHeight(global)*1 + tmp_w, s->H()*.45,
                    60, 30, 6, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    //"Load" is displayed on the button
    s->text(s->W()*0.5 - TTF_FontHeight(global) + tmp_w + 6, s->H()*.45 + 2,
            "Load", medium, background.r, background.g, background.b);
}

void FlashCards::creationScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displayReturnButton();

    //texts for instructions
    s->text(s->W()*0.5 - TTF_FontHeight(global)*2.6, 10, "Creation Menu", global, title.r, title.g, title.b, title.a);
    s->text(s->W()*0.5 - TTF_FontHeight(global)*4, s->H()*0.33, "Enter package name :", global, title.r, title.g, title.b, title.a);

    //display input
    s->text(s->W()*0.5 - TTF_FontHeight(global)*3.9, s->H()*0.45,
                buffers[1].substr(0, 14).c_str(), global,
                title.r, title.g, title.b, title.a);
    
    //add a box over the input :
    TTF_SizeText(global, (buffers[1].substr(0, 14)).c_str(), &tmp_w, &tmp_h);
    if((int)(buffers[1].length()) <= file_max_lenght)
        s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*4, s->H()*0.45,// x , y
                    tmp_w + 2, tmp_h,// w , h
                    10, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    else// red coloration if the text input is at it's max size
        s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*4, s->H()*0.45,// x , y
                        tmp_w + 2, tmp_h,// w , h
                        10, buttonColor.r, buttonColor.g/2, buttonColor.b/2, buttonColor.a);
}

/*
s->paragraph(s->W()*0.5 - TTF_FontHeight(global)*3.5, s->H()*0.45,
            s->add_ENTER_every(14, buffers[1]).c_str(), global,
            title.r, title.g, title.b, title.a);

//add a box over the input :
//TTF_SizeText(global, s->add_ENTER_every(14, buffers[1]).c_str(), &tmp_w, &tmp_h);
s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*3.5, s->H()*0.45,// x , y
            TTF_FontHeight(global)*6.5, TTF_FontHeight(global)*(1+(buffers[1].length())/14 -1/14),// w , h
            10);
*/

bool FlashCards::readDocument(std::string filename){
    std::string buff1 = ""; 
    std::string buff2 = "";
    std::string buff3 = "";
    std::string line; 
    std::ifstream myfile(filename);

    bool inPackage = false;
    bool inQuestionGrp = false;
    bool inQuestion = false;
    bool inHint = false;
    bool inAnswer = false;

    if(!myfile.is_open())
        return false;
    while (getline(myfile, line)) {
        if(!inPackage){//await for a package to be open
            for(long unsigned int i = 0 ; i < line.length() ; i++){
                if(line[i] == '<' && i + 4 <line.length())
                    if(line[i+1] == 'p' && line[i+2] == '='){
                        if(line[i+3] == ' '){
                            catchFlashReadingError(&myfile, "package name must be not null\nUse <p=<package-name>>");
                            return false;
                            }
                        else{
                            std::string title = "";
                            i+=3;
                            while(line[i] != '>'){
                                title += line[i++];
                            }
                            if(!package_is_already_loaded(title) || packages->size() == 0){
                                Package *p = new Package(title);
                                packages->pushTail(p);
                                inPackage = true;
                                i = line.length();//stop the for loop
                            }
                        }
                    }
            }
        }
        if(inPackage){
            for(long unsigned int i = 0 ; i < line.length() ; i++){
                if(line[i] == '<' && i + 3 < line.length() && line[i+1] == '/' && line[i+2] == 'p' && line[i+3] == '>'){//await for the package to be closed
                    if(inQuestionGrp){
                        catchFlashReadingError(&myfile, "package is close before question ended");
                        return false;
                    }else{
                        inPackage = false;
                        i = line.length();//stop the loop
                    }
                }else if(!inQuestionGrp && line[i] == '<' && i + 2 < line.length() && line[i+1] == 'Q' && line[i+2] == '>'){//await for a question to be opened
                    inQuestionGrp = true;
                }else if(inQuestionGrp && line[i] == '<' && i + 3 < line.length() && line[i+1] == '/' && line[i+2] == 'Q' && line[i+3] == '>'){//await for a question to be closed
                    if(inAnswer || inHint || inQuestion){
                        catchFlashReadingError(&myfile, "Close questions fields before closing the question itself");
                        return false;
                    }else if(packages->get(packages->size() - 1)->question_count() !=0 && (packages->get(packages->size() - 1)->get_lastQuestion()->question == "" || packages->get(packages->size() - 1)->get_lastQuestion()->answer == "")){
                        catchFlashReadingError(&myfile, "cannot close a question if both the question and answer fields are empty");
                        return false;
                    }else{
                        packages->get(packages->size() - 1)->add_question(buff1, buff3, buff2);
                        buff1 = buff2 = buff3 = "";
                        inQuestionGrp = false;
                    }
                }else if(inQuestionGrp && line[i] == '<' && i + 2 < line.length() && line[i + 1] == 'q' && line[i + 2] == '>'){//await for a question_field to be read
                    if(inAnswer || inHint || inQuestion){
                        catchFlashReadingError(&myfile, "Close questions fields before giving the question field");
                        return false;
                        }
                    inQuestion = true;
                    i+=3;
                }else if(inQuestionGrp && line[i] == '<' && i + 2 < line.length() && line[i + 1] == 'h' && line[i + 2] == '>'){//await for a hint to be read
                    if(inAnswer || inHint || inQuestion){
                        catchFlashReadingError(&myfile, "Close questions fields before giving the hint field");
                        return false;
                        }
                    inHint = true;
                    i+=3;
                }else if(inQuestionGrp && line[i] == '<' && i + 2 < line.length() && line[i + 1] == 'a' && line[i + 2] == '>'){//await for an answer to be read
                    if(inAnswer || inHint || inQuestion){
                        catchFlashReadingError(&myfile, "Close questions fields before giving the answer field");
                        return false;
                        }
                    inAnswer = true;
                    i+=3;
                }else if(inQuestionGrp && line[i] == '<' && i + 3 < line.length() && line[i + 1] == '/' && line[i + 2] == 'q' && line[i+3] == '>'){//await for a question_field to be closed
                    inQuestion = false;
                }else if(inQuestionGrp && line[i] == '<' && i + 3 < line.length() && line[i + 1] == '/' && line[i + 2] == 'a' && line[i+3] == '>'){//await for an answer to be closed
                    inAnswer = false;
                }else if(inQuestionGrp && line[i] == '<' && i + 3 < line.length() && line[i + 1] == '/' && line[i + 2] == 'h' && line[i+3] == '>'){//await for a hint to be closed
                    inHint = false;
                }

                //============add text

                if(inQuestionGrp){
                    if(inQuestion){
                        replace_n(&buff1);
                        buff1 += line[i];
                    }else if(inHint){
                        replace_n(&buff2);
                        buff2 += line[i];
                    }else if(inAnswer){
                        replace_n(&buff3);
                        buff3 += line[i];
                        }
                }
            }
        }
    }   
    myfile.close();
    return true;
}

void FlashCards::replace_n(std::string*s){
    size_t pos = s->find("\\n");
    while (pos != std::string::npos)
    {
        s->erase(pos, 2);
        s->insert(pos, "\n");
        pos = s->find("\\n", pos + 1);
    }
}

void FlashCards::printPackages()const{
    for(int i=0; i< packages->size(); i++)
        packages->get(i)->printPackage();
}

void FlashCards::displayPackagesNames()const{
    //display text : 
    int hh, ww, max_width = s->W()*.05;
    int txt_height = s->H()*.33;
    for(int i = 0; i<packages->size(); i++){
        TTF_SizeText(medium, packages->get(i)->getTitle().c_str(), &ww, &hh);
        s->text(s->W()/2 - ww/2, txt_height, packages->get(i)->getTitle().c_str(), medium, title.r, title.g, title.b, title.a);
        if(packages->get(i)->get_underline() == TITLE_UNDERLINE)
            s->line(s->W()/2 - ww/2, txt_height + hh, s->W()/2 + ww/2, txt_height + hh, title.r, title.g, title.b, title.a);
        txt_height += hh*1.2;
        max_width = max(max_width, ww);
    }
    //display box :
    int box_padding = 5;
    if(packages->size() != 0)
        s->emptyRect(s->W()*.5 - max_width*.5 - box_padding, s->H()*.33 - box_padding, max_width + 2*box_padding,
        txt_height - s->H()*.33 + 2*box_padding, 10, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
}

void FlashCards::catchFlashReadingError(std::ifstream *reader, const char * error_message){
    if(reader->is_open())
        reader->close();
    std::cout << "Error in reading .flashDocument : " << error_message << std::endl;
}

int FlashCards::countLines(const std::string& filePath){
    std::ifstream file(filePath);
    if(!file.is_open())
        return 0;
    std::string line;
    int count = 0;
    while (std::getline(file, line))
        count++;
    return count;
}

void FlashCards::openUnderlined(){
    for(int i = 0 ; i < packages->size(); i++)
        if(packages->get(i)->get_underline() == TITLE_UNDERLINE){
            packages->get(i)->set_underline(NO_UNDERLINE);
            screen = TEST_OFFSET + i;
            package_testing = i;
        }
}

void FlashCards::testScreen(int package_id){
    int nb_questions = packages->get(package_id)->question_count();
    if(package_id < 0)
        return;
    if(package_id >= packages->size()){
        std::cout << "\n\nerror package_id is out of range\n\n" << std::endl;
        return;
    }
    //=== return button
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displayReturnButton();
    //=== title of the package
    int ww, hh;
    TTF_SizeText(global, packages->get(package_id)->getTitle().c_str(), &ww, &hh);
    s->text(s->W()/2 - ww/2, 10, packages->get(package_id)->getTitle().c_str(), global, title.r, title.g, title.b, title.a);
    //=== breakline
    s->line(0, 10 + hh + 20, s->W(), 10 + hh + 20);
    //=== display the package advancement bar : 
    progressionBarHorizontal(15, s->H() - 30, 200, 15, 10, package_advancement, nb_questions);
    // and its percentage
    if(nb_questions != 0){
        int percentage = package_advancement*100 / nb_questions;
        std::string strr_buff = std::to_string(percentage) + " %";
        s->text(225, s->H() - 30, strr_buff.c_str(), small, buttonFontColor.r, buttonFontColor.g, buttonFontColor.b, buttonFontColor.a);
    }

    //=== display the question
    s->paragraph(s->W()*.5, s->H()*.1, packages->get(package_id)->get_question(package_advancement)->question.c_str(), medium);
    //=== display the hint
    if((hint_shown || answer_shown) && packages->get(package_id)->get_question(package_advancement)->hint.length() > 0)
        s->paragraph(s->W()*.25 , s->H()*.5, s->W()*.2, packages->get(package_id)->get_question(package_advancement)->hint.c_str(), medium, CENTER, hintColor.r, hintColor.g, hintColor.b, hintColor.a);
    else if(packages->get(package_id)->get_question(package_advancement)->hint.length() > 0)
        displayHintButton();
    //=== display the answer
    if(answer_shown)
        s->paragraph(s->W()*.75 , s->H()*.5, s->W()*.2, packages->get(package_id)->get_question(package_advancement)->answer.c_str(), medium, CENTER, answerColor.r, answerColor.g, answerColor.b, answerColor.a);
    else
        displayAnswerButton();
    displayNextButton();

}

void FlashCards::progressionBarHorizontal(int x, int y, int w, int h, int rounding, int value, int total){
    if(!(value <= 0 || value > total || total <= 0))//wrong arguments gestion
        s->filledRect(x, y, w*(value/(double)total), h, rounding, buttonFontColor.r, buttonFontColor.g, buttonFontColor.b, buttonFontColor.a);
    s->emptyRect(x, y, w, h, rounding, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);

}

void FlashCards::displayAnswerButton(){
    int tmp_w, tmp_y;
    //inside of button
    s->filledRect(s->W()*.75 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, answerColor.r, answerColor.g, answerColor.b, answerColor.a);
    //border
    s->emptyRect(s->W()*.75 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, 0, 0, 0, 255);
    TTF_SizeText(global, "show answer", &tmp_w, &tmp_y);
    s->text(s->W()*.75 - tmp_w/2, s->H()*.4 - tmp_y/2, "show answer", global, background.r, background.g, background.b, background.a);
}

void FlashCards::displayHintButton(){
    int tmp_w, tmp_y;
    //inside of button
    s->filledRect(s->W()*.25 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, hintColor.r, hintColor.g, hintColor.b, hintColor.a);
    //border
    s->emptyRect(s->W()*.25 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, 0, 0, 0, 255);
    TTF_SizeText(global, "show hint", &tmp_w, &tmp_y);
    s->text(s->W()*.25 - tmp_w/2, s->H()*.4 - tmp_y/2, "show hint", global, background.r, background.g, background.b, background.a);
}

void FlashCards::displayNextButton(){
    int tmp_w, tmp_y;
    //border
    s->emptyRect(s->W()*.5 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, 0, 0, 0, 0);
    //inside of button
    s->filledRect(s->W()*.5 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, nextColor.r, nextColor.g, nextColor.b, nextColor.a);
    //border
    s->emptyRect(s->W()*.5 - test_button_w/2, s->H()*.4 - test_button_h/2, test_button_w, test_button_h, 10, 0, 0, 0, 255);
    TTF_SizeText(global, "next", &tmp_w, &tmp_y);
    s->text(s->W()*.5 - tmp_w/2, s->H()*.4 - tmp_y/2, "next", global, background.r, background.g, background.b, background.a);
}

bool FlashCards::package_is_already_loaded(std::string package_title){
    for(int i=0; i< packages->size();i++)
        if(packages->get(i)->getTitle() == package_title)
            return true;
    return false;
}

void FlashCards::update_underline(){
    if(screen == HOME){
        int txt_heightt = s->H()*.33;
        for(int i = 0; i < packages->size(); i++){
            int ww, hh;
            TTF_SizeText(medium, packages->get(i)->getTitle().c_str(), &ww, &hh);
            if(s->rollover(m_x, m_y, s->W()/2 - ww/2, txt_heightt, ww, hh) || focus == i)
                packages->get(i)->set_underline(TITLE_UNDERLINE);
            else
                packages->get(i)->set_underline(NO_UNDERLINE);
            txt_heightt += hh*1.2;
        }
    }
}


















