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

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include "Jugador.h"

using namespace std;
namespace fs = std::filesystem;

void pausa() {
    cout << "\nPresiona Intro para continuar...";
    cin.ignore(); cin.get();
}

int main() {
    #ifdef _WIN32
    activarColores();
    system("chcp 65001");
    #endif
    srand(time(NULL));

    int opInicio = 0;
    string usuario, pass;
    Jugador* elJugador = nullptr;
    bool cargaExitosa = false;

    do {
        cout << "\n\n";
        cout << YELLOW << "============================================================" << RESET << endl;
        cout << YELLOW << "||       BIENVENIDO AL SISTEMA DE BATALLA GISBERIA        ||" << RESET << endl;
        cout << YELLOW << "============================================================" << RESET << endl;

        cout << CYAN << "\n=== MENU INICIO ===" << RESET << endl;
        cout << "[1] CARGAR PARTIDA" << endl;
        cout << "[2] NUEVA PARTIDA" << endl;
        cout << "[0] SALIR" << endl;
        cout << "Opcion: "; cin >> opInicio;

        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if (opInicio == 0) return 0;

        // --- CARGAR ---
        if (opInicio == 1) {
            if (!fs::exists("json")) { cout << RED << "No hay partidas." << RESET << endl; continue; }

            vector<string> archivos;
            int c = 1;
            for (const auto& entry : fs::directory_iterator("json")) {
                if (entry.path().extension() == ".json") {
                    cout << "[" << c++ << "] " << entry.path().filename().string() << endl;
                    archivos.push_back(entry.path().string()); // Guardamos ruta completa
                }
            }
            if (archivos.empty()) { cout << "Vacio." << endl; continue; }

            cout << "Elige: "; int sel; cin >> sel;
            if (sel > 0 && sel <= archivos.size()) {
                // Instanciamos jugador vacío temporalmente
                // El nombre da igual porque se sobreescribe al cargar si quisiéramos
                // pero para simplificar usamos el del archivo
                string ruta = archivos[sel-1];
                string nombreArchivo = fs::path(ruta).filename().string();
                usuario = nombreArchivo.substr(0, nombreArchivo.find("_partida.json"));

                elJugador = new Jugador(usuario, "*****");

                // LLAMADA A LA CARGA REAL
                if (elJugador->cargarDesdeArchivo(ruta)) {
                    cout << GREEN << "¡Partida cargada correctamente!" << RESET << endl;
                    cargaExitosa = true;
                    pausa();
                } else {
                    cout << RED << "Error al leer el archivo." << RESET << endl;
                    delete elJugador; elJugador = nullptr;
                }
            }
        }

        // --- NUEVA ---
        else if (opInicio == 2) {
            cout << "Nombre: "; cin >> usuario;
            cout << "Pass: "; cin >> pass;
            elJugador = new Jugador(usuario, pass);

            // DATOS INICIALES POR DEFECTO
            Tropa* t1 = new Tropa("Escuadron Crustaceo");
            t1->agregarSoldado(new Soldado("Patrik", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Bob", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Stuart", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Kevin", true, 5, 25, 0));
            t1->agregarSoldado(new Soldado("Dave", true, 5, 25, 0));
            elJugador->getEjercito()->agregarTropa(t1);

            elJugador->reclutarSoldado("Ragnar", 4, 30, 0);
            elJugador->reclutarSoldado("Bjorn", 4, 32, 0);

            cout << GREEN << "Partida creada." << RESET << endl;
            cargaExitosa = true;
            pausa();
        }

        // --- BUCLE JUEGO ---
        if (elJugador != nullptr && cargaExitosa) {
            int opMenu = 0;
            do {
                cout << "\n=== MENU PRINCIPAL ===" << endl;
                cout << "1. Ver Ejercito\n2. Gestion (Entrenar/Crear)\n3. Inventario\n4. Batalla\n5. Guardar y Salir" << endl;
                cout << "Opcion: "; cin >> opMenu;

                switch (opMenu) {
                    case 1: elJugador->verTropasExistentes(); pausa(); break;
                    case 2: {
                        int sub = 0;
                        do {
                            cout << "\n--- GESTION ---" << endl;
                            cout << "1. Crear Tropa\n2. Borrar Tropa\n3. Entrenar\n4. Ver Reservas\n5. Volver" << endl;
                            cin >> sub;
                            if(sub==1) elJugador->crearNuevaTropa();
                            else if(sub==2) elJugador->modificarTropas();
                            else if(sub==3) elJugador->entrenarSoldados();
                            else if(sub==4) elJugador->verSoldadosDisponibles();
                            if(sub!=5) pausa();
                        } while(sub!=5);
                        break;
                    }
                    case 3: elJugador->inventarioComandante(); pausa(); break;
                    case 4: elJugador->hogueraDeBatalla(); break; // Pausa interna
                    case 5: elJugador->guardarPartida(); break;
                }
            } while (opMenu != 5);

            delete elJugador;
            elJugador = nullptr;
            cargaExitosa = false;
        }

    } while (opInicio != 0);
    return 0;
}