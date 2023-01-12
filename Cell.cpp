#include "Cell.hpp"
using namespace std;


template <class T>
Cell<T>::Cell(T contenu, Cell<T> nextcell){
    this->content = contenu;
    this->next = &nextcell;
}


template <class T>
Cell<T>::~Cell(){}


template <class T>
Cell<T>::Cell(){
    content = NULL;
    next = nullptr;
}

template <class T>
Cell<T>::Cell(const Cell<T>& other){
    this->content = other.content;
    this->next = other.next;
}

template <class T>
Cell<T>::Cell(T cont){
    content = cont;
    next = nullptr;
}

template <class T>
T Cell<T>::getContent() const{
    return content;
}

template <class T>
void Cell<T>::setContent(T cont){
    content = cont;
}

template <class T>
Cell<T> Cell<T>::getNext() const{
    return next == nullptr ? throw std::invalid_argument("no cell founded") : *next;
}

template <class T>
Cell<T>* Cell<T>::getNextPtr() const{
    return next == nullptr ? throw std::invalid_argument("no cell founded") : next;
}

template <class T>
void Cell<T>::setNext(Cell<T> *nextcell){
    next = nextcell;
}

template <class T>
bool Cell<T>::equal(Cell<T> other) const{
    return content == other.content && next == other.next;
}

template <class T>
const string Cell<T>::toString() const//const means read-only
{
    ostringstream buffer;
    buffer<<fixed<<showpoint<<setprecision(4)//float and double display
    <<content;
    return buffer.str();
}

template <class T>
bool Cell<T>::hasNext() const{
    return next != nullptr;
}
