

#ifndef JUGADOR_H
#define JUGADOR_H

#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream>      // Para manejo de archivos (Guardar partida)
#include <filesystem>   // Para crear carpetas (C++17)
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// --- COLORES ANSI PARA LA CONSOLA ---
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

    // ESTRUCTURAS DE DATOS DINÁMICAS
    LinkedList<Soldado*>* soldados;       // Pool de reserva (Soldados sin asignar)
    Ejercito* ejercito_guardado;          // Ejército activo (Tropas asignadas)
    LinkedList<Item*>* inventario;        // Mochila de objetos (Ejercicio 2 Rúbrica)

public:
    /**
     * @brief Constructor del Jugador.
     * @param nombre Nombre del usuario (Rey Enolas).
     * @param pass Contraseña para el login simulado.
     */
    Jugador(string nombre, string pass) : nombre(nombre), passwd(pass) {
        soldados = new LinkedList<Soldado*>();
        ejercito_guardado = new Ejercito("Ejercito de " + nombre + " V de Gisberia", true);
        inventario = new LinkedList<Item*>();

        // Inicializamos el inventario con objetos por defecto
        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Tacticas", BOOST_EXPERIENCIA, 200));
    }

    /**
     * @brief Crea una nueva tropa vacía y la añade al ejército.
     * @note Usa memoria dinámica (new Tropa).
     */
    void crearNuevaTropa() {
        cout << YELLOW << "\n=== CREAR NUEVA TROPA ===" << RESET << endl;
        string nombreTropa;
        cout << "Nombre de la tropa: ";
        cin >> nombreTropa;
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << GREEN << "Tropa creada exitosamente." << RESET << endl;
    }

    /**
     * @brief Menú para eliminar tropas.
     * @note Demuestra manipulación de la LinkedList (eliminar nodos).
     */
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

    /**
     * @brief Sistema de entrenamiento (subida de nivel).
     * @details Aplica experiencia a todos los soldados de una tropa usando recursividad (forEach).
     */
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

        // Lambda recursiva para aplicar XP a cada soldado
        tropa->getListaSoldados()->forEach([xp](Soldado* s){ s->ganarExperiencia(xp); });
    }

    /**
     * @brief Muestra los soldados que están en el "pool" de reserva.
     */
    void verSoldadosDisponibles() {
        cout << CYAN << "\n=== SOLDADOS EN RESERVA ===" << RESET << endl;
        if (soldados->isEmpty()) cout << "Reserva vacia." << endl;
        else {
            int i = 0;
            soldados->forEach([&i](Soldado* s) { s->mostrarInfo(i++); });
        }
    }

    /**
     * @brief Muestra toda la información del ejército formateada en tablas.
     */
    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    /**
     * @brief Gestión del inventario (Rúbrica Ejercicio 2).
     * @details Permite seleccionar un objeto y aplicarlo a una tropa entera.
     */
    void inventarioComandante() {
        cout << YELLOW << "\n=== MOCHILA ===" << RESET << endl;
        if (inventario->isEmpty()) { cout << "Vacia." << endl; return; }

        int i = 0;
        inventario->forEach([&i](Item* it) { cout << "[" << i++ << "] "; it->mostrarInfo(); });

        cout << "Usar item nº (-1 salir): ";
        int op; cin >> op;
        if (op >= 0 && op < inventario->getSize()) {
            if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                // Aplicar item a la primera tropa disponible (Simplificación para demo)
                ejercito_guardado->getListaTropas()->get(0)->aplicarItemATodos(*inventario->get(op));
                inventario->removeAt(op); // Consumir objeto
                cout << GREEN << "Item usado en la primera tropa." << RESET << endl;
            } else {
                cout << RED << "No tienes tropas." << RESET << endl;
            }
        }
    }

    /**
     * @brief Sistema de Combate Completo (Rúbrica Ejercicio 3).
     * @details Incluye toda la lógica compleja:
     * 1. Eventos Aleatorios (Festín).
     * 2. Generación procedural de enemigos.
     * 3. Sistema de Derrota: Muerte de soldados y daño masivo.
     * 4. Sistema de Refuerzos: Mover soldados de Reserva -> Tropa Activa.
     * 5. Evento de Carisma: Reclutar enemigos tras la victoria.
     */
    void hogueraDeBatalla() {
        cout << RED << "\n======= HOGUERA DE BATALLA =======" << RESET << endl;
        cout << "1. Celebrar Festin (Evento Aleatorio)" << endl;
        cout << "2. Otear el horizonte (Combatir)" << endl;
        cout << "3. Volver" << endl;
        cout << "Elige tu destino: ";
        int op; cin >> op;

        // --- OPCIÓN 1: FESTÍN RANDOM ---
        if (op == 1) {
            cout << "\nPreparando la hoguera..." << endl;
            int suerte = rand() % 4; // RNG 0-3

            if (suerte == 0) {
                cout << MAGENTA << "¡INCREIBLE! El Rey Enolas ha enviado un bardo famoso." << RESET << endl;
                cout << GREEN << "[BONUS] +300 EXP para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("Epica", BOOST_EXPERIENCIA, 300)); });
            }
            else if (suerte == 1) {
                cout << YELLOW << "Huele a carne asada. Una cena copiosa." << RESET << endl;
                cout << GREEN << "[BONUS] +100 EXP para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("Carne Asada", BOOST_EXPERIENCIA, 100)); });
            }
            else if (suerte == 2) {
                cout << CYAN << "¡BUM! ¡BUM! Los tambores de guerra resuenan." << RESET << endl;
                cout << GREEN << "[BONUS] +10 ATAQUE permanente para todos." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("Furia", BOOST_ATAQUE, 10)); });
            }
            else {
                cout << RED << "¡PUAJ! La comida estaba en mal estado..." << RESET << endl;
                cout << RED << "[PENALIZACION] -5 VIDA a todos por indigestión." << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){ t->aplicarItemATodos(Item("Indigestion", BOOST_VIDA, -5)); });
            }
        }

        // --- OPCIÓN 2: COMBATE ---
        else if (op == 2) {
            cout << "\n\033[1;35m--- INFORME DE EXPLORADORES ---\033[0m" << endl;

            // Generación aleatoria de enemigo
            string nombres[] = {"Orcos de Mordor", "Bandidos", "Esqueletos", "Mercenarios", "Goblins"};
            string nombreEnemigo = nombres[rand() % 5];
            int poderEnemigo = (rand() % 151) + 100; // Poder entre 100 y 250

            cout << "¡Se aproxima una tropa enemiga!" << endl;
            cout << " - Enemigo: " << nombreEnemigo << " (Poder estimado: " << poderEnemigo << ")" << endl;

            int miPoder = ejercito_guardado->getPoderCombateTotal();
            cout << "\nTU PODER: " << miPoder << " vs ENEMIGO: " << poderEnemigo << endl;

            if (miPoder >= poderEnemigo) {
                // --- CASO VICTORIA ---
                cout << GREEN << "\n¡VICTORIA GLORIOSA!" << RESET << endl;
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                     t->aplicarItemATodos(Item("Victoria", BOOST_EXPERIENCIA, 200));
                });

                // EVENTO DE CARISMA (Reclutar enemigo - 40% prob)
                int azarReclutar = rand() % 100;
                if (azarReclutar < 40) {
                    cout << "\nEl ejercito enemigo, inspirado por el Valor de tus soldados ha solicitado unirse a tu ejercito. ";
                    cout << "¿Quieres que se unan a ti? [S]i [N]o: ";
                    char resp; cin >> resp;
                    if (resp == 'S' || resp == 's') {
                        cout << GREEN << "\nLos soldados del ejercito enemigo se han unido a tus tropas gracias a tu Carisma y Convicción en Batalla" << RESET << endl;
                        // Añadimos desertores a la reserva
                        reclutarSoldado("Desertor " + nombreEnemigo, 2, 20, 0);
                        reclutarSoldado("Desertor " + nombreEnemigo, 2, 20, 0);
                        cout << CYAN << "(+2 Soldados enemigos añadidos a la reserva)" << RESET << endl;
                    }
                }

            } else {
                // --- CASO DERROTA ---
                cout << RED << "\n¡DERROTA! Nos están masacrando..." << RESET << endl;

                // 1. BAJAS REALISTAS (Obligatorio para hacer hueco en la tropa)
                if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                    Tropa* t = ejercito_guardado->getListaTropas()->get(0);
                    if (!t->estaVacia()) {
                        cout << RED << " >> " << t->getListaSoldados()->get(0)->getNombre() << " ha caido en combate (MUERTO)." << RESET << endl;
                        t->eliminarSoldado(0); // Liberar hueco eliminando nodo
                    }
                    // Daño masivo al resto
                    t->getListaSoldados()->forEach([](Soldado* s){ s->recibirDanio(40); });
                    cout << RED << " >> El resto de la tropa ha recibido daño critico." << RESET << endl;
                }

                // 2. LLAMADA A REFUERZOS (Punto clave de la rúbrica)
                cout << YELLOW << "\n[SISTEMA] ¡Bajas confirmadas! Llamando a la reserva..." << RESET << endl;

                // Verificamos si hay soldados en la reserva
                if (soldados->isEmpty()) {
                    cout << RED << "[ERROR CRITICO] ¡No quedan soldados en la reserva! No pueden llegar refuerzos." << RESET << endl;
                } else {
                    int poderRefuerzos = 0;
                    if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                        Tropa* t = ejercito_guardado->getListaTropas()->get(0);

                        // Bucle de refuerzo: Mientras haya sitio y haya reservas
                        while (!t->estallena() && !soldados->isEmpty()) {
                            Soldado* ref = soldados->get(0);
                            poderRefuerzos += ref->getPoderCombate();
                            t->agregarSoldado(ref);
                            soldados->removeAt(0); // Sacar de reserva
                            cout << CYAN << " >> " << ref->getNombre() << " llega corriendo desde la reserva!" << RESET << endl;
                        }
                    }

                    if (poderRefuerzos > 0) {
                        cout << CYAN << "\n[CONTRAATAQUE] ¡Tropas reorganizadas!" << RESET << endl;
                        cout << "Poder Extra Sumado: " << poderRefuerzos << endl;

                        // SEGUNDA OPORTUNIDAD
                        int nuevoPoderTotal = ejercito_guardado->getPoderCombateTotal();
                        cout << "NUEVO PODER: " << nuevoPoderTotal << " vs ENEMIGO: " << poderEnemigo << endl;

                        if (nuevoPoderTotal >= poderEnemigo) {
                            cout << GREEN << "\n¡VICTORIA EPICA! Los refuerzos han vengado a los caidos." << RESET << endl;
                            ejercito_guardado->getListaTropas()->forEach([](Tropa* t){
                                t->aplicarItemATodos(Item("Victoria Epica", BOOST_EXPERIENCIA, 300));
                            });
                        } else {
                            cout << RED << "\n¡DERROTA FINAL! Retirada..." << RESET << endl;
                        }
                    } else {
                        cout << RED << "¡La tropa esta llena! No caben mas refuerzos." << RESET << endl;
                    }
                }
            }
        }

        cout << "\nPresiona Enter para continuar...";
        cin.ignore(); cin.get();
    }

    /**
     * @brief Guarda la partida en un archivo JSON simulado.
     * @details Crea la carpeta 'json' si no existe. Usa ios::trunc para asegurar sobrescritura.
     */
    void guardarPartida() {
        if (!fs::exists("json")) fs::create_directory("json");
        string ruta = "json/" + nombre + "_partida.json";

        // ios::trunc es CLAVE: Borra el contenido anterior y escribe lo nuevo
        ofstream f(ruta, ios::trunc);

        if (f.is_open()) {
            f << "{\n";
            f << "  \"jugador\": \"" << nombre << "\",\n";
            f << "  \"tropas_activas\": " << ejercito_guardado->getListaTropas()->getSize() << ",\n";
            f << "  \"reservas_disponibles\": " << soldados->getSize() << ",\n";
            f << "  \"id_guardado\": " << rand() << "\n"; // ID Random para verificar cambios
            f << "}\n";
            f.close();
            cout << GREEN << "\n[SISTEMA] Guardado en: " << ruta << RESET << endl;
        }
    }

    // Helper para añadir soldados a la reserva
    void reclutarSoldado(string n, int niv, int pod, int exp) {
        soldados->pushBack(new Soldado(n, true, niv, pod, exp));
    }
    Ejercito* getEjercito() { return ejercito_guardado; }
};

#endif