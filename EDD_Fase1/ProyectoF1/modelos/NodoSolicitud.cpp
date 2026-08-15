#include <iostream>
#include "Solicitud.cpp"

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
