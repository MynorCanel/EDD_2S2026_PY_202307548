#ifndef NODOBENEFICIO_H
#define NODOBENEFICIO_H


#include <string>
#include "Beneficio.h"

struct NodoBeneficio {
    Beneficio* beneficio;
    NodoBeneficio* siguiente;
    NodoBeneficio* anterior;

    NodoBeneficio(Beneficio* b) // Constructor
        : beneficio(b), siguiente(nullptr), anterior(nullptr) {}
    
    ~NodoBeneficio() {
        delete beneficio;
    }
};

#endif // NODOBENEFICIO_H