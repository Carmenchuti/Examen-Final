

#ifndef SOLDADO_H
#define SOLDADO_H

#include "Item.h"
#include <string>
#include <iostream>

using namespace std;

// Macros de colores para la consola
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
    /**
     * @brief Constructor. Calcula la experiencia necesaria basándose en el nivel.
     */
    Soldado(const string& nombre = "", bool esAliado = true, int nivel = 1, int poder = 10, int expBase = 0)
        : nombre(nombre), esAliado(esAliado), nivel(nivel), poderCombate(poder),
          vida(100), experienciaActual(expBase)
    {
        experienciaParaNivel = 100 * nivel; // Fórmula de escalado
    }

    // Getters básicos
    string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }
    int getNivel() const { return nivel; }
    int getPoderCombate() const { return poderCombate; }

    /**
     * @brief Añade experiencia y comprueba si sube de nivel.
     * @param exp Cantidad ganada.
     */
    void ganarExperiencia(int exp) {
        experienciaActual += exp;
        cout << "  -> " << nombre << " gana " << exp << " XP. (Total: " << experienciaActual << "/" << experienciaParaNivel << ")" << endl;

        // Bucle por si sube varios niveles de golpe
        while (experienciaActual >= experienciaParaNivel) {
            subirNivel();
        }
    }

    /**
     * @brief Sube nivel y mejora estadísticas.
     */
    void subirNivel() {
        experienciaActual -= experienciaParaNivel;
        nivel++;
        poderCombate += 5; // +5 Ataque por nivel
        vida += 20;        // +20 Vida por nivel
        experienciaParaNivel = 100 * nivel; // El siguiente cuesta más

        cout << YELLOW << "     ¡" << nombre << " HA SUBIDO A NIVEL " << nivel << "! (+Poder, +Vida)" << RESET << endl;
    }

    /**
     * @brief Aplica el efecto de un Item sobre el soldado.
     */
    void aplicarItem(const Item& item) {
        if (item.getTipo() == BOOST_ATAQUE) {
            poderCombate += item.getValor();
            cout << "  -> " << nombre << " afila su espada (+" << item.getValor() << " Ataque)." << endl;
        }
        else if (item.getTipo() == BOOST_VIDA) {
            vida += item.getValor();
            cout << "  -> " << nombre << " bebe pocion (+" << item.getValor() << " Vida)." << endl;
        }
        else if (item.getTipo() == BOOST_EXPERIENCIA) {
            ganarExperiencia(item.getValor());
        }
    }

    /**
     * @brief Imprime la ficha del soldado formateada como en la demo.
     */
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