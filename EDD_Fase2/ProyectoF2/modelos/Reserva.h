#ifndef RESERVA_H
#define RESERVA_H

#include <string>

struct Reserva {
    std::string codigoReserva;
    std::string idCliente;
    std::string codigoFuncion;
    int fila;
    int columna;
    std::string fechaReserva;

    Reserva(const std::string& codigo = "", const std::string& cliente = "", const std::string& funcion = "", int filaReserva = 0, int columnaReserva = 0, const std::string& fecha = "")
        : codigoReserva(codigo), idCliente(cliente), codigoFuncion(funcion), fila(filaReserva), columna(columnaReserva), fechaReserva(fecha) {}
};

#endif
