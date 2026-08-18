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

bool guardarDatosService::guardarPromocion(const std::string& codigo, const std::string& nombre, const std::string& fechaInicio, const std::string& fechaFin, const std::string& diasAplicables) {
    // Inserta la promoción en la lista circular
    if (listaPromociones.codigoExiste(codigo)) {
        std::cout << "Error: Ya existe una promoción con el código " << codigo << std::endl;
        return false;
    }
    listaPromociones.insertar(codigo, nombre, fechaInicio, fechaFin, diasAplicables);
    // Genera la visualización de la lista completa
    listaPromociones.graficar();
    std::cout << "Promoción guardada correctamente: " << nombre << std::endl;
    return true;
}



bool guardarDatosService::guardarBeneficioAux(const std::string& codigoPromo) {
    if (!listaPromociones.codigoExiste(codigoPromo)) {
        std::cout << "Error: No existe una promoción con el código " << codigoPromo << std::endl;
        return false;
    }
    return true;
}


bool guardarDatosService::guardarBeneficioAPromocion(const std::string& codigoPromo, std::string tipo, std::string descripcion, std::string valor) {
    // Agrega un beneficio a la promoción correspondiente
    if (!listaPromociones.codigoExiste(codigoPromo)) {
        std::cout << "Error: No existe una promoción con el código " << codigoPromo << std::endl;
        return false;
    }
    if (listaPromociones.agregarBeneficioAPromocion(codigoPromo, tipo, descripcion, valor)) {
        // Genera la visualización de la lista completa
        listaPromociones.graficar();
        std::cout << "Beneficio agregado correctamente a la promoción: " << codigoPromo << std::endl;
        return true;
    } else {
        std::cout << "Error al agregar el beneficio a la promoción: " << codigoPromo << std::endl;
        return false;
    }
}
        

  
