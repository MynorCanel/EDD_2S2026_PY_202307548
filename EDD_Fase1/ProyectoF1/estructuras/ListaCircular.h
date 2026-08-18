#ifndef LISTA_CIRCULAR_H
#define LISTA_CIRCULAR_H

#include <string>
#include <functional>
#include "../modelos/NodoPromocion.h"

class ListaCircular {
private:
    NodoPromocion* cabeza;

public:
    ListaCircular();
    ~ListaCircular();

    bool codigoExiste(const std::string& codigo);
    void insertar(std::string codigo, std::string nombre, std::string fechaInicio, std::string fechaFin, std::string diasAplicables);
    void eliminar(std::string nombre);
    
    // Método para asociar un beneficio buscando por el código de promoción
    bool agregarBeneficioAPromocion(const std::string& codigoPromo, std::string tipo, std::string descripcion, std::string valor);

    void recorrer(const std::function<void(const Promocion*, const ListaDoble&)>& funcion) const;
    
    void graficar();
};

#endif