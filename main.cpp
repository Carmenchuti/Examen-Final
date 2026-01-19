
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
// -------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib> // <--- NUEVO: Para rand()
#include <ctime>   // <--- NUEVO: Para time()
#include "Jugador.h"

using namespace std;
namespace fs = std::filesystem;

void pausa() {
    cout << "\nPresiona Intro para continuar...";
    cin.ignore(); cin.get();
}

int main() {
    // ACTIVAR COLORES EN WINDOWS
    #ifdef _WIN32
    activarColores();
    system("chcp 65001"); // Para que las tildes salgan bien
    #endif

    int opInicio = 0;
    string usuario, pass;
    Jugador* elJugador = nullptr;

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

        // --- OPCION 1: CARGAR PARTIDA ---
        if (opInicio == 1) {
            cout << CYAN << "\n--- ARCHIVOS ENCONTRADOS ---" << RESET << endl;

            if (!fs::exists("json")) {
                cout << RED << "No existe la carpeta 'json'. Crea una partida primero." << RESET << endl;
                continue;
            }

            vector<string> archivos;
            int contador = 1;
            // Listamos los archivos de la carpeta
            for (const auto& entry : fs::directory_iterator("json")) {
                if (entry.path().extension() == ".json") {
                    cout << "[" << contador << "] " << entry.path().filename().string() << endl;
                    archivos.push_back(entry.path().filename().string());
                    contador++;
                }
            }

            if (archivos.empty()) {
                cout << "La carpeta esta vacia." << endl;
                continue;
            }

            cout << "\nElige numero de archivo: ";
            int seleccion;
            cin >> seleccion;

            if (seleccion > 0 && seleccion <= archivos.size()) {
                string archivoElegido = archivos[seleccion - 1];

                // Extraemos el nombre: "Carmen_partida.json" -> "Carmen"
                size_t guion = archivoElegido.find("_partida.json");
                if (guion != string::npos) {
                    usuario = archivoElegido.substr(0, guion);
                    cout << GREEN << "\n¡Lectura exitosa!" << RESET << endl;
                    cout << YELLOW << "Bienvenido de nuevo, Gran Comandante " << usuario << "." << RESET << endl;
                    cout << "Tus tropas estaban esperandote." << endl;
                    pass = "******";
                    elJugador = new Jugador(usuario, pass);
                    pausa();
                } else {
                    cout << RED << "Archivo invalido." << RESET << endl;
                    continue;
                }
            } else {
                continue;
            }
        }

        // --- OPCION 2: NUEVA PARTIDA ---
        else if (opInicio == 2) {
            cout << CYAN << "\n[INFO] Iniciando nueva partida..." << RESET << endl;
            cout << YELLOW << "\nREY ENOLAS:" << RESET << " Bienvenido. Dime, ¿como te llamas?" << endl;
            cout << "Nombre: "; cin >> usuario;
            cout << YELLOW << "REY ENOLAS:" << RESET << " Bien, " << usuario << ". Contrasena:" << endl;
            cout << "Contrasena: "; cin >> pass;
            cout << YELLOW << "REY ENOLAS:" << RESET << " ¡Perfecto! Toma esta tropa inicial." << endl;
            cout << GREEN << "[SISTEMA] Partida creada." << RESET << endl;
            elJugador = new Jugador(usuario, pass);
            pausa();
        }

        // --- LÓGICA DEL JUEGO ---
        if (elJugador != nullptr) {
            // Tropa Demo
            Tropa* t1 = new Tropa("Escuadron Crustaceo");
            t1->agregarSoldado(new Soldado("Patrik", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Bob", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Stuart", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Kevin", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Dave", true, 5, 25, 0));
            elJugador->getEjercito()->agregarTropa(t1);

            elJugador->reclutarSoldado("Ragnar", 4, 30, 0);
            elJugador->reclutarSoldado("Bjorn", 4, 32, 0);

            // MENÚ PRINCIPAL
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