#ifndef FLASHCARDS_HPP
#define FLASHCARDS_HPP


/*
# Flashcards-App

flash card app is a program to study using flashcard : with a question, an hidden hint and a hidden answer

it's using files with a .flash extension;
    flash files contains  packages names,
                        for each package there is a question number
                        in each package there are questions
                        for each question there is an answer
                        for each question there is an optionnal hint

    the beginning of a package is <p=<package-name> Q=15>
    the end of a package is <\p>
    
    the beginning of question is <Q>
    the end of a question is <\Q>
    
    the question field starts and ends with <q> </q>
    the hint field starts and ends with <h> </h>
    the answer field starts and ends with <a> </a>*/


#define HOME 0
#define SETTINGS (-1)
#define CREATION 2
#define TEST_CENTER 1


#define PI (3.14159265)

#include <fstream>
#include "SDL_Screen.hpp"
#include "Package.hpp"

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

        LinkedList<Package*> *packages;

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

        bool readDocument(std::string filename);//idea : read the total lines numer of the document and then show a progression bar depending on how much lines have been read

        void printPackages()const;
};


#endif