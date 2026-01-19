#ifndef SOLDADO_H
#define SOLDADO_H

#include "Item.h"
#include <string>
#include <iostream>
using namespace std;

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
    Soldado(const string& nombre = "", bool esAliado = true, int nivel = 1, int poder = 10, int expBase = 0)
        : nombre(nombre), esAliado(esAliado), nivel(nivel), poderCombate(poder),
          vida(100), experienciaActual(expBase), experienciaParaNivel(100 * nivel) {}

    string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }
    int getNivel() const { return nivel; }
    int getPoderCombate() const { return poderCombate; }
    int getExperienciaActual() const { return experienciaActual; }
    int getExperienciaParaNivel() const { return experienciaParaNivel; }

    void ganarExperiencia(int exp) {
        experienciaActual += exp;
        // Mensaje exacto de la captura
        cout << "- " << nombre << " gano " << exp << " EXP" << endl;

        if (experienciaActual >= experienciaParaNivel) {
            subirNivel();
            // Si sobra mucha exp, recursión para seguir subiendo
            if (experienciaActual >= experienciaParaNivel) ganarExperiencia(0);
        }
    }

    void subirNivel() {
        experienciaActual -= experienciaParaNivel;
        nivel++;
        poderCombate += 5;
        vida += 20;
        experienciaParaNivel = 100 * nivel;
        // Formato visual de la captura (Amarillo simulado o destacado)
        cout << "  \033[1;33m¡SUBIO AL NIVEL " << nivel << "!\033[0m" << endl;
    }

    void aplicarItem(const Item& item) {
        if (item.getTipo() == BOOST_ATAQUE) poderCombate += item.getValor();
        else if (item.getTipo() == BOOST_VIDA) vida += item.getValor();
        else if (item.getTipo() == BOOST_EXPERIENCIA) ganarExperiencia(item.getValor());
    }

    void recibirDanio(int danio) {
        vida -= danio;
        if (vida < 0) vida = 0;
    }

    bool estaVivo() const { return vida > 0; }

    // FORMATO EXACTO DE LA CAPTURA
    // Ejemplo: [0] Soldado: Patrik | Bando: Aliado | Nivel: 5 | Poder: 25 | Exp: 0/500
    void mostrarInfo(int index) const {
        cout << "   [" << index << "] Soldado: " << nombre
             << " | Bando: " << (esAliado ? "Aliado" : "Enemigo")
             << " | Nivel: " << nivel
             << " | Poder: " << poderCombate
             << " | Exp: " << experienciaActual << "/" << experienciaParaNivel << endl;
    }
};

#endif