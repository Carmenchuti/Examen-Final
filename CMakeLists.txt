cmake_minimum_required(VERSION 3.20) # He bajado de 4.0 a 3.20 porque la 4.0 aun no existe
project(Examen_Final)

set(CMAKE_CXX_STANDARD 14)

add_executable(Examen_Final
        main.cpp
        LinkedList.h
        Item.h
        Soldado.h   # Añadido: Esencial para Tropa y Jugador
        Tropa.h
        Ejercito.h
        Jugador.h)