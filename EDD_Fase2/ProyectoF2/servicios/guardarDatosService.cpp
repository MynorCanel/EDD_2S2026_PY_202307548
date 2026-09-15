#include "guardarDatosService.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include "json.hpp" // Incluye la biblioteca nlohmann/json para manejar JSON
#include "rutasReportes.h"

using json = nlohmann::json;

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
    std::cout << "Película guardada correctamente: " << titulo << std::endl;
    return true;
}

bool guardarDatosService::cargarCSV(const std::string& ruta) {
    if (arbol.cargarCSV(ruta)){
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

    return true;
}

bool guardarDatosService::guardarPromocion(const std::string& codigo, const std::string& nombre, const std::string& fechaInicio, const std::string& fechaFin, const std::string& diasAplicables) {
    // Inserta la promoción en la lista circular
    if (listaPromociones.codigoExiste(codigo)) {
        std::cout << "Error: Ya existe una promoción con el código " << codigo << std::endl;
        return false;
    }
    listaPromociones.insertar(codigo, nombre, fechaInicio, fechaFin, diasAplicables);
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
        std::cout << "Beneficio agregado correctamente a la promoción: " << codigoPromo << std::endl;
        return true;
    } else {
        std::cout << "Error al agregar el beneficio a la promoción: " << codigoPromo << std::endl;
        return false;
    }
}

bool guardarDatosService::crearFuncion(const std::string& codigoPelicula, int filas, int columnas, const std::string& horario, const std::string& sala) {
    // Verifica si la película existe en el árbol
    Pelicula* pelicula = nullptr;
    try {
        pelicula = arbol.buscar(codigoPelicula);
    } catch (const std::exception&) {
        std::cout << "Error: No existe una película con el código " << codigoPelicula << std::endl;
        return false;
    }

    if (pelicula == nullptr) {
        std::cout << "Error: No existe una película con el código " << codigoPelicula << std::endl;
        return false;
    }

    int siguiente = 1;
    std::string codigo;
    do {
        codigo = "F" + std::to_string(siguiente++);
    } while (arbolFunciones.CodigoExiste(codigo));
    return crearFuncion(codigo, codigoPelicula, filas, columnas, horario, sala);
}

bool guardarDatosService::crearFuncion(const std::string& codigoFuncion, const std::string& codigoPelicula, int filas, int columnas, const std::string& horario, const std::string& sala, const std::string& archivoAsientos) {
    Pelicula* pelicula = nullptr;
    try { pelicula = arbol.buscar(codigoPelicula); } catch (...) { return false; }
    if (pelicula == nullptr || filas <= 0 || columnas <= 0 || codigoFuncion.empty() || arbolFunciones.CodigoExiste(codigoFuncion)) return false;
    arbolFunciones.insertar(codigoFuncion, filas, columnas, pelicula->titulo, horario, sala);
    const std::string nombreArchivo = archivoAsientos.empty() ? codigoFuncion + "_funcion.json" : archivoAsientos;
    const std::string rutaAsientos = rutasReportes::directorioAsientos() + "/" + nombreArchivo;
    std::ifstream existente(rutaAsientos);
    if (!existente.good()) {
        std::ofstream archivo(rutaAsientos);
        if (!archivo.is_open()) return false;
        archivo << "{\n  \"codigo_funcion\": \"" << codigoFuncion << "\",\n  \"asientos_ocupados\": []\n}\n";
    }
    cargarAsientos(codigoFuncion, nombreArchivo);
    MatrizCine* matriz = arbolFunciones.buscar(codigoFuncion);
    if (matriz != nullptr) matriz->generarGraphviz();
    arbolFunciones.generarDot();
    return true;
}

bool guardarDatosService::reservarAsiento(const std::string& nombreCliente, const std::string& fila, const std::string& columna) {
    return reservarAsientoFuncion("F1", nombreCliente, fila, columna);
}

bool guardarDatosService::reservarAsientoFuncion(const std::string& codigoFuncion, const std::string& nombreCliente, const std::string& fila, const std::string& columna) {
    MatrizCine* matriz = arbolFunciones.buscar(codigoFuncion);
    if (matriz == nullptr || !matriz->reservarAsiento(nombreCliente, fila, columna)) return false;
    matriz->guardarAsientosJson(rutasReportes::directorioAsientos() + "/" + codigoFuncion + "_funcion.json");
    return true;
}

