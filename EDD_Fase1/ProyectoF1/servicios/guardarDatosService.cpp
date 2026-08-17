#include "guardarDatosService.h"
#include <iostream>

guardarDatosService::guardarDatosService() {
    // El árbol se inicializa automáticamente
}

guardarDatosService::~guardarDatosService() {
    // El árbol se destruye automáticamente
}

bool guardarDatosService::guardarPelicula(const std::string& codigo, const std::string& titulo, const std::string& genero, int duracion, const std::string& clasificacion, const std::string& idioma, const std::string& fechaEstreno, const std::string& fechaFinCartelera) {
    // Inserta la película en el árbol persistente
    if (arbol.CodigoExiste(codigo)) {
        std::cout << "Error: Ya existe una película con el código " << codigo << std::endl;
        return false;
    }
    arbol.insertar(codigo, titulo, genero, duracion, clasificacion, idioma, fechaEstreno, fechaFinCartelera);
    // Genera la visualización del árbol completo
    arbol.generarDot();
    std::cout << "Película guardada correctamente: " << titulo << std::endl;
    return true;
}

bool guardarDatosService::cargarCSV(const std::string& ruta) {
    if (arbol.cargarCSV(ruta)){
        arbol.generarDot();
        return true;
    } else {
        std::cout << "Error al cargar el archivo CSV: " << ruta << std::endl;
        return false;
    }
}

bool guardarDatosService::eliminarPelicula(const std::string& codigo) {
    if (!arbol.CodigoExiste(codigo)) {
        //No se encuentra la pelicula
        return false;
    }
    arbol.eliminarConCodigo(codigo);
    arbol.generarDot();

    return true;
}
        

  
