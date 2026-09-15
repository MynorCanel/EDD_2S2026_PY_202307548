#ifndef MODELO_CLIENTE_H
#define MODELO_CLIENTE_H

#include <string>


struct Cliente {
    std::string id;
    std::string nombre;
    std::string correo;
    std::string telefono;
    std::string password;
    std::string tipo;

    Cliente(const std::string& i = "", const std::string& n = "", const std::string& c = "", const std::string& t = "", const std::string& p = "", const std::string& tp = "cliente")
        : id(i), nombre(n), correo(c), telefono(t), password(p), tipo(tp) {}
};

#endif // MODELO_CLIENTE_H
