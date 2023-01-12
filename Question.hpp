#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <iostream>
#include <sstream>


class Question{

    public :
        std::string question;
        std::string answer;
        std::string hint;

        Question();

        ~Question();

        bool hintIsPresent();

        void resetQuestion();
};



#endif