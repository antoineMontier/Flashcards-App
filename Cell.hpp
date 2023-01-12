#include <iostream>
#include <sstream>
#include <iomanip>

#ifndef CELL_HPP
#define CELL_HPP

using namespace std;

template <class T>
class Cell{
    
    private:
        T content;
        Cell<T> *next;

    public:
        Cell(T cont, Cell<T> nextcell);

        Cell(const Cell<T>& other);

        Cell();

        Cell(T cont);

        ~Cell();

        T getContent() const;

        void setContent(T cont);

        Cell<T> getNext() const;

        Cell<T>* getNextPtr() const;

        void setNext(Cell<T> *nextcell);

        bool equal(Cell<T> other) const;

        const string toString() const;

        bool hasNext() const;

};

#endif