void guardarDatosService::cargarAsientos(const std::string& codigoFuncion, const std::string& archivoAsientos) {
    MatrizCine* matriz = arbolFunciones.buscar(codigoFuncion);
    if (matriz == nullptr) return;
    const std::string nombreArchivo = archivoAsientos.empty() ? codigoFuncion + "_funcion.json" : archivoAsientos;
    std::ifstream archivo(rutasReportes::directorioAsientos() + "/" + nombreArchivo);
    if (!archivo.is_open()) return;
    json datos;
    try { archivo >> datos; } catch (...) { return; }
    if (!datos.contains("asientos_ocupados") || !datos["asientos_ocupados"].is_array()) return;
    for (const auto& asiento : datos["asientos_ocupados"]) {
        const int fila = asiento.value("fila", 0);
        const int columna = asiento.value("columna", 0);
        if (fila > 0 && columna > 0) {
            matriz->reservarAsiento(asiento.value("codigo_reserva", ""), std::string(1, static_cast<char>('A' + fila - 1)), std::to_string(columna));
        }
    }
}

void guardarDatosService::guardarSolicitud(const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion) {
    listaSolicitudes.insertarSolicitud(nombreCliente, telefonoContacto, tipoSolicitud, descripcion);
}

bool guardarDatosService::guardarCliente(const std::string& id, const std::string& nombre, const std::string& correo, const std::string& telefono, const std::string& password) {
    if (id.empty() || nombre.empty() || correo.empty() || telefono.empty() || password.empty()) return false;
    if (arbolClientes.existe(id) || arbolClientes.correoExiste(correo)) return false;
    return arbolClientes.insertar(Cliente(id, nombre, correo, telefono, password, "cliente"));
}

bool guardarDatosService::cargarJSONClientes(const std::string& ruta) {
    return arbolClientes.cargarJSON(ruta);
}




bool guardarDatosService::cargarJSONPeliculas(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir el archivo JSON: " << ruta << std::endl;
        return false;
    }

    json datosJson;
    try {
        archivo >> datosJson;
    } catch (const json::parse_error& e) {
        std::cout << "Error al parsear el archivo JSON: " << e.what() << std::endl;
        return false;
    }

    if (!datosJson.is_object() || !datosJson.contains("peliculas") || !datosJson["peliculas"].is_array()) return false;
    for (const auto& peliculaJson : datosJson["peliculas"]) {
        std::string id = peliculaJson.value("codigo", "");
        std::string titulo = peliculaJson.value("titulo", "");
        std::string genero = peliculaJson.value("genero", "");
        int duracion = peliculaJson.value("duracion", 0);
        std::string clasificacion = peliculaJson.value("clasificacion", "");
        std::string idioma = peliculaJson.value("idioma", "");
        std::string fechaEstreno = peliculaJson.value("fecha_estreno", "");
        std::string fechaFin = peliculaJson.value("fecha_fin", "");
        
        if (id.empty() || titulo.empty() || genero.empty() || duracion <= 0 || clasificacion.empty()) {
            std::cout << "Error: Datos incompletos para una película en el archivo JSON." << std::endl;
            continue; // O manejar el error según sea necesario
        }

        if (!arbol.CodigoExiste(id)) arbol.insertar(id, titulo, genero, duracion, clasificacion, idioma, fechaEstreno, fechaFin);
        if (peliculaJson.contains("funciones") && peliculaJson["funciones"].is_array()) {
            for (const auto& funcion : peliculaJson["funciones"]) {
                crearFuncion(funcion.value("codigo_funcion", ""), id,
                    funcion.value("filas", 0), funcion.value("columnas", 0),
                    funcion.value("horario", ""), funcion.value("sala", ""),
                    funcion.value("archivo_asientos", ""));
            }
        }
    }

    return true;
}
        
void guardarDatosService::graficarReportes() {
    arbol.generarDot();
    arbolFunciones.generarDot();
    listaPromociones.graficar();
    arbolFunciones.inOrden([](MatrizCine* matriz) { matriz->generarGraphviz(); });
    listaSolicitudes.graficar();
    arbolClientes.generarDot();
}
  
