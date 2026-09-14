
#ifndef GUARDARDATOSSERVICE_H
#define GUARDARDATOSSERVICE_H

#include <string>
#include "../estructuras/ArbolBinario.h"
#include "../estructuras/ListaCircular.h"
#include "../estructuras/MatrizCine.h"
#include "../estructuras/ListaCircularDoble.h"
#include "../estructuras/ArbolAVL.h"

class guardarDatosService {
private:
    std::string tituloPelicula; 
     

public:
    ArbolBinario arbol; //Arbolbinario para almacenar peliculas
    ListaCircular listaPromociones; //Lista circular para almacenar promociones
    ArbolAVL arbolFunciones; //AVL de funciones, cada nodo posee su matriz
    ListaCircularDoble listaSolicitudes; // Lista circular doble para almacenar solicitudes
    guardarDatosService();
    ~guardarDatosService();

    void guardarSolicitud(const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion);
    bool guardarPromocion(const std::string& codigo, const std::string& nombre, const std::string& fechaInicio, const std::string& fechaFin, const std::string& diasAplicables);
    bool cargarCSV(const std::string& ruta);
    bool guardarPelicula(const std::string& codigo, const std::string& titulo, const std::string& genero, int duracion, const std::string& clasificacion, const std::string& idioma, const std::string& fechaEstreno, const std::string& fechaFinCartelera);
    bool eliminarPelicula(const std::string& codigo);
    bool guardarBeneficioAPromocion(const std::string& codigoPromo, std::string tipo, std::string descripcion, std::string valor);
    bool guardarBeneficioAux(const std::string& codigoPromo);
    bool reservarAsiento(const std::string& nombreCliente, const std::string& fila, const std::string& columna);
    bool crearFuncion(const std::string& codigoPelicula, int filas, int columnas, const std::string& horario, const std::string& sala);
    void graficarReportes();
    bool cargarJSONPeliculas(const std::string& ruta);
    bool crearFuncion(const std::string& codigoFuncion, const std::string& codigoPelicula, int filas, int columnas, const std::string& horario, const std::string& sala, const std::string& archivoAsientos = "");
    bool reservarAsientoFuncion(const std::string& codigoFuncion, const std::string& nombreCliente, const std::string& fila, const std::string& columna);
    void cargarAsientos(const std::string& codigoFuncion, const std::string& archivoAsientos = "");
};

#endif // GUARDARDATOSSERVICE_H