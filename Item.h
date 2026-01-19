//
// Created by Carmen Bolaños Villarejo on 19/1/26.
//

/**
 * @file Item.h
 * @brief Representa un objeto consumible en el juego.
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
using namespace std;

// Enum para facilitar la lógica (Opcional, pero recomendado)
enum TipoItem { CURACION, ATAQUE_BUFF, DEFENSA_BUFF };

class Item {
private:
    string nombre;
    TipoItem tipo;
    int valor; // Cuánto cura o cuánto sube el ataque

public:
    /**
     * @brief Constructor del Item.
     * @param n Nombre del objeto (ej: "Pocion").
     * @param t Tipo de efecto.
     * @param v Valor numérico del efecto.
     */
    Item(string n, TipoItem t, int v) : nombre(n), tipo(t), valor(v) {}

    // Getters
    string getNombre() const { return nombre; }
    TipoItem getTipo() const { return tipo; }
    int getValor() const { return valor; }

    /**
     * @brief Muestra la información del item.
     */
    void mostrarInfo() const {
        string tipoStr;
        if(tipo == CURACION) tipoStr = "Curacion";
        else if(tipo == ATAQUE_BUFF) tipoStr = "Potenciador de Ataque";
        else tipoStr = "Defensa";

        cout << "[Item] " << nombre << " | Tipo: " << tipoStr << " | Potencia: " << valor << endl;
    }
};

#endif