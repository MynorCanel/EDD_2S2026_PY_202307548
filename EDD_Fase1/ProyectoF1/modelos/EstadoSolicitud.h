#ifndef ESTADOSOLICITUD_H
#define ESTADOSOLICITUD_H

#include <string>

enum class EstadoSolicitud {
    EnEspera,
    Aprobado,
    Rechazado
};

// Convierte texto libre (ej. lo que escribe el usuario en un formulario) al enum.
// Cualquier valor no reconocido se trata como "en espera" por seguridad.
inline EstadoSolicitud estadoDesdeTexto(const std::string& texto) {
    if (texto == "aprobado" || texto == "Aprobado") {
        return EstadoSolicitud::Aprobado;
    }
    if (texto == "rechazado" || texto == "Rechazado") {
        return EstadoSolicitud::Rechazado;
    }
    return EstadoSolicitud::EnEspera;
}

// Convierte el enum de vuelta a texto legible (para consola o para el grafo)
inline std::string textoDesdeEstado(EstadoSolicitud estado) {
    switch (estado) {
        case EstadoSolicitud::Aprobado:  return "Aprobado";
        case EstadoSolicitud::Rechazado: return "Rechazado";
        case EstadoSolicitud::EnEspera:
        default:                         return "En espera";
    }
}

// Color tipo semaforo para Graphviz segun el estado
inline std::string colorDesdeEstado(EstadoSolicitud estado) {
    switch (estado) {
        case EstadoSolicitud::Aprobado:  return "#2ecc71"; // verde
        case EstadoSolicitud::Rechazado: return "#e74c3c"; // rojo
        case EstadoSolicitud::EnEspera:
        default:                         return "#f1c40f"; // amarillo
    }
}

#endif // ESTADOSOLICITUD_H