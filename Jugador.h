//
// Created by athin on 19/01/2026.
//

#ifndef EXAMEN_FINAL_JUGADOR_H
#define EXAMEN_FINAL_JUGADOR_H
#include "Ejercito.h"
#include "Soldado.h"
#include "Item.h"
#include <string>
#include <iostream>
#include <fstream> // Para guardar partida
using namespace std;

class Jugador {
private:
    string nombre;
    string passwd;

    // SOLDADOS DISPONIBLES (Pool de reclutamiento)
    LinkedList<Soldado*>* soldados;

    // EJERCITO (Tus tropas organizadas)
    Ejercito* ejercito_guardado;

    // INVENTARIO (Necesario para el 2.5 de la rúbrica)
    LinkedList<Item*>* inventario;

public:
    Jugador(string nombre, string pass) : nombre(nombre), passwd(pass) {
        soldados = new LinkedList<Soldado*>();
        ejercito_guardado = new Ejercito("Ejercito de " + nombre, true);
        inventario = new LinkedList<Item*>();

        // Inicializamos con algunos items de prueba para el inventario
        inventario->pushBack(new Item("Pocion de Vida", BOOST_VIDA, 50));
        inventario->pushBack(new Item("Espada Legendaria", BOOST_ATAQUE, 20));
        inventario->pushBack(new Item("Libro de Sabiduria", BOOST_EXPERIENCIA, 100));
    }

    // --- GESTIÓN DE SOLDADOS Y TROPAS ---

    void crearNuevaTropa() {
        string nombreTropa;
        cout << "Nombre de la nueva tropa: ";
        cin >> nombreTropa;
        ejercito_guardado->agregarTropa(new Tropa(nombreTropa));
        cout << "Tropa '" << nombreTropa << "' creada." << endl;
    }

    void verSoldadosDisponibles() {
        cout << "\n--- SOLDADOS SIN ASIGNAR ---" << endl;
        if (soldados->isEmpty()) {
            cout << "No tienes soldados libres." << endl;
            return;
        }
        // Uso de recursividad forEach (Persona 1)
        soldados->forEach([](Soldado* s) {
            s->mostrarInfo();
        });
    }

    void verTropasExistentes() {
        ejercito_guardado->mostrarInfo();
    }

    // Mueve un soldado de la lista general a una tropa
    void entrenarSoldados() {
        if (soldados->isEmpty()) {
            cout << "Necesitas reclutar soldados primero." << endl;
            return;
        }
        if (ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "Crea una tropa primero." << endl;
            return;
        }

        cout << "Selecciona Tropa (Indice 0, 1...): " << endl;
        verTropasExistentes();
        int idxTropa;
        cin >> idxTropa;

        // Obtenemos la tropa (Recursivo get)
        Tropa* tropaDestino = ejercito_guardado->getListaTropas()->get(idxTropa);

        cout << "Selecciona Soldado para asignar (Indice 0, 1...): " << endl;
        // Mostramos índices manualmente para ayudar al usuario
        int i = 0;
        soldados->forEach([&i](Soldado* s) {
            cout << "[" << i++ << "] " << s->getNombre() << endl;
        });

        int idxSoldado;
        cin >> idxSoldado;

        Soldado* s = soldados->get(idxSoldado);

        if (tropaDestino->agregarSoldado(s)) {
            // Si cabe, lo quitamos de la lista de disponibles
            soldados->removeAt(idxSoldado);
            cout << "Soldado asignado correctamente." << endl;
        }
    }

