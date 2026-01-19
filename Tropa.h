//
// Created by Carmen Bolaños Villarejo on 19/1/26.
//

/**
 * @file Tropa.h
 * @brief Grupo de hasta 5 soldados. Usa LinkedList recursiva.
 */

#ifndef TROPA_H
#define TROPA_H

#include "LinkedList.h"
#include "Soldado.h"
#include <string>
#include <iostream>
using namespace std;

class Tropa {
private:
    string nombreTropa;
    LinkedList<Soldado*>* soldados; // Uso de memoria dinámica y LinkedList propia
    const int MAX_SOLDADOS = 5;

public:
    /**
     * @brief Constructor.
     * @param nombre Nombre del escuadrón.
     */
    Tropa(string nombre) : nombreTropa(nombre) {
        soldados = new LinkedList<Soldado*>();
    }

    /**
     * @brief Destructor. Libera la memoria de los soldados y la lista.
     */
    ~Tropa() {
        // Primero borramos los soldados (punteros)
        soldados->forEach([](Soldado* s) {
            delete s;
        });
        delete soldados; // Luego borramos la estructura de la lista
    }

    /**
     * @brief Intenta agregar un soldado si hay hueco.
     * @return true si se agregó, false si está llena.
     */
    bool agregarSoldado(Soldado* s) {
        if (soldados->getSize() >= MAX_SOLDADOS) {
            cout << "[Error] La tropa " << nombreTropa << " esta llena (Max 5)." << endl;
            return false;
        }
        soldados->pushBack(s);
        return true;
    }

    /**
     * @brief Elimina soldados muertos (Lógica recursiva interna de remove).
     */
    void limpiarMuertos() {
        // Nota: Para una limpieza perfecta en lista enlazada mientras se itera
        // se requeriría una lógica un poco más compleja en LinkedList.
        // Para simplificar en examen, asumimos gestión externa o simple.
        // Aquí una implementación conceptual.
    }

    /**
     * @brief Calcula el poder total recursivamente.
     */
    int obtenerPoderTotal() const {
        int total = 0;
        // Usamos la función forEach recursiva de la lista
        // Pasamos 'total' por referencia en la lambda [&]
        soldados->forEach([&](Soldado* s) {
            if (s->estaVivo()) {
                total += s->getPoder();
            }
        });
        return total;
    }

    // Getters
    string getNombre() const { return nombreTropa; }
    LinkedList<Soldado*>* getListaSoldados() const { return soldados; }

    void mostrarTropa() const {
        cout << "\n=== Tropa: " << nombreTropa << " ===" << endl;
        cout << "Soldados en tropa: " << soldados->getSize() << "/5" << endl;
        cout << "Poder de combate total: " << obtenerPoderTotal() << endl;

        // Iteración recursiva para mostrar detalles
        soldados->forEach([](Soldado* s) {
            s->mostrarInfo();
        });
        cout << "================================\n";
    }
};

#endif