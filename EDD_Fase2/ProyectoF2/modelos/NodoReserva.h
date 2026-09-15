#ifndef NODORESERVA_H
#define NODORESERVA_H

#include "Reserva.h"

struct NodoReserva {
    Reserva* reserva;
    NodoReserva* siguiente;

    explicit NodoReserva(Reserva* nuevaReserva)
        : reserva(nuevaReserva), siguiente(nullptr) {}

    ~NodoReserva() {
        delete reserva;
    }
};

#endif
