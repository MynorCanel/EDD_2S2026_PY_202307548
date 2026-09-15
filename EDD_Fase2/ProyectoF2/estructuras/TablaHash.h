#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <functional>
#include <fstream>
#include <string>
#include "../modelos/NodoReserva.h"

class TablaHash {
private:
    struct Cubeta {
        int indice;
        NodoReserva* reservas;
        Cubeta* siguiente;

        explicit Cubeta(int numero)
            : indice(numero), reservas(nullptr), siguiente(nullptr) {}
    };

    Cubeta* cubetas;
    int capacidad;
    int cantidad;
    int colisiones;

    unsigned int hash(const std::string& clave) const;
    Cubeta* obtenerCubeta(int indice) const;
    void destruir();
    void graficarCubeta(std::ofstream& archivo, int indice, NodoReserva* nodo) const;

public:
    explicit TablaHash(int capacidadInicial = 17);
    ~TablaHash();
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    bool insertar(Reserva* reserva);
    bool insertar(const Reserva& reserva);
    Reserva* buscar(const std::string& codigoReserva) const;
    bool eliminar(const std::string& codigoReserva);
    bool estaVacia() const;
    int obtenerCapacidad() const;
    int obtenerCantidad() const;
    int obtenerCubetasOcupadas() const;
    int obtenerColisiones() const;
    void recorrer(const std::function<void(const Reserva&)>& funcion) const;
    void generarDot() const;
};

#endif
