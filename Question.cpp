#include "Question.hpp"

using namespace std;

Question::Question(){
    answer = "";
    question = "";
    hint = "";
}

Question::~Question(){
}

bool Question::hintIsPresent(){
    return hint.size() != 0;
}

void Question::resetQuestion(){
    answer = "";
    question = "";
    hint = "";
}