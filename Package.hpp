#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "Question.hpp"
#include <fstream>
#include "LinkedList.cpp"

#define NO_UNDERLINE (-1)
#define TITLE_UNDERLINE (0)

class Package{

    private :
        std::string package_name;

        LinkedList<Question*>* questions;

        int underline;

    public :

        int get_underline()const;
        void set_underline(int id);

        Package();
        Package(const std::string title);

        ~Package();

        bool add_question(Question *q);
        bool add_question(std::string question, std::string answer, std::string hint="");

        Question* get_question(int index) const;

        bool remove_question(int index);

        int question_count() const;

        bool setTitle(const std::string title);

        std::string getTitle() const;

        bool setQuestion(int index, const std::string question, const std::string answer, const std::string hint="");

        Question* get_lastQuestion() const;

        void printPackage()const;
};
#endif