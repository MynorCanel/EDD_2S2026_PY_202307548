#include "autentificacionService.h"
#include "guardarDatosService.h"
#include "../modelos/Cliente.h"

AutentificacionService::AutentificacionService(guardarDatosService& servicio)
    : guardar(servicio) {}

Cliente* AutentificacionService::autenticar(const std::string& correo, const std::string& password) const
{
    Cliente* encontrado = nullptr;
    guardar.arbolClientes.recorrer([&](Cliente* cliente) {
        if (encontrado == nullptr && cliente->correo == correo && cliente->password == password) encontrado = cliente;
    });
    return encontrado;
}

bool AutentificacionService::registrar(const std::string& nombre, const std::string& correo, const std::string& telefono, const std::string& password, std::string& idAsignado)
{
    int siguiente = 1;
    do {
        idAsignado = std::string("U") + (siguiente < 10 ? "00" : siguiente < 100 ? "0" : "") + std::to_string(siguiente++);
    } while (guardar.arbolClientes.existe(idAsignado));
    return guardar.guardarCliente(idAsignado, nombre, correo, telefono, password);
}
