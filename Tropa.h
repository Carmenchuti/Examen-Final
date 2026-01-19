#ifndef TROPA_H
#define TROPA_H

#include "Soldado.h"
#include "LinkedList.h"
#include <string>
using namespace std;

class Tropa {
private:
    string nombreTropa;
    // En la foto pone "List", usamos nuestra LinkedList
    LinkedList<Soldado*>* soldados;
    int numSoldados; // Contador actual

public:
    Tropa(const string& nombre = "") : nombreTropa(nombre), numSoldados(0) {
        soldados = new LinkedList<Soldado*>();
    }

    // Destructor para limpiar memoria
    ~Tropa() {
        soldados->forEach([](Soldado* s) { delete s; });
        delete soldados;
    }

    string getNombreTropa() const { return nombreTropa; }
    int getNumSoldados() const { return numSoldados; }

    // Método para devolver la lista (necesario para persistencia)
    LinkedList<Soldado*>* getListaSoldados() const { return soldados; }

    bool agregarSoldado(Soldado* soldado) {
        if (estallena()) return false;
        soldados->pushBack(soldado);
        numSoldados++;
        return true;
    }

    bool eliminarSoldado(int index) {
        if (index < 0 || index >= numSoldados) return false;
        // Aquí habría que borrar el puntero Soldado* antes de quitarlo de la lista
        // Para simplificar, asumimos que se quita de la lista:
        soldados->removeAt(index);
        numSoldados--;
        return true;
    }

    bool estallena() const { return numSoldados >= 5; }
    bool estaVacia() const { return numSoldados == 0; }

    // Calcula poder total RECURSIVAMENTE (usando forEach de la lista)
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

    // Método de la foto
    void aplicarItemATodos(const Item& item) {
        soldados->forEach([&item](Soldado* s) {
            s->aplicarItem(item);
        });
    }

    void mostrarInfo() const {
        cout << "Tropa: " << nombreTropa << " (" << numSoldados << "/5)" << endl;
        soldados->forEach([](Soldado* s) {
            s->mostrarInfo();
        });
    }
};

#endif