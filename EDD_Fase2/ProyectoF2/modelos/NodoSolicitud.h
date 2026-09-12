#ifndef NODOSOLICITUD_H
#define NODOSOLICITUD_H


#include <string>
#include "Solicitud.h"

struct NodoSolicitud {
    Solicitud* solicitud;
    NodoSolicitud* siguiente;
    NodoSolicitud* anterior;

    NodoSolicitud(Solicitud* s) // Constructor
        : solicitud(s), siguiente(nullptr), anterior(nullptr) {}
    
    ~NodoSolicitud() {
        delete solicitud;
    }
};

#endif // NODOSOLICITUD_H