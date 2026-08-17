
#ifndef GUARDARDATOSSERVICE_H
#define GUARDARDATOSSERVICE_H

#include <string>
#include "../estructuras/ArbolBinario.h"

class guardarDatosService {
private:
     // Árbol binario persistente para almacenar todas las películas

public:
    ArbolBinario arbol; 
    guardarDatosService();
    ~guardarDatosService();

    bool guardarSolicitud(int numeroSolicitud, const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion, const std::string& fechaSolicitud, const std::string& estadoSolicitud);
    void guardarPromocion(const std::string& codigo, const std::string& nombre, const std::string& vigencia, const std::string& diasAplicables);
    bool cargarCSV(const std::string& ruta);
    bool guardarPelicula(const std::string& codigo, const std::string& titulo, const std::string& genero, int duracion, const std::string& clasificacion, const std::string& idioma, const std::string& fechaEstreno, const std::string& fechaFinCartelera);
    bool eliminarPelicula(const std::string& codigo);
};

#endif // GUARDARDATOSSERVICE_H