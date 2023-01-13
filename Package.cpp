#include "Package.hpp"

using namespace std;

Package::Package(){
    questions = new LinkedList<Question*>();
    package_name = "no_name";
    underline = -1;
}

Package::Package(const std::string title){
    questions = new LinkedList<Question*>();
    package_name = title;
    underline = -1;
}

Package::~Package(){
    for(int i=0; i<questions->size(); i++)
        delete(questions->get(i));
    delete(questions);
}

bool Package::add_question(Question *q){
    if(q->question == "" || q->answer == "")
        return false;
    questions->pushTail(q);
    return true;
}

bool Package::add_question(std::string question, std::string answer, std::string hint){
    if(question == "" || answer == "")
        return false;
    Question *q = new Question();
    q->question = question;
    q->answer = answer;
    q->hint = hint;
    questions->pushTail(q);
    return true;
}

Question* Package::get_question(int index) const{
    if(index < 0 || index >= questions->size())
        throw std::invalid_argument("index out of range of the questions linked list in get_question(int)");
    return questions->get(index);
}

bool Package::remove_question(int index){
    if(index < 0 || index >= questions->size())
        return false;
    questions->pop(index);
    return true;
}

int Package::question_count()const{return questions->size();}


bool Package::setTitle(const std::string title){
    package_name = title;
    return true;
}

std::string Package::getTitle() const{return package_name;}

bool Package::setQuestion(int index, const std::string question, const std::string answer, const std::string hint){
    if(index < 0 || index >= questions->size())
        return false;
    Question *q = new Question();
    q->answer = answer;
    q->hint = hint;
    q->question = question;
    questions->set(index, q);
    return true;
}

Question* Package::get_lastQuestion() const{
    if(questions->size() == 0)
        throw new std::runtime_error("cannot get last question from an empty package\n");
    return questions->get(questions->size() - 1);
}

void Package::printPackage()const{
    std::cout << "===== " << package_name << " =====\n";
    for(int i=0; i<questions->size(); i++)
        questions->get(i)->printQuestion();
    std::cout << "====================================\n";
}



int Package::get_underline()const{return underline;}

void Package::set_underline(int id){underline=id;}
