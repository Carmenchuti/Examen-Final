

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
using namespace std;

// Enum para clasificar los tipos de objetos fácilmente
enum TipoItem { BOOST_ATAQUE, BOOST_VIDA, BOOST_EXPERIENCIA };

class Item {
private:
    string nombre;
    TipoItem tipo;
    int valor; // Cantidad que cura o sube de stat

public:
    /**
     * @brief Constructor del Item.
     */
    Item(const string& nombre, TipoItem tipo, int valor = 0)
        : nombre(nombre), tipo(tipo), valor(valor) {}

    // Getters de solo lectura
    string getNombre() const { return nombre; }
    TipoItem getTipo() const { return tipo; }
    int getValor() const { return valor; }

    // Muestra la info del item por pantalla
    void mostrarInfo() const {
        cout << "Item: " << nombre << " | Potencia: " << valor << endl;
    }
};

#endif