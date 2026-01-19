#include <iostream>
#include "Jugador.h"

using namespace std;

void pausa() {
    cout << "\nPresiona Enter para continuar...";
    cin.ignore();
    cin.get();
}

int main() {
    string usuario, pass;
    cout << "=== BIENVENIDO AL SISTEMA DE BATALLA GISBERIA ===" << endl;
    cout << "Usuario (Rey Enolas): ";
    cin >> usuario;
    cout << "Contrasena: ";
    cin >> pass;

    Jugador* elJugador = new Jugador(usuario, pass);

    // DATOS DE PRUEBA EXACTOS PARA QUE SALGAN IGUAL QUE LA CAPTURA
    // Tropa: Escuadron Crustaceo
    Tropa* t1 = new Tropa("Escuadron Crustaceo");
    t1->agregarSoldado(new Soldado("Patrik", true, 5, 25, 0));
    t1->agregarSoldado(new Soldado("Eugine", true, 5, 25, 0));
    t1->agregarSoldado(new Soldado("Stuart", true, 5, 25, 0));
    t1->agregarSoldado(new Soldado("Edward", true, 5, 25, 0));
    t1->agregarSoldado(new Soldado("Bob", true, 5, 25, 0));
    elJugador->getEjercito()->agregarTropa(t1);

    // Soldados sueltos
    elJugador->reclutarSoldado("Ubba", 4, 28, 0);
    elJugador->reclutarSoldado("Sigurd", 3, 30, 0);

    int opcion = 0;
    do {
        cout << "\n=======================================" << endl;
        cout << "       MENU PRINCIPAL: EJERCITO" << endl;
        cout << "=======================================" << endl;
        cout << "|                                     |" << endl;
        cout << "| 1. Ver Informacion del ejercito activo |" << endl;
        cout << "| 2. Gestionar soldados               |" << endl;
        cout << "| 3. Inventario de comandante         |" << endl;
        cout << "| 4. Hoguera de Batalla               |" << endl;
        cout << "| 5. Salir                            |" << endl;
        cout << "|                                     |" << endl;
        cout << "=======================================" << endl;
        cout << "\nSeleccione una opcion:\n";
        cin >> opcion;

        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        switch (opcion) {
            case 1:
                elJugador->verTropasExistentes();
                pausa();
                break;

            case 2: {
                int subop = 0;
                do {
                    cout << "\n-------------------------------------------" << endl;
                    cout << "      GESTION DE SOLDADOS Y TROPAS" << endl;
                    cout << "-------------------------------------------" << endl;
                    cout << "|                                         |" << endl;
                    cout << "| 1. Crear nueva tropa                    |" << endl;
                    cout << "| 2. Modificar tropas del ejercito        |" << endl;
                    cout << "| 3. Entrenar soldados de una tropa       |" << endl;
                    cout << "| 4. Ver soldados disponibles             |" << endl;
                    cout << "| 5. Ver tropas existentes                |" << endl;
                    cout << "| 6. Volver al menu principal             |" << endl;
                    cout << "|                                         |" << endl;
                    cout << "-------------------------------------------" << endl;
                    cout << "\nSeleccione una opcion:\n";
                    cin >> subop;

                    if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }

                    switch(subop) {
                        case 1: elJugador->crearNuevaTropa(); break;
                        case 2: elJugador->modificarTropas(); break;
                        case 3: elJugador->entrenarSoldados(); break;
                        case 4: elJugador->verSoldadosDisponibles(); break;
                        case 5: elJugador->verTropasExistentes(); break;
                        case 6: cout << "Volviendo..." << endl; break;
                        default: cout << "Opcion no valida." << endl;
                    }
                    if (subop != 6) pausa();

                } while (subop != 6);
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
                cout << "Guardando partida... Adios." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }

    } while (opcion != 5);

    delete elJugador;
    return 0;
}