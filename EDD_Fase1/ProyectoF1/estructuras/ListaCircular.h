#ifndef LISTACIRCULAR_H
#define LISTACIRCULAR_H

#include "../modelos/NodoPromocion.h"

class ListaCircular {
private:
    NodoPromocion* cabeza;

public:
    ListaCircular();
    ~ListaCircular();
    void insertar(std::string codigo, std::string nombre, std::string vigencia, std::string diasAplicables);
    void eliminar(std::string nombre);
    void graficar();
};

#endif // LISTACIRCULAR_H

