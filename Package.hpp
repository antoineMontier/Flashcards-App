#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "Question.hpp"
#include <fstream>
#include "LinkedList.hpp"

class Package{

    private :
        std::string package_name;
        int nb_questions;

        LinkedList<Question>* questions;

    public :

        Package();
        Package(const std::string title);

        ~Package();

        bool add_question(Question q);
        bool add_question(std::string question, std::string answer, std::string hint="");

        Question get_question(int index);

        bool remove_question(int index);
};
#endif