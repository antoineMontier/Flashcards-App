#include "LinkedList.hpp"
using namespace std;

template <class T>
LinkedList<T>::LinkedList()
{
    head = nullptr;
    tail = nullptr;
}


template <typename T>
LinkedList<T>::~LinkedList()
{
    if (head == nullptr)
        return;
    Cell<T>* current = head;
    while (current->hasNext())
    {
        Cell<T>* next = current->getNextPtr();
        delete current;
        current = next;
    }
    delete tail;
    head = nullptr;
    tail = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(T content)
{
    Cell<T> *n_head = new Cell(content); // when using a pointer, add the "new"key word in front of the constructor
    // std::cout<<"n_head = "<<n_head->toString()<<endl;
    tail = n_head;
    // std::cout<<"tail = "<<(*tail).toString()<<endl;
    head = tail;
    // std::cout<<"head = "<<(*head).toString()<<endl;
}

template <class T>
const string LinkedList<T>::toString() const
{
    if (head == nullptr)
    {
        return "[-]";
    }
    ostringstream buffer;
    buffer << "[";
    Cell<T> *walker = head;
    // std::cout<<"walker initialized at "<<(*walker).toString()<<endl;
    while ((*walker).hasNext())
    {
        // std::cout<<"entering loop head = "<<(*head).toString()<<" next head = "<<(*head).getNext().toString()<<endl;
        // std::cout<< "loop entry, walker = "<<(*walker).toString()<<endl;
        buffer << (*walker).toString() << ", ";
        // std::cout<<"buffer loaded"<<buffer.str()<<endl;
        walker = (*walker).getNextPtr();
        // std::cout<< "walker increased"<<endl;
    }
    buffer << (*tail).toString() << "]";
    return buffer.str();
}

template <class T>
void LinkedList<T>::push(T content)
{
    // std::cout<<"1"<<endl;
    Cell<T> *c = new Cell(content);

    // std::cout<<"head = "<< head<<" tail = "<< tail <<" c = " << c <<endl;

    // std::cout<<"2"<<endl;
    if (head == nullptr)
    {
        // std::cout<<"1st"<<endl;
        head = c;
        // std::cout<<"1st ended"<<endl;
        tail = c;
        return;
    }
    else if (head == tail)
    {
        // std::cout<<"2nd"<<endl;
        c->setNext(tail);
        head = c;
        // std::cout<<"2nd ended"<<endl;
        // no changes for tail
        // std::cout<<"after push head = "<<(*head).toString()<<" next head = "<<(*head).getNext().toString()<<" tail = "<<(*tail).toString()<<endl;
        // std::cout<<"head = "<< head<<" head next = "<<  (*head).getNext().toString()  <<" tail = "<< tail <<" c = " << c <<endl;

        return;
    }
    else
    {
        // std::cout<<"3rd"<<endl;
        c->setNext(head);
        // std::cout<<"3rd ended"<<endl;
        head = c;
    }
}

template <class T>
void LinkedList<T>::pushTail(T content)
{
    Cell<T> *c = new Cell(content);
    if (head == nullptr)
    {
        head = c;
        tail = c;
        return;
    }
    if (head == tail)
    {
        head->setNext(c);
        tail = c;
        return;
    }
    else
    {
        tail->setNext(c);
        tail = c;
        return;
    }
}

template <class T>
T LinkedList<T>::pop()
{
    if (head == nullptr)
        throw std::invalid_argument("empty list cannot be popped");
    if (head == tail)
    {
        T result = head->getContent();
        head = nullptr;
        tail = nullptr;
        return result;
    }
    else
    {
        T result = head->getContent();
        head = head->getNextPtr();
        return result;
    }
}

template <class T>
T LinkedList<T>::popTail()
{
    if (head == nullptr)
        throw std::invalid_argument("empty list cannot be popped");
    if (head == tail)
    {
        T result = head->getContent();
        head = nullptr;
        tail = nullptr;
        return result;
    }
    else
    {
        T result = tail->getContent();
        Cell<T> *walker = head;
        while (walker->getNextPtr() != tail)
            walker = walker->getNextPtr();
        tail = walker;
        tail->setNext(nullptr);
        return result;
    }
}

template <class T>
int LinkedList<T>::size() const
{
    if (head == nullptr)
        return 0;
    if (head == tail)
        return 1;
    int count = 2;
    Cell<T> *walker = head;
    while (walker->getNextPtr() != tail)
    {
        walker = walker->getNextPtr();
        count++;
    }
    return count;
}

template <class T>
T LinkedList<T>::get(int index) const
{
    if (index < 0 || index >= size())
        throw std::invalid_argument("list index out of bounds");
    if (index == 0)
        return head->getContent();
    if (index == size() - 1)
        return tail->getContent();
    Cell<T> *walker = head;
    for (int i = 0; i < index; i++)
        walker = walker->getNextPtr();
    return walker->getContent();
}

template <class T>
void LinkedList<T>::set(int index, T content)
{
    if (index < 0 || index >= size())
        throw std::invalid_argument("list index out of bounds");
    if (index == 0)
        return head->setContent(content);
    if (index == size() - 1)
        return tail->setContent(content);
    Cell<T> *walker = head;
    for (int i = 0; i < index; i++)
        walker = walker->getNextPtr();
    walker->setContent(content);
}

template <class T>
T LinkedList<T>::pop(int index)
{
    if (index < 0 || index >= size())
        throw std::invalid_argument("list index out of bounds");
    if (index == 0)
        return pop();
    if (index == size() - 1)
        return popTail();
    Cell<T> *walker1 = head;
    Cell<T> *walker2 = head;
    for (int i = 0; i < index; i++)
    {
        walker2 = walker1;
        walker1 = walker1->getNextPtr();
    }
    T result = walker1->getContent();
    walker2->setNext(walker1->getNextPtr());
    delete walker1;
    return result;
}

template <class T>
void LinkedList<T>::push(int index, T content)
{
    if (index < 0 || index > size())
        throw std::invalid_argument("list index out of bounds");
    if (index == 0)
        return push(content);
    if (index == size())
        return pushTail(content);
    Cell<T> *newbie = new Cell(content);
    Cell<T> *walker1 = head;
    Cell<T> *walker2 = head;
    for (int i = 0; i < index; i++)
    {
        walker2 = walker1;
        walker1 = walker1->getNextPtr();
    }
    walker2->setNext(newbie);
    newbie->setNext(walker1);
}

template <class T>
bool LinkedList<T>::equals(LinkedList<T> other) const
{
    if (size() != other.size())
        return false;
    for (int i = 0; i < size(); i++)
        if (!get(i).equals(other.get(i)))
            return false;
    return true;
}

template <class T>
LinkedList<T> LinkedList<T>::bubbleSort(std::function<bool(T, T)> comparator)
{
    //first let's copy
    LinkedList<T> * result = new LinkedList<T>();
    for(int i = 0; i < size(); i++)
        result->pushTail(get(i));
    T tmp;
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < size() - 1; i++)
            if (!comparator(result->get(i), result->get(i + 1)))
            {
                tmp = result->get(i);
                result->set(i, result->get(i + 1));
                result->set(i + 1, tmp);
                sorted = false;
            }
    }
    return* result;
}

