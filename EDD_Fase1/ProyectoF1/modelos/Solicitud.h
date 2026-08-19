#ifndef SOLICITUD_H
#define SOLICITUD_H

#include <string>
#include "EstadoSolicitud.h"

struct Solicitud {
    int numeroSolicitud;
    std::string nombreCliente;
    std::string telefonoContacto;
    std::string tipoSolicitud;
    std::string descripcion;
    std::string fechaSolicitud;
    EstadoSolicitud estadoSolicitud;

    Solicitud(int t, const std::string& n, const std::string& tel, const std::string& ts, const std::string& d, const std::string& f, EstadoSolicitud e) // Constructor
        : numeroSolicitud(t), nombreCliente(n), telefonoContacto(tel), tipoSolicitud(ts), descripcion(d), fechaSolicitud(f), estadoSolicitud(e) {}

    Solicitud(int t, const std::string& n, const std::string& tel, const std::string& ts, const std::string& d, const std::string& f, const std::string& e) // Constructor alterno desde texto
        : numeroSolicitud(t), nombreCliente(n), telefonoContacto(tel), tipoSolicitud(ts), descripcion(d), fechaSolicitud(f), estadoSolicitud(estadoDesdeTexto(e)) {}
};

#endif // SOLICITUD_H