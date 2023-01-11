#ifndef FLASHCARDS_HPP
#define FLASHCARDS_HPP

#define HOME 0
#define SETTINGS (-1)


#define PI (3.14159265)

#include "SDL_Screen.hpp"

class FlashCards{

    private:

        SDL_Screen *s;
        SDL_Event e;
        TTF_Font *global;
        TTF_Font *small;
        int screen;

    public:

        FlashCards();

        ~FlashCards();

        void run();

        void displaySettingsButton();

        void displayReturnButton();



};


#endif