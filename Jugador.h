
#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream>      // Manejo de archivos
#include <filesystem>   // Manejo de carpetas
#include <vector>
#include <algorithm>    // Para reemplazar espacios

using namespace std;
namespace fs = std::filesystem;

// --- COLORES ---
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

    LinkedList<Soldado*>* soldados;       // Reserva
    Ejercito* ejercito_guardado;          // Ejército activo
    LinkedList<Item*>* inventario;        // Mochila

    // Helper: Sustituye espacios por guiones bajos para guardar sin romper el >>
    string codificar(string texto) {
        replace(texto.begin(), texto.end(), ' ', '_');
        return texto;
    }

    // Helper: Recupera los espacios al cargar
    string decodificar(string texto) {
        replace(texto.begin(), texto.end(), '_', ' ');
        return texto;
    }

public:
    Jugador(string nombre, string pass) : nombre(nombre), passwd(pass) {
        soldados = new LinkedList<Soldado*>();
        ejercito_guardado = new Ejercito("Ejercito de " + nombre + " V de Gisberia", true);
        inventario = new LinkedList<Item*>();

        // Items iniciales
        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Tacticas", BOOST_EXPERIENCIA, 200));
    }

    // --- GESTIÓN (Igual que antes) ---
    void crearNuevaTropa() {
        cout << YELLOW << "\n=== CREAR NUEVA TROPA ===" << RESET << endl;
        string nombreTropa;
        cout << "Nombre de la tropa (usa_guiones_si_hay_espacios): ";
        cin >> nombreTropa;
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << GREEN << "Tropa creada." << RESET << endl;
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
            cout << "\n[E] Eliminar primera tropa\n[V] Volver\n> ";
            cin >> op; op = toupper(op);
            if (op == 'E' && !ejercito_guardado->getListaTropas()->isEmpty()) {
                ejercito_guardado->eliminarTropa(0);
                cout << RED << "Tropa eliminada." << RESET << endl;
            }
        } while (op != 'V');
    }

    void entrenarSoldados() {
        if (ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "Crea tropas primero." << endl; return;
        }
        cout << YELLOW << "\n=== ENTRENAMIENTO ===" << RESET << endl;
        int i = 0;
        ejercito_guardado->getListaTropas()->forEach([&i](Tropa* t) {
            cout << "[" << (i + 1) << "] " << t->getNombreTropa() << endl; i++;
        });
        cout << "Elige tropa (0 salir): ";
        int idx; cin >> idx;
        if (idx <= 0) return;

        cout << "[1] Basico\n[2] Medio\n[3] Avanzado\n[4] Intensivo\n> ";
        int tipo; cin >> tipo;
        int xp = (tipo==1)?50 : (tipo==2)?150 : (tipo==3)?300 : 500;

        cout << GREEN << "Entrenando..." << RESET << endl;
        ejercito_guardado->getListaTropas()->get(idx-1)->getListaSoldados()->forEach([xp](Soldado* s){
            s->ganarExperiencia(xp);
        });
    }

    void verSoldadosDisponibles() {
        cout << CYAN << "\n=== RESERVA ===" << RESET << endl;
        if(soldados->isEmpty()) cout << "Vacia." << endl;
        else { int i=0; soldados->forEach([&i](Soldado* s){ s->mostrarInfo(i++); }); }
    }

    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    void inventarioComandante() {
        cout << YELLOW << "\n=== MOCHILA ===" << RESET << endl;
        if(inventario->isEmpty()) { cout << "Vacia." << endl; return; }
        int i=0; inventario->forEach([&i](Item* it){ cout << "[" << i++ << "] "; it->mostrarInfo(); });

        cout << "Usar item (-1 salir): "; int op; cin >> op;
        if(op >= 0 && op < inventario->getSize() && !ejercito_guardado->getListaTropas()->isEmpty()) {
            ejercito_guardado->getListaTropas()->get(0)->aplicarItemATodos(*inventario->get(op));
            inventario->removeAt(op);
            cout << GREEN << "Objeto usado." << RESET << endl;
        }
    }

    // --- COMBATE REAL ---
    void hogueraDeBatalla() {
        cout << RED << "\n======= BATALLA =======" << RESET << endl;
        cout << "1. Festin\n2. Combatir\n3. Volver\n> ";
        int op; cin >> op;

        if (op == 1) {
            int r = rand()%4;
            if(r==0) { cout << MAGENTA << "¡Bardo Epico! (+300 XP)" << RESET << endl; ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("X", BOOST_EXPERIENCIA, 300)); }); }
            else if(r==1) { cout << YELLOW << "Buena cena. (+100 XP)" << RESET << endl; ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("X", BOOST_EXPERIENCIA, 100)); }); }
            else if(r==2) { cout << CYAN << "Tambores. (+10 ATK)" << RESET << endl; ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("X", BOOST_ATAQUE, 10)); }); }
            else { cout << RED << "Comida podrida... (-5 HP)" << RESET << endl; ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("X", BOOST_VIDA, -5)); }); }
        }
        else if (op == 2) {
            int enemigo = (rand() % 151) + 100;
            cout << "ENEMIGO PODER: " << enemigo << " vs TU PODER: " << ejercito_guardado->getPoderCombateTotal() << endl;

            if (ejercito_guardado->getPoderCombateTotal() >= enemigo) {
                cout << GREEN << "¡VICTORIA!" << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("Win", BOOST_EXPERIENCIA, 200)); });

                // Carisma
                if(rand()%100 < 40) {
                    cout << "¿Reclutar desertores? (S/N): "; char c; cin >> c;
                    if(c=='S'||c=='s') { reclutarSoldado("Desertor",2,20,0); cout << CYAN << "+1 Desertor en reserva." << RESET << endl; }
                }
            } else {
                cout << RED << "¡DERROTA! Bajas confirmadas..." << RESET << endl;
                // Bajas
                if(!ejercito_guardado->getListaTropas()->isEmpty() && !ejercito_guardado->getListaTropas()->get(0)->estaVacia()) {
                    ejercito_guardado->getListaTropas()->get(0)->eliminarSoldado(0); // Muere uno
                    cout << RED << "Un soldado ha muerto." << RESET << endl;
                }

                // Refuerzos
                cout << YELLOW << "Llamando reservas..." << RESET << endl;
                if(!ejercito_guardado->getListaTropas()->isEmpty() && !soldados->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);
                    while(!t->estallena() && !soldados->isEmpty()) {
                        t->agregarSoldado(soldados->get(0));
                        soldados->removeAt(0);
                        cout << CYAN << "Refuerzo unido." << RESET << endl;
                    }
                    // Re-check
                    if(ejercito_guardado->getPoderCombateTotal() >= enemigo) cout << GREEN << "¡VICTORIA CON REFUERZOS!" << RESET << endl;
                    else cout << RED << "DERROTA DEFINITIVA." << RESET << endl;
                } else {
                    cout << RED << "No hay reservas o hueco." << RESET << endl;
                }
            }
        }
    }

    // --- GUARDADO REAL (SERIALIZACIÓN SIMPLE) ---
    void guardarPartida() {
        if (!fs::exists("json")) fs::create_directory("json");
        string ruta = "json/" + nombre + "_partida.json";
        ofstream f(ruta);

        if (f.is_open()) {
            // Guardamos datos básicos separados por espacios/saltos
            f << codificar(nombre) << " " << codificar(passwd) << endl;

            // Guardamos RESERVAS
            f << soldados->getSize() << endl;
            soldados->forEach([&f, this](Soldado* s) {
                f << codificar(s->getNombre()) << " " << s->getNivel() << " " << s->getPoderCombate() << " " << 0 << endl; // 0 exp base al guardar simple
            });

            // Guardamos TROPAS
            f << ejercito_guardado->getListaTropas()->getSize() << endl;
            ejercito_guardado->getListaTropas()->forEach([&f, this](Tropa* t) {
                f << codificar(t->getNombreTropa()) << " " << t->getNumSoldados() << endl;
                t->getListaSoldados()->forEach([&f, this](Soldado* s) {
                     f << codificar(s->getNombre()) << " " << s->getNivel() << " " << s->getPoderCombate() << " " << 0 << endl;
                });
            });

            f.close();
            cout << GREEN << "\n[GUARDADO REAL] Datos guardados en " << ruta << RESET << endl;
        }
    }

    // --- CARGA REAL (LECTURA DEL FICHERO) ---
    bool cargarDesdeArchivo(string ruta) {
        ifstream f(ruta);
        if (!f.is_open()) return false;

        string nombreF, passF;
        f >> nombreF >> passF; // Leemos cabecera
        // nombre = decodificar(nombreF); // Ya lo tenemos del main

        // 1. Cargar Reservas
        int numReservas; f >> numReservas;
        for(int i=0; i<numReservas; i++) {
            string n; int niv, pod, exp;
            f >> n >> niv >> pod >> exp;
            reclutarSoldado(decodificar(n), niv, pod, exp);
        }

        // 2. Cargar Tropas
        int numTropas; f >> numTropas;
        for(int i=0; i<numTropas; i++) {
            string nomTropa; int numSolds;
            f >> nomTropa >> numSolds;
            Tropa* t = new Tropa(decodificar(nomTropa));

            for(int j=0; j<numSolds; j++) {
                string n; int niv, pod, exp;
                f >> n >> niv >> pod >> exp;
                t->agregarSoldado(new Soldado(decodificar(n), true, niv, pod, exp));
            }
            ejercito_guardado->agregarTropa(t);
        }

        f.close();
        return true;
    }

    void reclutarSoldado(string n, int niv, int pod, int exp) {
        soldados->pushBack(new Soldado(n, true, niv, pod, exp));
    }
    Ejercito* getEjercito() { return ejercito_guardado; }
};

#endif