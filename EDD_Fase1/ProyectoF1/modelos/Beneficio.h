#ifndef BENEFICIO_H
#define BENEFICIO_H

#include <string>


struct Beneficio {
    std::string tipoBeneficio;
    std::string descripcion;
    std::string valor;

    Beneficio(const std::string& t, const std::string& d, const std::string& v) // Constructor
        : tipoBeneficio(t), descripcion(d), valor(v) {}
};

#endif // BENEFICIO_H
