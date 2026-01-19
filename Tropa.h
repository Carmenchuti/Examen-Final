#ifndef TROPA_H
#define TROPA_H

#include "Soldado.h"
#include "LinkedList.h"
#include <string>
using namespace std;

class Tropa {
private:
    string nombreTropa;
    LinkedList<Soldado*>* soldados;
    int numSoldados;

public:
    Tropa(const string& nombre = "") : nombreTropa(nombre), numSoldados(0) {
        soldados = new LinkedList<Soldado*>();
    }

    ~Tropa() {
        soldados->forEach([](Soldado* s) { delete s; });
        delete soldados;
    }

    string getNombreTropa() const { return nombreTropa; }
    int getNumSoldados() const { return numSoldados; }
    LinkedList<Soldado*>* getListaSoldados() const { return soldados; }

    bool agregarSoldado(Soldado* soldado) {
        if (estallena()) return false;
        soldados->pushBack(soldado);
        numSoldados++;
        return true;
    }

    bool eliminarSoldado(int index) {
        if (index < 0 || index >= numSoldados) return false;
        soldados->removeAt(index);
        numSoldados--;
        return true;
    }

    bool estallena() const { return numSoldados >= 5; }
    bool estaVacia() const { return numSoldados == 0; }

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

    void aplicarItemATodos(const Item& item) {
        soldados->forEach([&item](Soldado* s) {
            s->aplicarItem(item);
        });
    }

    // FORMATO IDÉNTICO A LA CAPTURA
    void mostrarInfo(int indexTropa) const {
        // Solo mostramos el índice de tropa si es >= 0 (para ocultarlo si no queremos)
        if (indexTropa >= 0) cout << "\n[Tropa " << indexTropa << "]" << endl;

        cout << "\n=== Tropa: " << nombreTropa << " ===" << endl;
        cout << "Soldados en tropa: " << numSoldados << "/5" << endl;
        cout << "Poder de combate total: " << getPoderCombateTotal() << endl;

        // Lambda recursiva con captura de índice para pintar [0], [1]...
        int i = 0;
        soldados->forEach([&i](Soldado* s) {
            s->mostrarInfo(i++);
        });
    }
};

#endif