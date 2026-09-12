#ifndef NODOPELICULA_H
#define NODOPELICULA_H
 
#include "Pelicula.h"
 
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
 
#endif // NODOPELICULA_H
 