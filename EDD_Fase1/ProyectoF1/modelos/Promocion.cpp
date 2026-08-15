#include <iostream>

struct Promocion {
    std::string codigo;
    std::string nombre;
    std::string vigencia;
    std::string diasAplicables;

    Promocion(const std::string& c, const std::string& n, const std::string& v, const std::string& d) // Constructor
        : codigo(c), nombre(n), vigencia(v), diasAplicables(d) {}
};