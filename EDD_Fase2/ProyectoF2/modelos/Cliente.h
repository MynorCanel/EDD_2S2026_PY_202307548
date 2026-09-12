#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>


struct Cliente {
    std::string id;
    std::string nombre;
    private:
    std::string correo;
    std::string telefono;
    std::string password;

    Cliente(const std::string& i, const std::string& n, const std::string& c, const std::string& t, const std::string& p) // Constructor
        : id(i), nombre(n), correo(c), telefono(t), password(p) {}
};

#endif // CLIENTE_H
