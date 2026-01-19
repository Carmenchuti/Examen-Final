#ifndef EJERCITO_H
#define EJERCITO_H

#include "Tropa.h"
#include "LinkedList.h"
#include <string>
using namespace std;

class Ejercito {
private:
    string nombreEjercito;
    bool esAliado;
    // En la foto: "DinamicaMemoryList<Tropa*>* tropas"
    LinkedList<Tropa*>* tropas; 

public:
    Ejercito(string nombre, bool aliado) : nombreEjercito(nombre), esAliado(aliado) {
        tropas = new LinkedList<Tropa*>();
    }

    ~Ejercito() {
        tropas->forEach([](Tropa* t) { delete t; });
        delete tropas;
    }

    // Getters y Setters básicos según foto...
    string getNombre() const { return nombreEjercito; }
    LinkedList<Tropa*>* getListaTropas() const { return tropas; }

    void agregarTropa(Tropa* tropa) {
        tropas->pushBack(tropa);
    }

    void eliminarTropa(int index) {
        tropas->removeAt(index);
    }

    // Métodos de conteo recursivo
    int getNumSoldadosTotal() const {
        int total = 0;
        tropas->forEach([&total](Tropa* t) {
            total += t->getNumSoldados();
        });
        return total;
    }

    int getPoderCombateTotal() const {
        int total = 0;
        tropas->forEach([&total](Tropa* t) {
            total += t->getPoderCombateTotal();
        });
        return total;
    }

    int getNumSoldadosVivos() const {
        int total = 0;
        tropas->forEach([&total](Tropa* t) {
            total += t->getNumSoldadosVivos();
        });
        return total;
    }

    void mostrarInfo() const {
        cout << "=== EJERCITO: " << nombreEjercito << " ===" << endl;
        cout << "Poder Total: " << getPoderCombateTotal() << endl;
        tropas->forEach([](Tropa* t) {
            t->mostrarInfo();
        });
    }
};

#endif