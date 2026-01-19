#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream> // Para guardar fichero

using namespace std;

// Colores básicos para que quede bonito como en la demo
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

class Jugador {
private:
    string nombre;
    string passwd;

    // ESTRUCTURAS (Punteros para memoria dinámica)
    LinkedList<Soldado*>* soldados;       // Reserva de soldados
    Ejercito* ejercito_guardado;          // Ejército activo
    LinkedList<Item*>* inventario;        // Mochila

public:
    Jugador(string nombre, string pass) : nombre(nombre), passwd(pass) {
        soldados = new LinkedList<Soldado*>();
        ejercito_guardado = new Ejercito("Ejercito de " + nombre + " V de Gisberia", true);
        inventario = new LinkedList<Item*>();

        // Items iniciales (Cumple Rúbrica Ejercicio 2)
        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Tacticas", BOOST_EXPERIENCIA, 200));
    }

    // --- OPCION 1: CREAR TROPA ---
    void crearNuevaTropa() {
        cout << YELLOW << "\n=== CREAR NUEVA TROPA ===" << RESET << endl;
        string nombreTropa;
        cout << "Nombre de la tropa: ";
        cin >> nombreTropa;
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << GREEN << "Tropa creada exitosamente." << RESET << endl;
    }

    // --- OPCION 2: MODIFICAR TROPAS (Como en la foto) ---
    void modificarTropas() {
        char op;
        do {
            cout << YELLOW << "\n=== MODIFICAR TROPAS ===" << RESET << endl;
            // Mostramos resumen rápido
            if (ejercito_guardado->getListaTropas()->isEmpty()) {
                cout << "No hay tropas." << endl;
            } else {
                // Mostramos la primera como ejemplo
                ejercito_guardado->getListaTropas()->get(0)->mostrarInfo(0);
            }

            cout << "\n[E] Eliminar primera tropa" << endl;
            cout << "[V] Volver" << endl;
            cout << "Opcion: ";
            cin >> op;
            op = toupper(op);

            if (op == 'E' && !ejercito_guardado->getListaTropas()->isEmpty()) {
                ejercito_guardado->eliminarTropa(0);
                cout << RED << "Tropa eliminada." << RESET << endl;
            }
        } while (op != 'V');
    }

    // --- OPCION 3: ENTRENAR (Submenú XP) ---
    void entrenarSoldados() {
        if (ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "No hay tropas para entrenar." << endl;
            return;
        }

        cout << YELLOW << "\n=== ENTRENAMIENTO ===" << RESET << endl;
        // Listar tropas
        int i = 0;
        ejercito_guardado->getListaTropas()->forEach([&i](Tropa* t) {
            cout << "[" << (i + 1) << "] " << t->getNombreTropa() << endl;
            i++;
        });

        cout << "Selecciona tropa (0 salir): ";
        int idx; cin >> idx;
        if (idx <= 0) return;

        Tropa* tropa = ejercito_guardado->getListaTropas()->get(idx - 1);

        cout << CYAN << "\n--- TIPO DE ENTRENAMIENTO ---" << RESET << endl;
        cout << "[1] Basico (+50 XP)\n[2] Medio (+150 XP)\n[3] Avanzado (+300 XP)\n[4] Intensivo (+500 XP)" << endl;
        int tipo; cin >> tipo;

        int xp = (tipo == 1) ? 50 : (tipo == 2) ? 150 : (tipo == 3) ? 300 : 500;

        cout << GREEN << "Entrenando..." << RESET << endl;
        tropa->getListaSoldados()->forEach([xp](Soldado* s){
            s->ganarExperiencia(xp);
        });
    }

    // --- OPCION 4: VER RESERVA ---
    void verSoldadosDisponibles() {
        cout << CYAN << "\n=== SOLDADOS EN RESERVA ===" << RESET << endl;
        if (soldados->isEmpty()) cout << "Reserva vacia." << endl;
        else {
            int i = 0;
            soldados->forEach([&i](Soldado* s) { s->mostrarInfo(i++); });
        }
    }

    // --- OPCION 5: VER TODO ---
    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    // --- RÚBRICA EJERCICIO 2: INVENTARIO ---
    void inventarioComandante() {
        cout << YELLOW << "\n=== MOCHILA ===" << RESET << endl;
        if (inventario->isEmpty()) { cout << "Vacia." << endl; return; }

        int i = 0;
        inventario->forEach([&i](Item* it) { cout << "[" << i++ << "] "; it->mostrarInfo(); });

        cout << "Usar item nº (-1 salir): ";
        int op; cin >> op;
        if (op >= 0 && op < inventario->getSize()) {
            if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                // Aplicamos a la primera tropa (simplificado para demo)
                ejercito_guardado->getListaTropas()->get(0)->aplicarItemATodos(*inventario->get(op));
                inventario->removeAt(op);
                cout << GREEN << "Item usado en la primera tropa." << RESET << endl;
            } else {
                cout << RED << "No tienes tropas." << RESET << endl;
            }
        }
    }

    // --- RÚBRICA EJERCICIO 3: COMBATE Y REFUERZOS ---
    void hogueraDeBatalla() {
        cout << RED << "\n=== HOGUERA DE BATALLA ===" << RESET << endl;
        cout << "1. Festin (+XP)\n2. Combatir\n3. Salir\n> ";
        int op; cin >> op;

        if (op == 1) {
            cout << GREEN << "Tus tropas comen y ganan experiencia." << RESET << endl;
            ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                t->aplicarItemATodos(Item("Festin", BOOST_EXPERIENCIA, 100));
            });
        }
        else if (op == 2) {
            cout << "\nUn ejercito enemigo aparece..." << endl;
            int miPoder = ejercito_guardado->getPoderCombateTotal();
            int enemigoPoder = 190; // Valor fijo demo

            cout << "Tu Poder: " << miPoder << " vs Enemigo: " << enemigoPoder << endl;

            if (miPoder >= enemigoPoder) {
                cout << GREEN << "¡VICTORIA! El enemigo huye." << RESET << endl;
            } else {
                cout << RED << "¡DERROTA! Tus tropas sufren bajas." << RESET << endl;

                // --- RÚBRICA: REFUERZOS AUTOMÁTICOS ---
                cout << YELLOW << "Solicitando refuerzos de la reserva..." << RESET << endl;
                if (!ejercito_guardado->getListaTropas()->isEmpty() && !soldados->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);
                    while (!t->estallena() && !soldados->isEmpty()) {
                        Soldado* ref = soldados->get(0);
                        t->agregarSoldado(ref);
                        soldados->removeAt(0);
                        cout << "Refuerzo " << ref->getNombre() << " unido." << endl;
                    }
                } else {
                    cout << "No quedan refuerzos." << endl;
                }
            }
        }
    }

    // --- GUARDAR PARTIDA (JSON FALSO) ---
    void guardarPartida() {
        ofstream f(nombre + "_partida.json");
        if (f.is_open()) {
            f << "{\n \"jugador\": \"" << nombre << "\",\n \"tropas\": "
              << ejercito_guardado->getListaTropas()->getSize() << "\n}";
            f.close();
            cout << GREEN << "Guardado en " << nombre << "_partida.json" << RESET << endl;
        }
    }

    // Helper
    void reclutarSoldado(string n, int niv, int pod, int exp) {
        soldados->pushBack(new Soldado(n, true, niv, pod, exp));
    }

    Ejercito* getEjercito() { return ejercito_guardado; }
};

#endif