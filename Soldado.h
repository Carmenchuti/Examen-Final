
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
    // Constructor según la foto IMG_4495
    Soldado(const string& nombre = "", bool esAliado = true, int nivel = 1, int poder = 10)
        : nombre(nombre), esAliado(esAliado), nivel(nivel), poderCombate(poder),
          vida(100), experienciaActual(0), experienciaParaNivel(100) {}

    // Getters const
    string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }
    int getNivel() const { return nivel; }
    int getPoderCombate() const { return poderCombate; }
    int getExperienciaActual() const { return experienciaActual; }
    int getExperienciaParaNivel() const { return experienciaParaNivel; }

    // Setters
    void setNivel(int n) { nivel = n; }
    void setPoderCombate(int p) { poderCombate = p; }

    // Lógica RPG
    void ganarExperiencia(int exp) {
        experienciaActual += exp;
        // Lógica recursiva para subir de nivel si sobra mucha exp
        if (experienciaActual >= experienciaParaNivel) {
            subirNivel();
            // Si sigue sobrando exp, llamada recursiva (opcional)
            if (experienciaActual >= experienciaParaNivel) ganarExperiencia(0);
        }
    }

    void subirNivel() {
        experienciaActual -= experienciaParaNivel;
        nivel++;
        poderCombate += 5;
        vida += 20;
        experienciaParaNivel += 50;
        cout << nombre << " sube a nivel " << nivel << "!" << endl;
    }

    // IMPRESCINDIBLE SEGÚN FOTO
    void aplicarItem(const Item& item) {
        if (item.getTipo() == BOOST_ATAQUE) {
            poderCombate += item.getValor();
        } else if (item.getTipo() == BOOST_VIDA) {
            vida += item.getValor();
        } else if (item.getTipo() == BOOST_EXPERIENCIA) {
            ganarExperiencia(item.getValor());
        }
    }

    void recibirDanio(int danio) {
        vida -= danio;
        if (vida < 0) vida = 0;
    }

    bool estaVivo() const { return vida > 0; }

    void mostrarInfo() const {
        cout << "Soldado: " << nombre << " | Nvl: " << nivel
             << " | Poder: " << poderCombate << " | Vida: " << vida << endl;
    }
};

#endif