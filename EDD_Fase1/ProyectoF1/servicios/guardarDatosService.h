
#ifndef GUARDARDATOSSERVICE_H
#define GUARDARDATOSSERVICE_H

#include <string>
#include "../estructuras/ArbolBinario.h"
#include "../estructuras/ListaCircular.h"

class guardarDatosService {
private:
     

public:
    ArbolBinario arbol; //Arbolbinario para almacenar peliculas
    ListaCircular listaPromociones; //Lista circular para almacenar promociones
    guardarDatosService();
    ~guardarDatosService();

    bool guardarSolicitud(int numeroSolicitud, const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion, const std::string& fechaSolicitud, const std::string& estadoSolicitud);
    bool guardarPromocion(const std::string& codigo, const std::string& nombre, const std::string& fechaInicio, const std::string& fechaFin, const std::string& diasAplicables);
    bool cargarCSV(const std::string& ruta);
    bool guardarPelicula(const std::string& codigo, const std::string& titulo, const std::string& genero, int duracion, const std::string& clasificacion, const std::string& idioma, const std::string& fechaEstreno, const std::string& fechaFinCartelera);
    bool eliminarPelicula(const std::string& codigo);
    bool guardarBeneficioAPromocion(const std::string& codigoPromo, std::string tipo, std::string descripcion, std::string valor);
    bool guardarBeneficioAux(const std::string& codigoPromo);
};

#endif // GUARDARDATOSSERVICE_H