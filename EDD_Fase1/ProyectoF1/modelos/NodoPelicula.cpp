#include <iostream>
#include "Pelicula.cpp"
//#include "Pelicula.h"

struct NodoPelicula {
    Pelicula* pelicula;
    NodoPelicula* izq;
    NodoPelicula* der;

    NodoPelicula(Pelicula* p) // Constructor
        : pelicula(p), izq(nullptr), der(nullptr) {}

    ~NodoPelicula() {
        delete pelicula;
    }
    
};


