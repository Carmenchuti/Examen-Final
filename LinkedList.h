

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <functional> // Para usar lambdas en forEach
using namespace std;

// Nodo genérico
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

    // --- MÉTODOS PRIVADOS RECURSIVOS (El motor de la lista) ---

    /**
     * @brief Inserta un elemento al final buscando recursivamente el último nodo.
     */
    void pushBackRec(Node<T>*& current, T val) {
        // Caso Base: Si estamos en el final, creamos el nodo aquí
        if (current == nullptr) {
            current = new Node<T>(val);
            return;
        }
        // Paso Recursivo: Sigue buscando en el siguiente
        pushBackRec(current->next, val);
    }

    /**
     * @brief Elimina un nodo por índice usando recursividad.
     */
    void removeAtRec(Node<T>*& current, int index, int currentIndex) {
        if (current == nullptr) return; // Caso base: Lista acabada

        if (index == currentIndex) {
            Node<T>* temp = current;
            current = current->next; // Enlazamos el anterior con el siguiente
            delete temp; // Liberamos memoria
            size--;
            return;
        }
        removeAtRec(current->next, index, currentIndex + 1);
    }

    /**
     * @brief Recorre la lista ejecutando una acción en cada elemento.
     * Sustituye a los bucles iterativos tradicionales.
     */
    void forEachRec(Node<T>* current, function<void(T)> action) const {
        if (current == nullptr) return; // Caso base
        action(current->data); // Ejecuta la acción
        forEachRec(current->next, action); // Llamada recursiva
    }

    /**
     * @brief Obtiene un dato por índice recursivamente.
     */
    T getRec(Node<T>* current, int index, int currentIndex) const {
        if (current == nullptr) throw out_of_range("Indice fuera de rango");
        if (index == currentIndex) return current->data;
        return getRec(current->next, index, currentIndex + 1);
    }

    /**
     * @brief Destructor recursivo para evitar memory leaks.
     */
    void clearRec(Node<T>* current) {
        if (current == nullptr) return;
        clearRec(current->next);
        delete current;
    }

public:
    // Constructor: Inicia lista vacía
    LinkedList() : head(nullptr), size(0) {}

    // Destructor: Limpia toda la memoria al cerrar
    ~LinkedList() { clearRec(head); }

    // Fachada pública para insertar
    void pushBack(T val) {
        pushBackRec(head, val);
        size++;
    }

    // Fachada pública para borrar
    void removeAt(int index) {
        if (index < 0 || index >= size) return;
        removeAtRec(head, index, 0);
    }

    // Fachada pública para obtener dato
    T get(int index) const {
        return getRec(head, index, 0);
    }

    // Getters de utilidad
    int getSize() const { return size; }
    bool isEmpty() const { return head == nullptr; }

    /**
     * @brief Método clave para interactuar con los elementos sin usar 'for'.
     * @param action Función lambda a ejecutar sobre cada elemento.
     */
    void forEach(function<void(T)> action) const {
        forEachRec(head, action);
    }
};

#endif