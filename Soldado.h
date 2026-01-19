//
// Created by Carmen Bolaños Villarejo on 19/1/26.
//

/**
 * @file Soldado.h
 * @brief Unidad básica de combate. Gestiona stats y experiencia.
 */

#ifndef SOLDADO_H
#define SOLDADO_H

#include <string>
#include <iostream>
using namespace std;

class Soldado {
private:
    string nombre;
    int nivel;
    int poderAtaque;
    int vidaActual;
    int vidaMax;
    int experiencia;
    int maxExperiencia; // Exp necesaria para el siguiente nivel
    bool esAliado;      // True = Jugador, False = Enemigo

public:
    /**
     * @brief Constructor de Soldado.
     */
    Soldado(string n, int ataque, int vida, bool aliado)
        : nombre(n), poderAtaque(ataque), vidaActual(vida), vidaMax(vida),
          experiencia(0), maxExperiencia(100), esAliado(aliado), nivel(1) {}

    // --- MÉTODOS DE LÓGICA RPG ---

    /**
     * @brief Aumenta la experiencia y gestiona la subida de nivel.
     * @param exp Cantidad de experiencia ganada.
     */
    void ganarExperiencia(int exp) {
        if (!esAliado) return; // Los enemigos no suelen subir de nivel en combates simples

        experiencia += exp;
        cout << nombre << " gana " << exp << " XP." << endl;

        // Bucle (o recursión implícita) para subir múltiples niveles si sobra mucha XP
        while (experiencia >= maxExperiencia) {
            subirNivel();
        }
    }

    /**
     * @brief Sube de nivel, resetea XP y mejora stats.
     */
    void subirNivel() {
        experiencia -= maxExperiencia;
        nivel++;
        maxExperiencia += 50; // Cada nivel cuesta más
        poderAtaque += 5;
        vidaMax += 20;
        vidaActual = vidaMax; // Curar al subir nivel
        cout << "¡" << nombre << " ha subido al Nivel " << nivel << "!" << endl;
    }

    /**
     * @brief Aplica daño al soldado.
     * @param dmg Cantidad de daño.
     */
    void recibirDanio(int dmg) {
        vidaActual -= dmg;
        if (vidaActual < 0) vidaActual = 0;
    }

    /**
     * @brief Cura al soldado (usado por Items).
     */
    void curar(int cantidad) {
        vidaActual += cantidad;
        if (vidaActual > vidaMax) vidaActual = vidaMax;
    }

    /**
     * @brief Aumenta ataque permanentemente (usado por Items).
     */
    void buffAtaque(int cantidad) {
        poderAtaque += cantidad;
    }

    // --- GETTERS Y SETTERS ---
    bool estaVivo() const { return vidaActual > 0; }
    int getPoder() const { return poderAtaque; }
    int getVida() const { return vidaActual; }
    string getNombre() const { return nombre; }
    bool getEsAliado() const { return esAliado; }

    void mostrarInfo() const {
        cout << "Soldado: " << nombre
             << " | Bando: " << (esAliado ? "Aliado" : "Enemigo")
             << " | Nivel: " << nivel
             << " | Poder: " << poderAtaque
             << " | Exp: " << experiencia << "/" << maxExperiencia << endl;
    }
};

#endif