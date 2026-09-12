#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include <string>
#include <fstream>
#include <functional>
#include "../modelos/NodoBeneficio.h"

class ListaDoble {
private:
    NodoBeneficio* cabeza;
    static std::string escaparHtml(const std::string& texto);

public:
    ListaDoble();
    ~ListaDoble();

    void agregar(std::string tipo, std::string descripcion, std::string valor);
    void eliminar(std::string tipo);
    void mostrar() const;
    bool estaVacia() const;

    void recorrer(const std::function<void(const Beneficio*)>& funcion) const;

    // Genera la sub-graficación de los beneficios para Graphviz
    void generarDotUnificado(std::ofstream& archivo, const std::string& idPadre, const std::string& codigoPromocion) const;
};

#endif