#include "Cell.cpp"
#include <functional>

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

using namespace std;

template <class T>

class LinkedList{
    
    private:
        Cell<T> * head;
        Cell<T> * tail;

    public:

        LinkedList();

        LinkedList(T content);

        ~LinkedList();

        void push(T content);

        void pushTail(T content);

        T pop();

        T popTail();

        int size() const;

        T get(int index) const;

        void set(int index, T content);

        T pop(int index);

        void push(int index, T content);

        bool equals(LinkedList<T> other) const;

        LinkedList<T> bubbleSort(std::function <bool(T, T)> comparator);

        LinkedList<T> map(std::function <T(T)> mapfunction) const;

        LinkedList<T> filter(std::function <bool(T)> filterfunction) const;

        bool contains(T obj) const;

        bool remove(T obj);

        bool removeAll(T obj);

        T reduce(T first, std::function<T(T, T)> reducefunction) const;

        T extremum(std::function <bool(T, T)> comparator) const;

        LinkedList<T> reverse() const;

        LinkedList<T> merge(LinkedList<T> other) const;

        const string toString() const;

};


#endif