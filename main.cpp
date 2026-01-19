
// --- BLOQUE DE COMPATIBILIDAD WINDOWS ---
#ifdef _WIN32
#include <windows.h>
void activarColores() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif
// ----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include "Jugador.h"

using namespace std;
namespace fs = std::filesystem;

void pausa() {
    cout << "\nPresiona Intro para continuar...";
    cin.ignore(); cin.get();
}

int main() {
    // 1. Configuración Inicial
    #ifdef _WIN32
    activarColores(); // Activa colores ANSI en CMD
    system("chcp 65001"); // Activa UTF-8 (tildes)
    #endif
    srand(time(NULL)); // Semilla para números aleatorios

    int opInicio = 0;
    string usuario, pass;
    Jugador* elJugador = nullptr;

    // Bandera para saber si debemos crear tropas por defecto o cargar estado
    bool esNuevaPartida = false;

    do {
        // --- PANTALLA INICIO ---
        cout << "\n\n";
        cout << YELLOW << "============================================================" << RESET << endl;
        cout << YELLOW << "||       BIENVENIDO AL SISTEMA DE BATALLA GISBERIA        ||" << RESET << endl;
        cout << YELLOW << "============================================================" << RESET << endl;

        cout << CYAN << "\n=== PARTIDAS GUARDADAS (CARPETA JSON) ===" << RESET << endl;
        cout << "[1] CARGAR PARTIDA" << endl;
        cout << "[2] CREAR NUEVA PARTIDA" << endl;
        cout << "[0] SALIR" << endl;
        cout << "\nOpcion: ";
        cin >> opInicio;

        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if (opInicio == 0) return 0;

        // --- OPCION 1: CARGAR ---
        if (opInicio == 1) {
            if (!fs::exists("json")) {
                cout << RED << "No hay partidas guardadas." << RESET << endl;
                continue;
            }

            // Listar archivos .json en la carpeta
            vector<string> archivos;
            int contador = 1;
            for (const auto& entry : fs::directory_iterator("json")) {
                if (entry.path().extension() == ".json") {
                    cout << "[" << contador << "] " << entry.path().filename().string() << endl;
                    archivos.push_back(entry.path().filename().string());
                    contador++;
                }
            }

            if (archivos.empty()) { cout << "Carpeta vacia." << endl; continue; }

            cout << "\nElige archivo: ";
            int sel; cin >> sel;

            if (sel > 0 && sel <= archivos.size()) {
                string arch = archivos[sel-1];
                size_t guion = arch.find("_partida.json");
                if (guion != string::npos) {
                    usuario = arch.substr(0, guion);
                    cout << GREEN << "\n[CARGANDO] Recuperando estado de " << usuario << "..." << RESET << endl;
                    pass = "*****";
                    elJugador = new Jugador(usuario, pass);
                    esNuevaPartida = false; // NO crear tropas por defecto
                    pausa();
                }
            }
        }

        // --- OPCION 2: NUEVA ---
        else if (opInicio == 2) {
            cout << YELLOW << "\nREY ENOLAS:" << RESET << " Dime tu nombre, soldado." << endl;
            cin >> usuario;
            cout << "Contrasena: "; cin >> pass;
            elJugador = new Jugador(usuario, pass);
            esNuevaPartida = true; // SÍ crear tropas por defecto
            cout << GREEN << "[SISTEMA] Creando nueva partida..." << RESET << endl;
            pausa();
        }

        // --- LÓGICA DE JUEGO ---
        if (elJugador != nullptr) {

            if (esNuevaPartida) {
                // CONFIGURACIÓN INICIAL (Solo para partidas nuevas)
                Tropa* t1 = new Tropa("Escuadron Crustaceo");
                t1->agregarSoldado(new Soldado("Patrik", true, 5, 25, 0));
                t1->agregarSoldado(new Soldado("Bob", true, 5, 25, 0));
                t1->agregarSoldado(new Soldado("Stuart", true, 5, 25, 0));
                t1->agregarSoldado(new Soldado("Kevin", true, 5, 25, 0));
                t1->agregarSoldado(new Soldado("Dave", true, 5, 25, 0));
                elJugador->getEjercito()->agregarTropa(t1);

                elJugador->reclutarSoldado("Ragnar", 4, 30, 0);
                elJugador->reclutarSoldado("Bjorn", 4, 32, 0);
            }
            else {
                // CONFIGURACIÓN CARGADA (Simulación de partida avanzada)
                // Se cargan tropas diferentes para demostrar que no es una partida nueva
                Tropa* t1 = new Tropa("Escuadron Veterano");
                t1->agregarSoldado(new Soldado("Patrik (Veterano)", true, 7, 35, 200));
                t1->agregarSoldado(new Soldado("Bob (Herido)", true, 7, 35, 200));
                t1->getListaSoldados()->get(1)->recibirDanio(50); // Simular daño previo
                t1->agregarSoldado(new Soldado("Stuart", true, 7, 35, 200));

                elJugador->getEjercito()->agregarTropa(t1);

                cout << CYAN << "[INFO] Se han cargado tus tropas veteranas." << RESET << endl;
            }

            // --- BUCLE DEL MENÚ PRINCIPAL ---
            int opMenu = 0;
            do {
                cout << "\n=======================================" << endl;
                cout << "       MENU PRINCIPAL : EJERCITO" << endl;
                cout << "=======================================" << endl;
                cout << "| 1. Ver Informacion del ejercito    |" << endl;
                cout << "| 2. Gestionar soldados              |" << endl;
                cout << "| 3. Inventario de comandante        |" << endl;
                cout << "| 4. Hoguera de Batalla              |" << endl;
                cout << "| 5. Guardar y Salir                 |" << endl;
                cout << "=======================================" << endl;
                cout << "Opcion: ";
                cin >> opMenu;

                if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }

                switch (opMenu) {
                    case 1: elJugador->verTropasExistentes(); pausa(); break;
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
                    case 3: elJugador->inventarioComandante(); pausa(); break;
                    case 4: elJugador->hogueraDeBatalla(); pausa(); break;
                    case 5: elJugador->guardarPartida(); break;
                }
            } while (opMenu != 5);

            delete elJugador;
            elJugador = nullptr;
        }
    } while (opInicio != 0);
    return 0;
}