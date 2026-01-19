#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
using namespace std;

// Definido según IMG_4494
enum TipoItem { BOOST_ATAQUE, BOOST_VIDA, BOOST_EXPERIENCIA };

class Item {
private:
    string nombre;
    TipoItem tipo;
    int valor;

public:
    // Constructor
    Item(const string& nombre, TipoItem tipo, int valor = 0)
        : nombre(nombre), tipo(tipo), valor(valor) {}

    // Getters
    string getNombre() const { return nombre; }
    TipoItem getTipo() const { return tipo; }
    int getValor() const { return valor; }

    void mostrarInfo() const {
        cout << "Item: " << nombre << " | Valor: " << valor << endl;
    }
};

#endif