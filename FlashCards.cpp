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

        if(screen == HOME){
            s->bg(100);
            s->text(s->W()*0.5 - TTF_FontHeight(global)*1.1, 10, "Home", global);
            //settings button
            displaySettingsButton();
        }else if(screen == SETTINGS){
            s->bg(150);
            s->text(s->W()*0.5 - TTF_FontHeight(global)*1.5, 10, "Settings", global);
            displayReturnButton();
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
                if (screen == HOME && s->rollover(e.button.x, e.button.y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07))
                    screen = SETTINGS;
                else if(screen == SETTINGS && s->rollover(e.button.x, e.button.y, s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.035))
                    screen = HOME;
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
        //s->displayPortions(70, 70);
        s->refreshAndDetails();

    }
}


void FlashCards::displaySettingsButton(){
    s->emptyRect(s->W()*0.01, s->H()*0.01, s->H()*0.07, s->H()*0.07, 5);
}

void FlashCards::displayReturnButton(){
    s->text(s->W()*0.02, s->H()*0.02, "return", small);
    s->emptyRect(s->W()*0.01, s->H()*0.015, s->H()*0.09, s->H()*0.045, 5);
}


