#include <iostream>
#include "Beneficio.cpp"

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