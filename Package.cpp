#include "Package.hpp"

using namespace std;

Package::Package(){
    questions = new LinkedList<Question>();
    package_name = "no_name";
    nb_questions = 0;
}

Package::Package(const std::string title){
    questions = new LinkedList<Question>();
    package_name = title;
    nb_questions = 0;
}

Package::~Package(){
    delete questions;
}

bool Package::add_question(Question q){
    if(q.question == "" || q.answer == "")
        return false;
    questions->pushTail(q);
    nb_questions++;
    return true;
}

bool Package::add_question(std::string question, std::string answer, std::string hint){
    if(question == "" || answer == "")
        return false;
    Question q;
    q.question = question;
    q.answer = answer;
    q.hint = hint;
    questions->pushTail(q);
    nb_questions++;
    return true;
}

Question Package::get_question(int index){
    if(index < 0 || index >= questions->size())
        throw std::invalid_argument("index out of range of the questions linked list in get_question(int)");
    return questions->get(index);
}

bool Package::remove_question(int index){
    if(index < 0 || index >= questions->size())
        return false;
    questions->pop(index);
    nb_questions--;
}



