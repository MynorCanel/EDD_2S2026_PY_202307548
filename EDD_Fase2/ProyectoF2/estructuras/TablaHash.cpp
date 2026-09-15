#include "TablaHash.h"
#include "../servicios/rutasReportes.h"
#include <sstream>

TablaHash::TablaHash(int capacidadInicial)
    : cubetas(nullptr), capacidad(capacidadInicial < 3 ? 3 : capacidadInicial), cantidad(0), colisiones(0) {}

TablaHash::~TablaHash() { destruir(); }

unsigned int TablaHash::hash(const std::string& clave) const
{
    unsigned int resultado = 0;
    for (char caracter : clave) resultado = resultado * 31u + static_cast<unsigned char>(caracter);
    return resultado % static_cast<unsigned int>(capacidad);
}

TablaHash::Cubeta* TablaHash::obtenerCubeta(int indice) const
{
    Cubeta* actual = cubetas;
    while (actual != nullptr) {
        if (actual->indice == indice) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHash::insertar(Reserva* reserva)
{
    if (reserva == nullptr || reserva->codigoReserva.empty() || buscar(reserva->codigoReserva) != nullptr) return false;
    const int indice = static_cast<int>(hash(reserva->codigoReserva));
    Cubeta* cubeta = obtenerCubeta(indice);
    if (cubeta == nullptr) {
        cubeta = new Cubeta(indice);
        cubeta->siguiente = cubetas;
        cubetas = cubeta;
    } else if (cubeta->reservas != nullptr) {
        ++colisiones;
    }
    NodoReserva* nodo = new NodoReserva(reserva);
    nodo->siguiente = cubeta->reservas;
    cubeta->reservas = nodo;
    ++cantidad;
    return true;
}

bool TablaHash::insertar(const Reserva& reserva) { return insertar(new Reserva(reserva)); }

Reserva* TablaHash::buscar(const std::string& codigoReserva) const
{
    Cubeta* cubeta = obtenerCubeta(static_cast<int>(hash(codigoReserva)));
    if (cubeta == nullptr) return nullptr;
    NodoReserva* actual = cubeta->reservas;
    while (actual != nullptr) {
        if (actual->reserva->codigoReserva == codigoReserva) return actual->reserva;
        actual = actual->siguiente;
    }
    return nullptr;
}

bool TablaHash::eliminar(const std::string& codigoReserva)
{
    Cubeta* cubeta = obtenerCubeta(static_cast<int>(hash(codigoReserva)));
    if (cubeta == nullptr) return false;
    NodoReserva* anterior = nullptr;
    NodoReserva* actual = cubeta->reservas;
    while (actual != nullptr && actual->reserva->codigoReserva != codigoReserva) {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (actual == nullptr) return false;
    if (anterior == nullptr) cubeta->reservas = actual->siguiente;
    else anterior->siguiente = actual->siguiente;
    delete actual;
    --cantidad;
    return true;
}

bool TablaHash::estaVacia() const { return cantidad == 0; }
int TablaHash::obtenerCapacidad() const { return capacidad; }
int TablaHash::obtenerCantidad() const { return cantidad; }
int TablaHash::obtenerColisiones() const { return colisiones; }

int TablaHash::obtenerCubetasOcupadas() const
{
    int ocupadas = 0;
    Cubeta* cubeta = cubetas;
    while (cubeta != nullptr) {
        if (cubeta->reservas != nullptr) ++ocupadas;
        cubeta = cubeta->siguiente;
    }
    return ocupadas;
}

void TablaHash::recorrer(const std::function<void(const Reserva&)>& funcion) const
{
    Cubeta* cubeta = cubetas;
    while (cubeta != nullptr) {
        NodoReserva* reserva = cubeta->reservas;
        while (reserva != nullptr) {
            funcion(*reserva->reserva);
            reserva = reserva->siguiente;
        }
        cubeta = cubeta->siguiente;
    }
}

void TablaHash::destruir()
{
    while (cubetas != nullptr) {
        Cubeta* siguienteCubeta = cubetas->siguiente;
        NodoReserva* reserva = cubetas->reservas;
        while (reserva != nullptr) {
            NodoReserva* siguienteReserva = reserva->siguiente;
            delete reserva;
            reserva = siguienteReserva;
        }
        delete cubetas;
        cubetas = siguienteCubeta;
    }
}

void TablaHash::graficarCubeta(std::ofstream& archivo, int indice, NodoReserva* nodo) const
{
    const std::string bucket = "bucket" + std::to_string(indice);
    archivo << "{ rank=same; ";
    archivo << bucket << " [shape=box, style=filled, fillcolor=\"" << (nodo == nullptr ? "#e5e7eb" : "#f8c471") << "\", label=\"Bucket " << indice << "\"];\n";
    if (nodo == nullptr) {
        archivo << "}\n";
        return;
    }
    int consecutivo = 0;
    while (nodo != nullptr) {
        const std::string id = "reserva" + std::to_string(indice) + "_" + std::to_string(consecutivo++);
        archivo << id << " [shape=box, style=filled, fillcolor=\"#ffffff\", label=\""
                << nodo->reserva->codigoReserva << "\\nCliente: " << nodo->reserva->idCliente
                << "\\nFuncion: " << nodo->reserva->codigoFuncion << "\"];\n";
        if (consecutivo == 1) {
            archivo << bucket << " -> " << id << " [constraint=false];\n";
        } else {
            archivo << "reserva" << indice << "_" << (consecutivo - 2) << " -> " << id << " [constraint=false];\n";
        }
        nodo = nodo->siguiente;
    }
    archivo << "}\n";
}

void TablaHash::generarDot() const
{
    const std::string directorio = rutasReportes::directorio();
    const std::string rutaDot = directorio + "/tabla_hash_reservas.dot";
    const std::string rutaPng = directorio + "/tabla_hash_reservas.png";
    std::ofstream archivo(rutaDot);
    archivo << "digraph TablaHashReservas {\nrankdir=TB;\nsplines=ortho;\nnodesep=0.45;\nranksep=0.35;\nnode [fontname=Arial, color=\"#374151\"];\n";
    archivo << "label=\"Tabla Hash de Reservas\\nTamano: " << capacidad
            << " | Reservas: " << cantidad << " | Cubetas ocupadas: " << obtenerCubetasOcupadas()
            << " | Colisiones: " << colisiones << "\";\nlabelloc=t;\n";
    int indice = capacidad - 1;
    while (indice >= 0) {
        Cubeta* cubeta = obtenerCubeta(indice);
        graficarCubeta(archivo, indice, cubeta == nullptr ? nullptr : cubeta->reservas);
        if (indice > 0) {
            archivo << "bucket" << indice << " -> bucket" << (indice - 1) << " [style=invis, weight=100];\n";
        }
        --indice;
    }
    archivo << "}\n";
    archivo.close();
    rutasReportes::convertirAPng(rutaDot, rutaPng);
}
