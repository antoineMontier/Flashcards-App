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
#define CREATION (2)
#define TEST_CENTER (1)
#define TEST_OFFSET (52651)

#define MAX_PACKAGES (16)


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
        SDL_Color answerColor;
        SDL_Color hintColor;
        SDL_Color nextColor;
        bool typingAllowed;
        int typingNow;
        int buffer_id;
        std::string buffers[3];
        int buffers_limits[3];
        const int file_max_lenght = 13;
        char*buffer;
        int tmp_h, tmp_w;
        int package_testing;//-1 if no package selected
        int package_advancement;//0 if no advancement
        bool hint_shown;
        bool answer_shown;
        const int test_button_w = 200;
        const int test_button_h = 60;

        int m_x, m_y;

        LinkedList<Package*> *packages;

        void catchFlashReadingError(std::ifstream *reader, const char * error_message);

        int countLines(const std::string& filePath);

        void openUnderlined();

        void replace_n(std::string*s);

    public:

        FlashCards();

        ~FlashCards();

        void run();

        void displaySettingsButton();

        void displayReturnButton();

        void displayCreateButton();

        void settingsScreen();

        void homeScreen();//idea : be able to scroll the package title window displayed in the center of the screen

        void creationScreen();

        void testScreen(int package_id);

        bool readDocument(std::string filename);

        void printPackages()const;

        void displayPackagesNames()const;

        void progressionBarHorizontal(int x, int y, int w, int h, int rounding, int value, int total);

        void displayAnswerButton();

        void displayHintButton();

        void displayNextButton();

        bool package_is_already_loaded(std::string package_title);
};


#endif