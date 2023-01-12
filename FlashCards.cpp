#include "FlashCards.hpp"

using namespace std;

FlashCards::FlashCards(){
    screen = HOME;
    s = new SDL_Screen(1080, 720, "FlashCards", 60);
    s->setFont(&global, "Roboto_m.ttf", 30);
    s->setFont(&small, "Roboto_m.ttf", 18);
    title = {60, 60, 60, 255};
    background = {150, 150, 230, 255};
    buttonColor = {100, 100, 100, 255};
    buttonFontColor = {0, 128, 255, 255};
    typingAllowed = false;
    typingNow = 0;
    buffers[0] = "";
    buffers[1] = "";
    buffers[2] = "";
    lines[0] = 0;
    lines[1] = 0;
    lines[2] = 0;
}

FlashCards::~FlashCards(){
    TTF_CloseFont(global);
    delete(s);
    s = nullptr;
}

void FlashCards::run(){
    while(s->isRunning()){//main loop
        switch(screen){
            
            case HOME:
                homeScreen();
                break;

            case SETTINGS:
                settingsScreen();
                break;

            case CREATION:
                creationScreen();
                break;

            default:
                s->stopRunning();//a bug happened
                break;
        }
        
        while (SDL_PollEvent(&e)){ // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (e.type){

            case SDL_TEXTINPUT:
                if(typingAllowed)
                    buffers[buffer_id] += e.text.text;
                break;


            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    s->updateSize();
                break;

            case SDL_QUIT:
                s->stopRunning(); // quit the program if the user closes the window
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch(screen){
                    case HOME:
                        if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.01, s->H()*.07, s->H()*.07))
                            screen = SETTINGS;
                        else if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.47, 120, 100)){
                            buffer_id = 1;
                            typingAllowed = true;
                            screen = CREATION;
                        }
                        break;

                    case SETTINGS:
                        if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.01, s->H()*.07, s->H()*.035))
                            screen = HOME;
                        break;
                    
                    case CREATION:
                        if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.01, s->H()*.07, s->H()*.035)){
                            typingAllowed = false;
                            typingNow = false;
                            buffers[buffer_id = 1] = "";//clean buffer
                            screen = HOME;
                        }
                        break;

                    default:
                        break;
                }
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (e.key.keysym.sym){ // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_BACKSPACE:
                    if(buffer_id == 0)
                        break;
                    if(buffers[buffer_id].length() != 0)
                        buffers[buffer_id].pop_back();

                case SDLK_RETURN:
                    if(typingNow)
                        typingNow = 0;
                    else if(!typingNow)
                        typingNow = buffer_id;
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
        s->displayPortions(25, 8);
        s->refreshAndDetails();

    }
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
}

void FlashCards::displayReturnButton(){
    s->text(s->W()*0.02, s->H()*0.02, "return", small, buttonFontColor.r, buttonFontColor.b, buttonFontColor.b, buttonFontColor.a);
    s->emptyRect(s->W()*0.01, s->H()*0.015, s->H()*0.09, s->H()*0.045, 5);
}

void FlashCards::displayCreateButton(){
    s->emptyRect(s->W()*.01, s->H()*.47, 120, 100, 5, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    s->text(s->W()*.01 + 10, s->H()*.47, "Create", global, buttonFontColor.r, buttonFontColor.b, buttonFontColor.b, buttonFontColor.a);
    s->text(s->W()*.01 + 15, s->H()*.47 + 30, "a new", global, buttonFontColor.r, buttonFontColor.b, buttonFontColor.b, buttonFontColor.a);
    s->text(s->W()*.01 + 4, s->H()*.47 + 60, "package", global, buttonFontColor.r, buttonFontColor.b, buttonFontColor.b, buttonFontColor.a);
}

void FlashCards::homeScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displaySettingsButton();
    displayCreateButton();
    s->setColor(150, 160, 170);
    s->text(s->W()*0.5 - TTF_FontHeight(global)*1.1, 10, "Home", global, title.r, title.g, title.b, title.a);
}

void FlashCards::settingsScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displayReturnButton();
    s->text(s->W()*0.5 - TTF_FontHeight(global)*1.5, 10, "Settings", global, title.r, title.g, title.b, title.a);
}

void FlashCards::creationScreen(){
    s->setColor(background.r, background.g, background.b);
    s->bg();
    s->setColor(buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    displayReturnButton();

    s->text(s->W()*0.5 - TTF_FontHeight(global)*2.6, 10, "Creation Menu", global, title.r, title.g, title.b, title.a);
    s->text(s->W()*0.5 - TTF_FontHeight(global)*4, s->H()*0.33, "Enter package name :", global, title.r, title.g, title.b, title.a);

    buffer = buffers[1].c_str();
    s->paragraph(s->W()*0.5 - TTF_FontHeight(global)*3.5, s->H()*0.45,
                s->add_ENTER_every(14, buffers[1]).c_str(), global,
                title.r, title.g, title.b, title.a);
    
    //add a box over the input :
    //TTF_SizeText(global, s->add_ENTER_every(14, buffers[1]).c_str(), &tmp_w, &tmp_h);
    s->emptyRect(s->W()*0.5 - TTF_FontHeight(global)*3.5, s->H()*0.45,// x , y
                TTF_FontHeight(global)*7, TTF_FontHeight(global)*(1+(buffers[1].length())/14 -1/14),// w , h
                10);
}