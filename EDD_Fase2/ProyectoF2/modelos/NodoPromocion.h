
#ifndef NODO_PROMOCION_H
#define NODO_PROMOCION_H

#include "Promocion.h"
#include "../estructuras/ListaDoble.h"

struct NodoPromocion {
    Promocion* promocion;
    ListaDoble listaBeneficios; // Instancia de la Lista Doble asociada a este nodo
    NodoPromocion* siguiente;

    NodoPromocion(Promocion* p) : promocion(p), siguiente(nullptr) {}
};

#endif