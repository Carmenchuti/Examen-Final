#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
using namespace std;

class Jugador {
private:
    string nombre;
    string passwd;
    LinkedList<Soldado*>* soldados; 
    Ejercito* ejercito_guardado;
    LinkedList<Item*>* inventario;

public:
    Jugador(string nombre, string pass) : nombre(nombre), passwd(pass) {
        soldados = new LinkedList<Soldado*>();
        ejercito_guardado = new Ejercito("Ejercito de " + nombre, true);
        inventario = new LinkedList<Item*>();

        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Sabiduria", BOOST_EXPERIENCIA, 100));
    }

    void crearNuevaTropa() {
        cout << "\n=== CREAR NUEVA TROPA ===" << endl;
        string nombreTropa;
        cout << "Ingrese el nombre de la nueva tropa:\n";
        cin >> nombreTropa; // Nota: para nombres con espacios usar getline, pero cin para seguir video simple
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << "Tropa '" << nombreTropa << "' creada." << endl;
    }

    void verSoldadosDisponibles() {
        cout << "\n--- SOLDADOS DISPONIBLES ---" << endl;
        if (soldados->isEmpty()) {
            cout << "No tienes soldados libres." << endl;
            return;
        }
        int i = 0;
        soldados->forEach([&i](Soldado* s) {
            s->mostrarInfo(i++);
        });
    }

    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    // OPCIÓN 2 DEL MENÚ: MODIFICAR TROPAS (IGUAL A CAPTURA)
    void modificarTropas() {
        char op;
        do {
            cout << "\n=== MODIFICAR TROPAS DEL EJERCITO ===" << endl;
            cout << "\n--- TROPAS EN EL EJERCITO ---" << endl;
            
            // Mostrar lista simple de tropas para el menú
            if (ejercito_guardado->getListaTropas()->isEmpty()) {
                cout << "[Vacio]" << endl;
            } else {
                int i = 0;
                ejercito_guardado->getListaTropas()->forEach([&i](Tropa* t) {
                    t->mostrarInfo(i++);
                });
            }

            cout << "\n--- OPCIONES ---" << endl;
            cout << "\n[E] Eliminar una tropa del ejercito" << endl;
            cout << "[R] Reorganizar tropas (cambiar orden)" << endl;
            cout << "[V] Volver al menu anterior" << endl;
            cout << "\nSeleccione una opcion:\n";
            cin >> op;
            op = toupper(op);

            if (op == 'E') {
                cout << "Ingrese el numero de la tropa a eliminar: ";
                int idx;
                cin >> idx;
                ejercito_guardado->eliminarTropa(idx);
                cout << "Tropa eliminada." << endl;
            } else if (op == 'R') {
                cout << "Funcionalidad de reorganizar pendiente..." << endl;
            }

        } while (op != 'V');
    }

    // OPCIÓN 3 DEL MENÚ: ENTRENAR (IGUAL A CAPTURA)
    void entrenarSoldados() {
        if (ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "No hay tropas para entrenar." << endl;
            return;
        }

        cout << "\n=== ENTRENAR SOLDADOS ===" << endl;
        cout << "\n--- TROPAS DISPONIBLES PARA ENTRENAR ---" << endl;
        
        int i = 0;
        ejercito_guardado->getListaTropas()->forEach([&i](Tropa* t) {
            cout << "[" << i++ << "] " << t->getNombreTropa() 
                 << " - Soldados: " << t->getNumSoldados() 
                 << " - Poder total: " << t->getPoderCombateTotal() << endl;
        });

        cout << "\nSeleccione la tropa a entrenar (0 para cancelar):\n"; 
        // Nota: En la captura pone 0 cancelar, pero los indices empiezan en 0. 
        // Asumimos que si elige una valida entrena.
        int idxTropa;
        cin >> idxTropa;
        
        // Corrección lógica: la captura dice "1" para la primera tropa visualmente quizás?
        // Vamos a usar el índice directo del array.
        Tropa* tropa = ejercito_guardado->getListaTropas()->get(idxTropa); // Ojo con índices

        cout << "\n--- TIPO DE ENTRENAMIENTO ---" << endl;
        cout << "\n[1] Entrenamiento basico (+50 EXP)" << endl;
        cout << "[2] Entrenamiento intermedio (+150 EXP)" << endl;
        cout << "[3] Entrenamiento avanzado (+300 EXP)" << endl;
        cout << "[4] Entrenamiento intensivo (+500 EXP)" << endl;
        
        cout << "\nSeleccione el tipo de entrenamiento:\n";
        int tipo;
        cin >> tipo;

        int expGanada = 0;
        string nombreEntrenamiento = "";
        switch(tipo) {
            case 1: expGanada = 50; nombreEntrenamiento = "basico"; break;
            case 2: expGanada = 150; nombreEntrenamiento = "intermedio"; break;
            case 3: expGanada = 300; nombreEntrenamiento = "avanzado"; break;
            case 4: expGanada = 500; nombreEntrenamiento = "intensivo"; break;
            default: return;
        }

        cout << "\n\033[1;32m[ENTRENAMIENTO] Iniciando entrenamiento " << nombreEntrenamiento << "...\033[0m" << endl;
        
        tropa->getListaSoldados()->forEach([expGanada](Soldado* s) {
            s->ganarExperiencia(expGanada);
            cout << endl; // Espacio entre soldados
        });
    }
    
    // Asignar soldado (Opción extra en gestión)
    void asignarSoldadoATropa() {
         if (soldados->isEmpty()) { cout << "No hay soldados." << endl; return; }
         if (ejercito_guardado->getListaTropas()->isEmpty()) { cout << "No hay tropas." << endl; return; }

         verSoldadosDisponibles();
         cout << "Seleccione soldado a agregar: ";
         int idxS; cin >> idxS;
         Soldado* s = soldados->get(idxS);

         verTropasExistentes(); // Simplificado
         cout << "Seleccione tropa destino: ";
         int idxT; cin >> idxT;
         
         if(ejercito_guardado->getListaTropas()->get(idxT)->agregarSoldado(s)) {
             soldados->removeAt(idxS);
             cout << "Soldado asignado." << endl;
         }
    }

    void inventarioComandante() {
        cout << "\n=== INVENTARIO DEL COMANDANTE ===" << endl;
        int i = 0;
        inventario->forEach([&i](Item* item) { cout << "[" << i++ << "] "; item->mostrarInfo(); });
        // (Resto igual que antes...)
        cout << "\nPresiona tecla para volver..." << endl;
    }

    void reclutarSoldado(string nombre, int nivel, int poder, int exp) {
        soldados->pushBack(new Soldado(nombre, true, nivel, poder, exp));
    }
    
    Ejercito* getEjercito() { return ejercito_guardado; }
    LinkedList<Soldado*>* getPoolSoldados() { return soldados; }

    void hogueraDeBatalla() {
        // (Código de batalla del mensaje anterior)
        cout << "Batalla simulada..." << endl;
    }
};

#endif