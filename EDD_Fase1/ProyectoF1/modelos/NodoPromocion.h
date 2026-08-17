#ifndef NODOPROMOCION_H
#define NODOPROMOCION_H

#include <string>
#include "Promocion.h" 
#include "../estructuras/listadobleenlazada.cpp"



struct NodoPromocion {
    Promocion* promocion;
    ListaDoble beneficios; // Puntero a la lista doblemente enlazada de beneficios
    NodoPromocion* siguiente;

    NodoPromocion(Promocion* p) // Constructor
        : promocion(p), siguiente(nullptr) {}
    
    ~NodoPromocion() {
        delete promocion;
        // No es necesario borrar la lista de beneficios, ya que se elimina automáticamente con el destructor de ListaDoble
    }

};

#endif // NODOPROMOCION_H