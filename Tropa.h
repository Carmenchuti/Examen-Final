
#ifndef TROPA_H
#define TROPA_H

#include "Soldado.h"
#include "LinkedList.h"
#include <string>
using namespace std;

class Tropa {
private:
    string nombreTropa;
    LinkedList<Soldado*>* soldados; // Lista dinámica
    int numSoldados;
    const int MAX_SOLDADOS = 5;

public:
    Tropa(const string& nombre = "") : nombreTropa(nombre), numSoldados(0) {
        soldados = new LinkedList<Soldado*>();
    }

    // Destructor: Limpia memoria de soldados y de la lista
    ~Tropa() {
        soldados->forEach([](Soldado* s) { delete s; });
        delete soldados;
    }

    // Getters
    string getNombreTropa() const { return nombreTropa; }
    int getNumSoldados() const { return numSoldados; }
    LinkedList<Soldado*>* getListaSoldados() const { return soldados; }

    /**
     * @brief Intenta añadir un soldado si hay hueco.
     */
    bool agregarSoldado(Soldado* soldado) {
        if (estallena()) return false;
        soldados->pushBack(soldado);
        numSoldados++;
        return true;
    }

    /**
     * @brief Elimina un soldado de la tropa (por muerte o despido).
     */
    bool eliminarSoldado(int index) {
        if (index < 0 || index >= numSoldados) return false;
        soldados->removeAt(index);
        numSoldados--;
        return true;
    }

    bool estallena() const { return numSoldados >= MAX_SOLDADOS; }
    bool estaVacia() const { return numSoldados == 0; }

    /**
     * @brief Calcula el poder total recursivamente usando forEach.
     */
    int getPoderCombateTotal() const {
        int total = 0;
        soldados->forEach([&total](Soldado* s) {
            if (s->estaVivo()) total += s->getPoderCombate();
        });
        return total;
    }

    int getNumSoldadosVivos() const {
        int vivos = 0;
        soldados->forEach([&vivos](Soldado* s) {
            if (s->estaVivo()) vivos++;
        });
        return vivos;
    }

    /**
     * @brief Aplica un item a TODOS los soldados de la tropa.
     */
    void aplicarItemATodos(const Item& item) {
        soldados->forEach([&item](Soldado* s) {
            s->aplicarItem(item);
        });
    }

    // Muestra la info con el formato visual de recuadro
    void mostrarInfo(int indexTropa) const {
        if (indexTropa >= 0) cout << "\n[Tropa " << indexTropa << "]" << endl;

        cout << "\n=== Tropa: " << nombreTropa << " ===" << endl;
        cout << "Soldados en tropa: " << numSoldados << "/5" << endl;
        cout << "Poder de combate total: " << getPoderCombateTotal() << endl;

        int i = 0;
        soldados->forEach([&i](Soldado* s) {
            s->mostrarInfo(i++);
        });
    }
};

#endif