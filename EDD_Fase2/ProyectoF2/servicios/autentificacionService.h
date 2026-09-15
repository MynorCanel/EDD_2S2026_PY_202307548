#ifndef AUTENTIFICACIONSERVICE_H
#define AUTENTIFICACIONSERVICE_H

#include <string>

class guardarDatosService;
struct Cliente;

class AutentificacionService {
private:
    guardarDatosService& guardar;

public:
    explicit AutentificacionService(guardarDatosService& servicio);
    Cliente* autenticar(const std::string& correo, const std::string& password) const;
    bool registrar(const std::string& nombre, const std::string& correo, const std::string& telefono, const std::string& password, std::string& idAsignado);
};

#endif
