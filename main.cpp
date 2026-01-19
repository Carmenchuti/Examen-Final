#include <iostream>
#include <string>
#include "Jugador.h"

using namespace std;

// Pausa simple para leer textos
void pausa() {
    cout << "\nPresiona Intro para continuar...";
    cin.ignore(); cin.get();
}

int main() {
    int opInicio = 0;
    string usuario, pass;

    do {
        // --- 1. PANTALLA DE INICIO (AMARILLA Y CIAN) ---
        cout << "\n\n";
        cout << YELLOW << "============================================================" << RESET << endl;
        cout << YELLOW << "||       BIENVENIDO AL SISTEMA DE BATALLA GISBERIA        ||" << RESET << endl;
        cout << YELLOW << "============================================================" << RESET << endl;

        cout << CYAN << "\n=== PARTIDAS GUARDADAS ===" << RESET << endl;
        cout << "[1] Geo_partida.json" << endl;
        cout << "[2] Crear nueva partida" << endl;
        cout << "[0] Salir" << endl;
        cout << "\nOpcion: ";
        cin >> opInicio;

        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if (opInicio == 0) return 0;

        // --- 2. INTRODUCCIÓN DEL REY (TEXTO) ---
        cout << CYAN << "\n[INFO] Iniciando..." << RESET << endl;

        cout << YELLOW << "\nREY ENOLAS:" << RESET << " Bienvenido. Dime, ¿como te llamas?" << endl;
        cout << "Nombre: ";
        cin >> usuario;

        cout << YELLOW << "REY ENOLAS:" << RESET << " Bien, " << usuario << ". Establece tu contrasena." << endl;
        cout << "Contrasena: ";
        cin >> pass;

        cout << YELLOW << "REY ENOLAS:" << RESET << " ¡Perfecto! Toma esta tropa inicial." << endl;
        pausa();

        // --- 3. CARGA DE DATOS (PREPARAR DEMO) ---
        Jugador* elJugador = new Jugador(usuario, pass);

        // Creamos la "Tropa Demo" para que salga bonita al verla
        Tropa* t1 = new Tropa("Escuadron Crustaceo");
        t1->agregarSoldado(new Soldado("Patrik", true, 5, 25, 0));
        t1->agregarSoldado(new Soldado("Bob", true, 5, 25, 0));
        t1->agregarSoldado(new Soldado("Stuart", true, 5, 25, 0));
        t1->agregarSoldado(new Soldado("Kevin", true, 5, 25, 0));
        t1->agregarSoldado(new Soldado("Dave", true, 5, 25, 0));
        elJugador->getEjercito()->agregarTropa(t1);

        // Soldados en reserva
        elJugador->reclutarSoldado("Ragnar", 4, 30, 0);
        elJugador->reclutarSoldado("Bjorn", 4, 32, 0);

        // --- 4. MENÚ PRINCIPAL ---
        int opMenu = 0;
        do {
            cout << "\n=======================================" << endl;
            cout << "       MENU PRINCIPAL : EJERCITO" << endl;
            cout << "=======================================" << endl;
            cout << "| 1. Ver Informacion del ejercito    |" << endl;
            cout << "| 2. Gestionar soldados              |" << endl;
            cout << "| 3. Inventario de comandante        |" << endl;
            cout << "| 4. Hoguera de Batalla              |" << endl;
            cout << "| 5. Salir                           |" << endl;
            cout << "=======================================" << endl;
            cout << "Opcion: ";
            cin >> opMenu;

            if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }

            switch (opMenu) {
                case 1:
                    elJugador->verTropasExistentes();
                    pausa();
                    break;
                case 2: {
                    int sub = 0;
                    do {
                        cout << "\n--- GESTION ---" << endl;
                        cout << "1. Crear tropa\n2. Modificar tropas\n3. Entrenar\n4. Ver reserva\n5. Ver tropas\n6. Volver" << endl;
                        cout << "Opcion: "; cin >> sub;

                        if (sub==1) elJugador->crearNuevaTropa();
                        else if (sub==2) elJugador->modificarTropas();
                        else if (sub==3) elJugador->entrenarSoldados();
                        else if (sub==4) elJugador->verSoldadosDisponibles();
                        else if (sub==5) elJugador->verTropasExistentes();

                        if (sub != 6) pausa();
                    } while (sub != 6);
                    break;
                }
                case 3:
                    elJugador->inventarioComandante();
                    pausa();
                    break;
                case 4:
                    elJugador->hogueraDeBatalla();
                    pausa();
                    break;
                case 5:
                    elJugador->guardarPartida();
                    break;
            }
        } while (opMenu != 5);

        delete elJugador;
        return 0;

    } while (opInicio != 0);
    return 0;
}