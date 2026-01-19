#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// Colores
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

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
        ejercito_guardado = new Ejercito("Ejercito de " + nombre + " V de Gisberia", true);
        inventario = new LinkedList<Item*>();

        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Tacticas", BOOST_EXPERIENCIA, 200));
    }

    // --- GESTIÓN DE TROPAS ---
    void crearNuevaTropa() {
        cout << YELLOW << "\n=== CREAR NUEVA TROPA ===" << RESET << endl;
        string nombreTropa;
        cout << "Nombre de la tropa: ";
        cin >> nombreTropa;
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << GREEN << "Tropa creada exitosamente." << RESET << endl;
    }

    void modificarTropas() {
        char op;
        do {
            cout << YELLOW << "\n=== MODIFICAR TROPAS ===" << RESET << endl;
            if (ejercito_guardado->getListaTropas()->isEmpty()) {
                cout << "No hay tropas." << endl;
            } else {
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

    void entrenarSoldados() {
        if (ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "No hay tropas para entrenar." << endl;
            return;
        }
        cout << YELLOW << "\n=== ENTRENAMIENTO ===" << RESET << endl;
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
        tropa->getListaSoldados()->forEach([xp](Soldado* s){ s->ganarExperiencia(xp); });
    }

    void verSoldadosDisponibles() {
        cout << CYAN << "\n=== SOLDADOS EN RESERVA ===" << RESET << endl;
        if (soldados->isEmpty()) cout << "Reserva vacia." << endl;
        else {
            int i = 0;
            soldados->forEach([&i](Soldado* s) { s->mostrarInfo(i++); });
        }
    }

    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    void inventarioComandante() {
        cout << YELLOW << "\n=== MOCHILA ===" << RESET << endl;
        if (inventario->isEmpty()) { cout << "Vacia." << endl; return; }

        int i = 0;
        inventario->forEach([&i](Item* it) { cout << "[" << i++ << "] "; it->mostrarInfo(); });

        cout << "Usar item nº (-1 salir): ";
        int op; cin >> op;
        if (op >= 0 && op < inventario->getSize()) {
            if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                ejercito_guardado->getListaTropas()->get(0)->aplicarItemATodos(*inventario->get(op));
                inventario->removeAt(op);
                cout << GREEN << "Item usado en la primera tropa." << RESET << endl;
            } else {
                cout << RED << "No tienes tropas." << RESET << endl;
            }
        }
    }

    // --- HOGUERA DE BATALLA ---
    void hogueraDeBatalla() {
        cout << RED << "\n======= HOGUERA DE BATALLA =======" << RESET << endl;
        cout << "1. Celebrar Festin (Evento Aleatorio)" << endl;
        cout << "2. Otear el horizonte (Combatir)" << endl;
        cout << "3. Volver" << endl;
        cout << "Elige tu destino: ";
        int op; cin >> op;

        // OPCIÓN 1: FESTÍN RANDOM
        if (op == 1) {
            cout << "\nPreparando la hoguera..." << endl;
            int suerte = rand() % 4;

            if (suerte == 0) {
                cout << MAGENTA << "¡INCREIBLE! El Rey Enolas ha enviado un bardo famoso." << RESET << endl;
                cout << GREEN << "[BONUS] +300 EXP para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                    t->aplicarItemATodos(Item("Epica", BOOST_EXPERIENCIA, 300));
                });
            }
            else if (suerte == 1) {
                cout << YELLOW << "Huele a carne asada. Una cena copiosa." << RESET << endl;
                cout << GREEN << "[BONUS] +100 EXP para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                    t->aplicarItemATodos(Item("Carne Asada", BOOST_EXPERIENCIA, 100));
                });
            }
            else if (suerte == 2) {
                cout << CYAN << "¡BUM! ¡BUM! Los tambores de guerra resuenan." << RESET << endl;
                cout << GREEN << "[BONUS] +10 ATAQUE permanente para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                    t->aplicarItemATodos(Item("Furia", BOOST_ATAQUE, 10));
                });
            }
            else {
                cout << RED << "¡PUAJ! La comida estaba en mal estado..." << RESET << endl;
                cout << RED << "[PENALIZACION] -5 VIDA a todos por indigestión." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                    t->aplicarItemATodos(Item("Indigestion", BOOST_VIDA, -5));
                });
            }
        }

        // OPCIÓN 2: COMBATE
        else if (op == 2) {
            cout << "\n\033[1;35m--- INFORME DE EXPLORADORES ---\033[0m" << endl;

            string nombres[] = {"Orcos de Mordor", "Bandidos", "Esqueletos", "Mercenarios", "Goblins"};
            string nombreEnemigo = nombres[rand() % 5];
            int poderEnemigo = (rand() % 151) + 100; // 100 - 250

            cout << "¡Se aproxima una tropa enemiga!" << endl;
            cout << " - Enemigo: " << nombreEnemigo << " (Poder estimado: " << poderEnemigo << ")" << endl;

            int miPoder = ejercito_guardado->getPoderCombateTotal();
            cout << "\nTU PODER: " << miPoder << " vs ENEMIGO: " << poderEnemigo << endl;

            if (miPoder >= poderEnemigo) {
                cout << GREEN << "\n¡VICTORIA GLORIOSA!" << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                     t->aplicarItemATodos(Item("Victoria", BOOST_EXPERIENCIA, 200));
                });
            } else {
                cout << RED << "\n¡DERROTA! Nos están masacrando..." << RESET << endl;

                // FASE DE BAJAS: PRIMERO MUERTE
                if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);

                    if (!t->estaVacia()) {
                        cout << RED << " >> " << t->getListaSoldados()->get(0)->getNombre() << " ha caido en combate (MUERTO)." << RESET << endl;
                        t->eliminarSoldado(0); // Hueco libre
                    }
                    // Daño al resto
                    t->getListaSoldados()->forEach([](Soldado* s){
                        s->recibirDanio(40);
                    });
                    cout << RED << " >> El resto de la tropa ha recibido daño critico." << RESET << endl;
                }

                // FASE DE REFUERZOS
                cout << YELLOW << "\n[SISTEMA] ¡Huecos disponibles! Llamando a la reserva..." << RESET << endl;

                int poderRefuerzos = 0;
                if (!ejercito_guardado->getListaTropas()->isEmpty() && !soldados->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);

                    while (!t->estallena() && !soldados->isEmpty()) {
                        Soldado* ref = soldados->get(0);
                        poderRefuerzos += ref->getPoderCombate();
                        t->agregarSoldado(ref);
                        soldados->removeAt(0);
                        cout << CYAN << " >> " << ref->getNombre() << " llega corriendo desde la reserva!" << RESET << endl;
                    }
                }

                if (poderRefuerzos > 0) {
                    cout << CYAN << "\n[CONTRAATAQUE] ¡Tropas reorganizadas!" << RESET << endl;
                    cout << "Poder Extra Sumado: " << poderRefuerzos << endl;

                    int nuevoPoderTotal = ejercito_guardado->getPoderCombateTotal();
                    cout << "NUEVO PODER TOTAL: " << nuevoPoderTotal << " vs ENEMIGO: " << poderEnemigo << endl;

                    if (nuevoPoderTotal >= poderEnemigo) {
                        cout << GREEN << "\n¡VICTORIA EPICA! Los refuerzos han vengado a los caidos." << RESET << endl;
                        ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                            t->aplicarItemATodos(Item("Victoria Epica", BOOST_EXPERIENCIA, 300));
                        });
                    } else {
                        cout << RED << "\n¡DERROTA FINAL! Retirada..." << RESET << endl;
                    }
                } else {
                    cout << RED << "¡No quedan reservas o la tropa estaba llena!" << RESET << endl;
                }
            }
        }

        // --- PAUSA SIMPLIFICADA (SOLO ENTER) ---
        cout << "\nPresiona Enter para continuar...";
        cin.ignore();
        cin.get();
    }

    void guardarPartida() {
        if (!fs::exists("json")) fs::create_directory("json");
        string ruta = "json/" + nombre + "_partida.json";
        ofstream f(ruta);
        if (f.is_open()) {
            f << "{\n  \"jugador\": \"" << nombre << "\",\n  \"tropas\": " << ejercito_guardado->getListaTropas()->getSize() << "\n}\n";
            f.close();
            cout << GREEN << "\n[SISTEMA] Guardado en: " << ruta << RESET << endl;
        }
    }

    void reclutarSoldado(string n, int niv, int pod, int exp) {
        soldados->pushBack(new Soldado(n, true, niv, pod, exp));
    }
    Ejercito* getEjercito() { return ejercito_guardado; }
};

#endif