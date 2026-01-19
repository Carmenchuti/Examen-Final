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
    LinkedList<Tropa*>* tropas;

public:
    Ejercito(string nombre, bool aliado) : nombreEjercito(nombre), esAliado(aliado) {
        tropas = new LinkedList<Tropa*>();
    }

    ~Ejercito() {
        tropas->forEach([](Tropa* t) { delete t; });
        delete tropas;
    }

    string getNombre() const { return nombreEjercito; }
    LinkedList<Tropa*>* getListaTropas() const { return tropas; }

    void agregarTropa(Tropa* tropa) {
        tropas->pushBack(tropa);
    }

    void eliminarTropa(int index) {
        tropas->removeAt(index);
    }

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

    // EL RECUADRO EXACTO DE LA CAPTURA
    void mostrarInfo() const {
        cout << "\n\n=== TROPAS EXISTENTES ===" << endl;
        cout << "\n________________________________________________________" << endl;
        cout << "||                                                    ||" << endl;
        cout << "||  EJERCITO: " << nombreEjercito << "       ||" << endl;
        cout << "||____________________________________________________||" << endl;
        cout << "||                                                    ||" << endl;
        cout << "||  Bando: " << (esAliado ? "Aliado" : "Enemigo") << "                         ||" << endl;
        cout << "||  Numero de tropas: " << tropas->getSize() << "                               ||" << endl;
        cout << "||  Total de soldados: " << getNumSoldadosTotal() << "                              ||" << endl;
        cout << "||  Soldados vivos: " << getNumSoldadosVivos() << "                                 ||" << endl;
        cout << "||  Poder de combate total: " << getPoderCombateTotal() << "                       ||" << endl;
        cout << "||____________________________________________________||" << endl;

        int i = 0;
        tropas->forEach([&i](Tropa* t) {
            t->mostrarInfo(i++);
        });
    }
};

#endif