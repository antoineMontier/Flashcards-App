#include "FlashCards.hpp"

using namespace std;

FlashCards::FlashCards(){
    screen = HOME;
    s = new SDL_Screen(1080, 720, "FlashCards", 60);
    s->setFont(&global, "Roboto_m.ttf", 30);
    s->setFont(&small, "Roboto_m.ttf", 18);
}

FlashCards::~FlashCards(){
    TTF_CloseFont(global);
    s = nullptr;
}

void FlashCards::run(){
    while(s->isRunning()){//main loop

        switch(screen){
            
            case HOME:
                s->setColor(140, 255, 30);
                s->bg();
                displaySettingsButton();
                displayCreateButton();
                s->setColor(150, 160, 170);
                s->text(s->W()*0.5 - TTF_FontHeight(global)*1.1, 10, "Home", global);
                break;

            case SETTINGS:
                s->bg(150);
                s->text(s->W()*0.5 - TTF_FontHeight(global)*1.5, 10, "Settings", global);
                displayReturnButton();
                break;

            case CREATION:
                s->setColor(30, 150, 255);
                s->bg();
                displayReturnButton();
                s->text(s->W()*0.5 - TTF_FontHeight(global)*1.5, 10, "Creation Menu", global);
                break;

            default:
                s->stopRunning();
                break;
        }
        
        while (SDL_PollEvent(&e)){ // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (e.type){

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
                        if(s->rollover(e.button.x, e.button.y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07))
                            screen = SETTINGS;
                        else if(s->rollover(e.button.x, e.button.y, s->W()*.01, s->H()*.47, 120, 100))
                            screen = CREATION;
                        break;

                    case SETTINGS:
                        if(s->rollover(e.button.x, e.button.y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.035))
                            screen = HOME;
                        break;
                    
                    case CREATION:
                        if(s->rollover(e.button.x, e.button.y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.035))
                            screen = HOME;
                        break;

                    default:
                        break;
                }
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (e.key.keysym.sym){ // returns the key ('0' ; 'e' ; 'SPACE'...)

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
        s->displayPortions(70, 8);
        s->refreshAndDetails();

    }
}


void FlashCards::displaySettingsButton(){
    s->emptyRect(s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07, 5, 60, 60, 60);
    //settings logo
    s->filledCircle(s->W()*0.01 + s->H()*0.07/2, s->H()*0.01 + s->H()*0.07/2, s->H()*0.07*0.37, 60, 60, 60);
    s->filledCircle(s->W()*0.01 + s->H()*0.07/2, s->H()*0.01 + s->H()*0.07/2, s->H()*0.07*0.25);
    int nb_spikes = 7;
    for(int i=0; i<nb_spikes; i++){
        double angle = 2*PI*i/(double)nb_spikes;
        s->filledCircle(s->W()*.01 + s->H()*.07/2 + cos(angle)*s->H()*.07*.32,
                        s->H()*.01 + s->H()*.07/2 + sin(angle)*s->H()*.07*.32,
                        s->H()*.07*0.1,
                        60, 60, 60);
    }
}

void FlashCards::displayReturnButton(){
    s->text(s->W()*0.02, s->H()*0.02, "return", small, 60, 60, 60);
    s->emptyRect(s->W()*0.01, s->H()*0.015, s->H()*0.09, s->H()*0.045, 5);
}

void FlashCards::displayCreateButton(){
    s->emptyRect(s->W()*.01, s->H()*.47, 120, 100, 5, 60, 60, 60);
    s->text(s->W()*.01 + 10, s->H()*.47, "Create", global, 60, 60, 60);
    s->text(s->W()*.01 + 15, s->H()*.47 + 30, "a new", global, 60, 60, 60);
    s->text(s->W()*.01, s->H()*.47 + 60, "package", global, 60, 60, 60);
}
