#include "SDL_Screen.hpp"
#define HOME 0
#define SETTINGS (-1)



int main(int argc, char **argv){
    SDL_Screen s(1080, 720, "FlashCards", 60);
    SDL_Event e;
    TTF_Font *global;
    s.setFont(&global, "Roboto_m.ttf", 30);
    int screen = HOME;
    while(s.isRunning()){//main loop

        if(screen == HOME){
            s.bg(100);
            s.text(s.W()/2, 10, "Home", global);
        }
        
        
        while (SDL_PollEvent(&e)){ // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (e.type)
            {
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    s.updateSize();
                break;
            case SDL_QUIT:
                s.stopRunning(); // quit the program if the user closes the window
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (e.key.keysym.sym){ // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_ESCAPE:
                    s.stopRunning(); // escape the program if user presses esc
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

        s.refreshAndDetails();

    }
    TTF_CloseFont(global);
    return 0;
}