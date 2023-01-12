#ifndef FLASHCARDS_HPP
#define FLASHCARDS_HPP

#define HOME 0
#define SETTINGS (-1)
#define CREATION 2
#define TEST_CENTER 1


#define PI (3.14159265)

#include <fstream>
#include "SDL_Screen.hpp"

class FlashCards{

    private:

        SDL_Screen *s;
        SDL_Event e;
        TTF_Font *global;
        TTF_Font *small;
        TTF_Font *medium;
        int screen;
        SDL_Color title;
        SDL_Color background;
        SDL_Color buttonColor;
        SDL_Color buttonFontColor;
        bool typingAllowed;
        int typingNow;
        int buffer_id;
        std::string buffers[3];
        int buffers_limits[3];
        const int file_max_lenght = 13;
        const char*buffer;
        int tmp_h, tmp_w;

    public:

        FlashCards();

        ~FlashCards();

        void run();

        void displaySettingsButton();

        void displayReturnButton();

        void displayCreateButton();

        void settingsScreen();

        void homeScreen();

        void creationScreen();

        bool readDocument(std::string filename);

};


#endif