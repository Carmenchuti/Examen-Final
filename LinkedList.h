#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <functional>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;

    void pushBackRec(Node<T>*& current, T val) {
        if (current == nullptr) {
            current = new Node<T>(val);
            return;
        }
        pushBackRec(current->next, val);
    }

    void removeAtRec(Node<T>*& current, int index, int currentIndex) {
        if (current == nullptr) return;
        if (index == currentIndex) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
            size--;
            return;
        }
        removeAtRec(current->next, index, currentIndex + 1);
    }

    void forEachRec(Node<T>* current, function<void(T)> action) const {
        if (current == nullptr) return;
        action(current->data);
        forEachRec(current->next, action);
    }

    T getRec(Node<T>* current, int index, int currentIndex) const {
        if (current == nullptr) throw out_of_range("Indice fuera de rango");
        if (index == currentIndex) return current->data;
        return getRec(current->next, index, currentIndex + 1);
    }

    void clearRec(Node<T>* current) {
        if (current == nullptr) return;
        clearRec(current->next);
        delete current;
    }

public:
    LinkedList() : head(nullptr), size(0) {}
    ~LinkedList() { clearRec(head); }

    void pushBack(T val) {
        pushBackRec(head, val);
        size++;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) return;
        removeAtRec(head, index, 0);
    }

    T get(int index) const {
        return getRec(head, index, 0);
    }

    int getSize() const { return size; }
    bool isEmpty() const { return head == nullptr; }

    void forEach(function<void(T)> action) const {
        forEachRec(head, action);
    }
};

#endif