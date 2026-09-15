#ifndef NODOCLIENTE_H
#define NODOCLIENTE_H

#include "Cliente.h"

// Referencia enlazada preparada para conectar las reservas con TablaHash.
struct NodoReservaCliente {
    std::string codigoReserva;
    NodoReservaCliente* siguiente;

    explicit NodoReservaCliente(const std::string& codigo)
        : codigoReserva(codigo), siguiente(nullptr) {}
};

struct NodoCliente {
    Cliente* primero;
    Cliente* segundo;
    Cliente* tercero;
    NodoCliente* hijoIzquierdo;
    NodoCliente* hijoCentroIzquierdo;
    NodoCliente* hijoCentroDerecho;
    NodoCliente* hijoDerecho;
    bool hoja;
    int cantidad;
    NodoReservaCliente* reservas;

    explicit NodoCliente(Cliente* cliente = nullptr)
        : primero(cliente), segundo(nullptr), tercero(nullptr),
          hijoIzquierdo(nullptr), hijoCentroIzquierdo(nullptr),
          hijoCentroDerecho(nullptr), hijoDerecho(nullptr),
          hoja(true), cantidad(cliente == nullptr ? 0 : 1), reservas(nullptr) {}

    ~NodoCliente() {
        delete primero;
        delete segundo;
        delete tercero;
        while (reservas != nullptr) {
            NodoReservaCliente* siguiente = reservas->siguiente;
            delete reservas;
            reservas = siguiente;
        }
    }
};

#endif