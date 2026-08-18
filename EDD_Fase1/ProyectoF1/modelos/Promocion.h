#ifndef PROMOCION_H
#define PROMOCION_H

#include <string>

struct Promocion {
    std::string codigo;
    std::string nombre;
    std::string fechaInicio;
    std::string fechaFin;
    std::string diasAplicables;



    Promocion(const std::string& c, const std::string& n, const std::string& i, const std::string& f, const std::string& d) // Constructor
        : codigo(c), nombre(n), fechaInicio(i), fechaFin(f), diasAplicables(d) {}
};

#endif // PROMOCION_H