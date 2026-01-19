#ifndef SOLDADO_H
#define SOLDADO_H

#include "Item.h"
#include <string>
#include <iostream>

using namespace std;

// Definimos colores también aquí por si acaso
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

class Soldado {
private:
    string nombre;
    bool esAliado;
    int nivel;
    int poderCombate;
    int vida;
    int experienciaActual;
    int experienciaParaNivel;

public:
    // Constructor completo
    Soldado(const string& nombre = "", bool esAliado = true, int nivel = 1, int poder = 10, int expBase = 0)
        : nombre(nombre), esAliado(esAliado), nivel(nivel), poderCombate(poder),
          vida(100), experienciaActual(expBase)
    {
        // La experiencia necesaria crece con el nivel (Nivel 1 = 100, Nivel 5 = 500)
        experienciaParaNivel = 100 * nivel;
    }

    string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }
    int getNivel() const { return nivel; }
    int getPoderCombate() const { return poderCombate; }

    // --- LÓGICA DE EXPERIENCIA ---
    void ganarExperiencia(int exp) {
        experienciaActual += exp;

        // Mensaje visual para confirmar que reciben XP
        cout << "  -> " << nombre << " gana " << exp << " XP. (Total: " << experienciaActual << "/" << experienciaParaNivel << ")" << endl;

        // Subida de nivel (Bucle por si sube varios niveles de golpe)
        while (experienciaActual >= experienciaParaNivel) {
            subirNivel();
        }
    }

    void subirNivel() {
        experienciaActual -= experienciaParaNivel; // Se resta la XP gastada (ej: tenias 120, gastas 100, te quedan 20)
        nivel++;
        poderCombate += 5; // Sube stats
        vida += 20;
        experienciaParaNivel = 100 * nivel; // El siguiente nivel cuesta más

        cout << YELLOW << "     ¡" << nombre << " HA SUBIDO A NIVEL " << nivel << "! (+Poder, +Vida)" << RESET << endl;
    }

    void aplicarItem(const Item& item) {
        if (item.getTipo() == BOOST_ATAQUE) {
            poderCombate += item.getValor();
            cout << "  -> " << nombre << " aumenta su ataque en " << item.getValor() << "." << endl;
        }
        else if (item.getTipo() == BOOST_VIDA) {
            vida += item.getValor();
            cout << "  -> " << nombre << " recupera " << item.getValor() << " HP." << endl;
        }
        else if (item.getTipo() == BOOST_EXPERIENCIA) {
            ganarExperiencia(item.getValor());
        }
    }

    // --- MOSTRAR INFO (ESTO ES LO QUE VEIAS MAL) ---
    // Me aseguro de imprimir 'experienciaActual' actualizado
    void mostrarInfo(int index) const {
        cout << "   [" << index << "] Soldado: " << nombre
             << " | Bando: " << (esAliado ? "Aliado" : "Enemigo")
             << " | Nivel: " << nivel
             << " | Poder: " << poderCombate
             << " | Exp: " << experienciaActual << "/" << experienciaParaNivel << endl;
    }

    bool estaVivo() const { return vida > 0; }
    void recibirDanio(int dmg) { vida -= dmg; if(vida<0) vida=0; }
};

#endif