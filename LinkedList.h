//
// Created by Carmen Bolaños Villarejo on 19/1/26.
//

/**
 * @file LinkedList.h
 * @brief Estructura de datos dinámica recursiva.
 * @details Cumple el requisito de NO usar iteraciones (bucles) para recorrer datos.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <functional> // Para pasar funciones lambda en recorridos avanzados
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

    // --- MÉTODOS PRIVADOS RECURSIVOS (El motor del 10) ---

    // Insertar al final recursivamente
    Node<T>* pushBackRec(Node<T>* current, T val) {
        // Caso Base: Llegamos al final (o lista vacía)
        if (current == nullptr) {
            return new Node<T>(val);
        }
        // Paso Recursivo: Sigue buscando el final
        current->next = pushBackRec(current->next, val);
        return current;
    }

    // Mostrar recursivamente
    void printRec(Node<T>* current) const {
        if (current == nullptr) return; // Caso Base
        // Nota: Asumimos que T tiene sobrecarga de << o es puntero con método imprimir
        // Para hacerlo genérico simple, imprimimos dirección o valor
        // En este proyecto, usaremos un método específico 'forEach' para imprimir objetos complejos
        cout << current->data << " -> ";
        printRec(current->next);
    }

    // Borrar nodo recursivamente (por valor/puntero)
    Node<T>* removeRec(Node<T>* current, T val) {
        if (current == nullptr) return nullptr;

        if (current->data == val) {
            Node<T>* temp = current->next;
            delete current; // Liberamos memoria
            size--;
            return temp; // Saltamos el nodo borrado
        }

        current->next = removeRec(current->next, val);
        return current;
    }

    // Recorrido funcional recursivo (Para ejecutar una acción en cada elemento)
    // Esto permite a la Persona 2 y 3 hacer cosas con los soldados sin usar 'for'
    void forEachRec(Node<T>* current, function<void(T)> action) const {
        if (current == nullptr) return;
        action(current->data); // Ejecuta la acción (ej: soldado->atacar())
        forEachRec(current->next, action); // Sigue con el siguiente
    }

    // Obtener elemento por índice recursivamente
    T getRec(Node<T>* current, int index) const {
        if (current == nullptr) throw out_of_range("Indice fuera de rango");
        if (index == 0) return current->data;
        return getRec(current->next, index - 1);
    }

    // Destructor recursivo
    void clearRec(Node<T>* current) {
        if (current == nullptr) return;
        clearRec(current->next);
        delete current;
    }

public:
    /**
     * @brief Constructor por defecto. Inicializa la lista vacía.
     */
    LinkedList() : head(nullptr), size(0) {}

    /**
     * @brief Destructor. Limpia la memoria recursivamente.
     */
    ~LinkedList() {
        clearRec(head);
    }

    /**
     * @brief Agrega un elemento al final de la lista.
     * @param val Elemento a agregar.
     */
    void pushBack(T val) {
        head = pushBackRec(head, val);
        size++;
    }

    /**
     * @brief Elimina la primera aparición del elemento.
     * @param val Elemento a borrar.
     */
    void remove(T val) {
        head = removeRec(head, val);
    }

    /**
     * @brief Ejecuta una función sobre cada elemento de la lista.
     * Útil para imprimir soldados o sumar poder sin usar bucles fuera.
     * @param action Función lambda o puntero a función.
     */
    void forEach(function<void(T)> action) const {
        forEachRec(head, action);
    }

    /**
     * @brief Obtiene un elemento en una posición específica.
     * @param index Índice (0 a size-1).
     * @return El dato T.
     */
    T get(int index) const {
        return getRec(head, index);
    }

    /**
     * @brief Devuelve el tamaño actual de la lista.
     */
    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

#endif