#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem> // Necesario para crear carpetas

using namespace std;
namespace fs = std::filesystem; // Atajo para no escribir std::filesystem todo el rato

// Colores
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

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

   // --- COMBATE ---
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
            cout << "\n\033[1;35m--- INFORME DE EXPLORADORES ---\033[0m" << endl;

            // CAMBIO: Ahora siempre es 1 tropa, pero aleatoria
            int numTropasEnemigas = 1;
            int poderTotalEnemigo = 0;

            // Nombres aleatorios
            string nombres[] = {"Orcos de Mordor", "Bandidos del Desierto", "Esqueletos", "Mercenarios", "Goblins"};
            string nombreEnemigo = nombres[rand() % 5];

            // Poder aleatorio entre 100 y 250 (Un solo enemigo fuerte)
            int poderEnemigo = (rand() % 151) + 100;
            poderTotalEnemigo = poderEnemigo;

            cout << "¡Se aproxima una tropa enemiga!" << endl;
            cout << " - Enemigo: " << nombreEnemigo << " (Poder estimado: " << poderEnemigo << ")" << endl;

            cout << "\n-----------------------------------" << endl;
            int miPoder = ejercito_guardado->getPoderCombateTotal();
            cout << "TU PODER TOTAL: " << miPoder << endl;
            cout << "PODER ENEMIGO:  " << poderTotalEnemigo << endl;
            cout << "-----------------------------------" << endl;

            if (miPoder >= poderTotalEnemigo) {
                cout << GREEN << "\n¡VICTORIA GLORIOSA!" << RESET << endl;
                cout << "El enemigo ha sido aniquilado." << endl;
                // Premio
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                     t->aplicarItemATodos(Item("Victoria", BOOST_EXPERIENCIA, 200));
                });
            } else {
                cout << RED << "\n¡DERROTA! Has sido superado." << RESET << endl;
                cout << "Tus tropas sufren bajas..." << endl;

                // Refuerzos de la reserva (Rúbrica)
                cout << YELLOW << "\n[SISTEMA] Solicitando refuerzos de la reserva..." << RESET << endl;

                if (!ejercito_guardado->getListaTropas()->isEmpty() && !soldados->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);
                    int cont = 0;
                    while (!t->estallena() && !soldados->isEmpty()) {
                        Soldado* ref = soldados->get(0);
                        t->agregarSoldado(ref);
                        soldados->removeAt(0);
                        cout << " >> Refuerzo " << ref->getNombre() << " se ha unido." << endl;
                        cont++;
                    }
                    if(cont==0) cout << "No caben mas soldados en la tropa." << endl;
                } else {
                    cout << RED << "No hay refuerzos disponibles." << RESET << endl;
                }
            }
        }
        cout << "\n(Presiona una tecla y Enter para continuar)";
        string dummy; cin >> dummy;
    }

    // --- GUARDA DENTRO DE LA CARPETA JSON ---
    void guardarPartida() {
        // 1. Crear carpeta si no existe
        if (!fs::exists("json")) {
            fs::create_directory("json");
        }

        // 2. Guardar archivo dentro
        string rutaArchivo = "json/" + nombre + "_partida.json";
        ofstream f(rutaArchivo);

        if (f.is_open()) {
            f << "{\n";
            f << "  \"jugador\": \"" << nombre << "\",\n";
            f << "  \"tropas_activas\": " << ejercito_guardado->getListaTropas()->getSize() << "\n";
            f << "}\n";
            f.close();

            cout << GREEN << "\n[SISTEMA] Partida guardada en carpeta 'json': " << nombre << "_partida.json" << RESET << endl;
        } else {
            cout << RED << "[ERROR] No se pudo crear el archivo." << RESET << endl;
        }
    }

    void reclutarSoldado(string n, int niv, int pod, int exp) {
        soldados->pushBack(new Soldado(n, true, niv, pod, exp));
    }
    Ejercito* getEjercito() { return ejercito_guardado; }
};

#endif