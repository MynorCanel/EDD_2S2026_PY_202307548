#ifndef NODOFUNCION_H
#define NODOFUNCION_H


#include <string>
#include "../estructuras/MatrizCine.h"

struct NodoFuncion {
    MatrizCine* funcion;
    int altura; // Altura del nodo para el balanceo del árbol AVL
    NodoFuncion* izquierdo;
    NodoFuncion* derecho;

    NodoFuncion(MatrizCine* f, int h) // Constructor
        : funcion(f), altura(h), izquierdo(nullptr), derecho(nullptr) {}
    
    ~NodoFuncion() {
        delete funcion;
    }
};

#endif // NODOFUNCION_H