template <class T>
LinkedList<T> LinkedList<T>::map(std::function <T(T)> mapfunction) const{
    LinkedList<T>* result = new LinkedList<T>();
    for (int i = 0; i < size(); i++)
        result->pushTail(mapfunction(get(i)));
    return *result;
}

template <class T>
LinkedList<T> LinkedList<T>::filter(std::function <bool(T)> filterfunction) const{
    LinkedList<T>* result = new LinkedList<T>();
    for (int i = 0; i < size(); i++)
        if (filterfunction(get(i)))
            result->pushTail(get(i));
    return *result;
}

template <class T>
bool LinkedList<T>::contains(T obj) const{
    for (int i = 0; i < size(); i++)
        if (get(i) == obj)
            return true;
    return false;
}

template <class T>
T LinkedList<T>::reduce(T first, std::function<T(T, T)> reducefunction) const{
    if(size() == 0)
        return first;
    T result = first;
    for (int i = 0; i < size(); i++)
        result = reducefunction(result, get(i));
    return result;
}

template <class T>
T LinkedList<T>::extremum(std::function <bool(T, T)> comparator) const{
    if(size() == 0)
        return T();
    T result = get(0);
    for (int i = 1; i < size(); i++)
        if (comparator(result, get(i)))
            result = get(i);
    return result;
}

template <class T>
LinkedList<T> LinkedList<T>::reverse() const{
    LinkedList<T>* res = new LinkedList<T>();
    for (int i = 0 ; i < size(); i++){
        res->push(get(i));
    }
    return *res;;
}

template <class T>
LinkedList<T> LinkedList<T>::merge(LinkedList<T> other) const{
    LinkedList<T> *result = new LinkedList<T>();
    for (int i = 0; i < size(); i++)
        result->pushTail(get(i));
    for(int i = 0; i < other.size(); i++)
        result->pushTail(other.get(i));
    return *result;
}

template <class T>
bool LinkedList<T>::remove(T obj){
    bool removed = false;
    for (int i = 0; i < size() && !removed; i++)
        if (get(i) == obj){
            removed = true;
            pop(i);
        }
    return removed;
}

template <class T>
bool LinkedList<T>::removeAll(T obj){
    bool removed = false;
    for (int i = size() -1; i >= 0 ;  i--)
        if (get(i) == obj){
            removed = true;
            pop(i);
        }
    return removed;
}