    // --- RÚBRICA EJERCICIO 2 (2.5 Pts) ---
    void inventarioComandante() {
        cout << "\n=== INVENTARIO DEL COMANDANTE ===" << endl;
        if (inventario->isEmpty()) {
            cout << "Mochila vacia." << endl;
            return;
        }

        // 1. Mostrar Items
        int i = 0;
        inventario->forEach([&i](Item* item) {
            cout << "[" << i++ << "] ";
            item->mostrarInfo();
        });

        cout << "Elige item para usar (-1 para salir): ";
        int idxItem;
        cin >> idxItem;
        if (idxItem < 0) return;

        Item* itemUsar = inventario->get(idxItem);

        // 2. Elegir Objetivo (Tropa completa o Soldado suelto)
        cout << "1. Usar en un Soldado suelto\n2. Usar en una Tropa entera\n> ";
        int op;
        cin >> op;

        if (op == 1 && !soldados->isEmpty()) {
            cout << "Indice de soldado: ";
            int idxSol;
            cin >> idxSol;
            soldados->get(idxSol)->aplicarItem(*itemUsar);
            cout << "Objeto aplicado." << endl;
            // Consumir item (borrar del inventario)
            inventario->removeAt(idxItem);
        }
        else if (op == 2 && !ejercito_guardado->getListaTropas()->isEmpty()) {
            cout << "Indice de tropa: ";
            int idxTr;
            cin >> idxTr;
            // Aplicar recursivamente a todos los de la tropa
            ejercito_guardado->getListaTropas()->get(idxTr)->aplicarItemATodos(*itemUsar);
            cout << "Objeto aplicado a toda la tropa." << endl;
            inventario->removeAt(idxItem);
        }
        else {
            cout << "No hay objetivos validos." << endl;
        }
    }

    // --- METODOS DE APOYO ---
    void reclutarSoldado(string nombre, int nivel) {
        soldados->pushBack(new Soldado(nombre, true, nivel, 10 + (nivel*5)));
    }

    // Necesario para que Persona 3 implemente el combate
    Ejercito* getEjercito() { return ejercito_guardado; }
    LinkedList<Soldado*>* getPoolSoldados() { return soldados; }

    // --- RÚBRICA EJERCICIO 3: COMBATE Y REFUERZOS ---
    void hogueraDeBatalla() {
        cout << "\n🔥 --- HOGUERA DE BATALLA --- 🔥" << endl;

        // 1. Generar Enemigo Aleatorio
        Ejercito* enemigo = new Ejercito("La Horda Oscura", false);
        Tropa* tropaMala = new Tropa("Orcos");
        tropaMala->agregarSoldado(new Soldado("Orco Jefe", false, 5, 50));
        tropaMala->agregarSoldado(new Soldado("Goblin", false, 1, 10));
        tropaMala->agregarSoldado(new Soldado("Goblin", false, 1, 10));
        enemigo->agregarTropa(tropaMala);

        cout << "¡Un ejercito enemigo se aproxima!" << endl;
        enemigo->mostrarInfo();

        cout << "\nTu poder actual: " << ejercito_guardado->getPoderCombateTotal() << endl;
        cout << "Poder enemigo: " << enemigo->getPoderCombateTotal() << endl;

        cout << "1. Luchar\n2. Huir\n> ";
        int op;
        cin >> op;

        if (op == 1) {
            // Lógica simple de combate: Gana quien tenga más poder total
            if (ejercito_guardado->getPoderCombateTotal() >= enemigo->getPoderCombateTotal()) {
                cout << "¡VICTORIA! Tus tropas han arrasado al enemigo." << endl;
                // Ganar experiencia (simulado a todos)
                ejercito_guardado->getListaTropas()->forEach([](Tropa* t) {
                    t->aplicarItemATodos(Item("XP Victoria", BOOST_EXPERIENCIA, 100));
                });
            } else {
                cout << "DERROTA... Tus tropas han sufrido bajas." << endl;
                // Simular daño a la primera tropa
                if (!ejercito_guardado->getListaTropas()->isEmpty()) {
                    Tropa* primera = ejercito_guardado->getListaTropas()->get(0);
                    // Matamos al primer soldado como castigo
                    if (!primera->estaVacia()) primera->eliminarSoldado(0);

                    // --- RÚBRICA: REFUERZOS (1 pt) ---
                    // Si la tropa ha quedado mermada, buscamos en la reserva (soldados)
                    cout << "¡Solicitando refuerzos de la reserva!" << endl;
                    while (!primera->estallena() && !soldados->isEmpty()) {
                        Soldado* refuerzo = soldados->get(0);
                        primera->agregarSoldado(refuerzo);
                        soldados->removeAt(0);
                        cout << "Refuerzo " << refuerzo->getNombre() << " unido al frente." << endl;
                    }
                }
            }
        } else {
            cout << "Has huido cobardemente." << endl;
        }

        delete enemigo; // Limpiar memoria del enemigo generado
    }
};
#endif //EXAMEN_FINAL_JUGADOR_H