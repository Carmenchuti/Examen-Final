#include <iostream>
#include "Jugador.h"

using namespace std;

// Función auxiliar para pausar
void pausa() {
    cout << "Presiona Enter para continuar...";
    cin.ignore();
    cin.get();
}

int main() {
    // 1. PANTALLA DE LOGIN (Como en el video)
    string usuario, pass;
    cout << "=== BIENVENIDO AL SISTEMA DE BATALLA GISBERIA ===" << endl;
    cout << "Usuario (Rey Enolas): ";
    cin >> usuario;
    cout << "Contrasena: ";
    cin >> pass;

    // Instanciamos al Jugador (El Manager)
    Jugador* elJugador = new Jugador(usuario, pass);

    // Damos algunos soldados iniciales para que no esté vacío
    elJugador->reclutarSoldado("Recluta Patoso", 1);
    elJugador->reclutarSoldado("Rambo", 10);
    elJugador->reclutarSoldado("Legolas", 5);

    int opcion = 0;
    do {
        cout << "\n--- MENU PRINCIPAL: EJERCITO ---" << endl;
        cout << "1. Ver Informacion del ejercito activo" << endl; // verTropasExistentes
        cout << "2. Gestionar soldados (Crear tropas/Entrenar)" << endl;
        cout << "3. Inventario de comandante" << endl;
        cout << "4. Hoguera de Batalla" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        // Control de errores de input (Bug Fix)
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (opcion) {
            case 1:
                elJugador->verTropasExistentes();
                break;
            case 2:
                int subop;
                cout << "\n--- GESTION ---" << endl;
                cout << "1. Crear Nueva Tropa" << endl;
                cout << "2. Ver Soldados Disponibles" << endl;
                cout << "3. Asignar Soldado a Tropa" << endl;
                cin >> subop;
                if (subop == 1) elJugador->crearNuevaTropa();
                else if (subop == 2) elJugador->verSoldadosDisponibles();
                else if (subop == 3) elJugador->entrenarSoldados();
                break;
            case 3:
                elJugador->inventarioComandante();
                break;
            case 4:
                elJugador->hogueraDeBatalla();
                break;
            case 5:
                cout << "Guardando partida... (Simulado)" << endl;
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
        if(opcion != 5) pausa();

    } while (opcion != 5);

    delete elJugador;
    return 0;
}