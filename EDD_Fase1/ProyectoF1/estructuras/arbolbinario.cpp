#include "ArbolBinario.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include "../servicios/rutasReportes.h"

ArbolBinario::ArbolBinario() {
    raiz = nullptr;
}

bool ArbolBinario::CodigoExiste(const std::string& codigo) {
    if (estaVacio()) {
        return false;
    }
    return buscarNodo(raiz, codigo) != nullptr;
}

bool ArbolBinario::estaVacio() {
    return raiz == nullptr;
}

void ArbolBinario::insertar(std::string codigo, std::string titulo, std::string genero, int duracion, std::string clasificacion, std::string idioma, std::string fechaEstreno, std::string fechaFinCartelera) {
    Pelicula* pelicula = new Pelicula(codigo, titulo, genero, duracion, clasificacion, idioma, fechaEstreno, fechaFinCartelera);

    if (estaVacio()) {
        raiz = new NodoPelicula(pelicula);
    }
    else {
        raiz = insertarNodo(raiz, pelicula);
    }
}

// El arbol queda ordenado por "codigo". Antes se ordenaba por "titulo" pero buscar()/eliminar()
// navegaban comparando por codigo, lo cual rompia el invariante del arbol de busqueda.
NodoPelicula* ArbolBinario::insertarNodo(NodoPelicula* nodo, Pelicula* pelicula) {
    if (nodo == nullptr) {
        return new NodoPelicula(pelicula);
    }

    if (pelicula->id == nodo->pelicula->id) {
        delete pelicula; // codigo duplicado: no se inserta y se libera la copia que sobra
        return nodo;
    }
    else if (pelicula->id < nodo->pelicula->id) {
        nodo->izq = insertarNodo(nodo->izq, pelicula);
    }
    else {
        nodo->der = insertarNodo(nodo->der, pelicula);
    }

    return nodo;
}

std::string ArbolBinario::quitarComillas(const std::string& campo) {
    std::string resultado = campo;
    if (resultado.size() >= 2 && resultado.front() == '"' && resultado.back() == '"') {
        resultado = resultado.substr(1, resultado.size() - 2);
    }
    return resultado;
}

NodoPelicula* ArbolBinario::buscarNodo(NodoPelicula* nodo, const std::string& codigo) {
    if (nodo == nullptr || nodo->pelicula->id == codigo) {
        return nodo;
    }

    if (codigo < nodo->pelicula->id) {
        return buscarNodo(nodo->izq, codigo);
    }
    else {
        return buscarNodo(nodo->der, codigo);
    }
}

Pelicula* ArbolBinario::buscar(const std::string& codigo) {
    NodoPelicula* nodo = buscarNodo(raiz, codigo);
    if (nodo != nullptr) {
        return nodo->pelicula;
    }
    else {
        throw std::runtime_error("Pelicula no encontrada");
    }
}

void ArbolBinario::eliminarConCodigo(const std::string& codigo) {
    raiz = eliminarNodo(raiz, codigo);
}

NodoPelicula* ArbolBinario::eliminarNodo(NodoPelicula* nodo, const std::string& codigo) {
    if (nodo == nullptr) {
        return nullptr; // no existe una pelicula con ese codigo, no hay nada que eliminar
    }

    if (codigo < nodo->pelicula->id) {
        nodo->izq = eliminarNodo(nodo->izq, codigo);
    }
    else if (codigo > nodo->pelicula->id) {
        nodo->der = eliminarNodo(nodo->der, codigo);
    }
    else {
        // Este es el nodo a eliminar

        // Caso 1: no tiene hijo izquierdo (incluye ser una hoja)
        if (nodo->izq == nullptr) {
            NodoPelicula* reemplazo = nodo->der;
            delete nodo; // ~NodoPelicula ya libera su Pelicula
            return reemplazo;
        }

        // Caso 2: no tiene hijo derecho
        if (nodo->der == nullptr) {
            NodoPelicula* reemplazo = nodo->izq;
            delete nodo;
            return reemplazo;
        }

        // Caso 3: tiene dos hijos -> se reemplaza con el sucesor inorder
        // (el nodo mas a la izquierda del subarbol derecho)
        NodoPelicula* sucesor = nodo->der;
        while (sucesor->izq != nullptr) {
            sucesor = sucesor->izq;
        }
        std::string codigoSucesor = sucesor->pelicula->id;

        delete nodo->pelicula;
        nodo->pelicula = new Pelicula(*sucesor->pelicula);

        nodo->der = eliminarNodo(nodo->der, codigoSucesor);
    }

    return nodo;
}

void ArbolBinario::inOrdenAux(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        inOrdenAux(nodo->izq);
        std::cout << nodo->pelicula->id << " - " << nodo->pelicula->titulo << std::endl;
        inOrdenAux(nodo->der);
    }
}

void ArbolBinario::inOrden() {
    inOrdenAux(raiz);
}

void ArbolBinario::inOrdenAux(NodoPelicula* nodo, const std::function<void(Pelicula*)>& funcion) {
    if (nodo != nullptr) {
        inOrdenAux(nodo->izq, funcion);
        funcion(nodo->pelicula);
        inOrdenAux(nodo->der, funcion);
    }
}

void ArbolBinario::inOrden(const std::function<void(Pelicula*)>& funcion) {
    inOrdenAux(raiz, funcion);
}

// Dias entre "fecha" (AAAA-MM-DD) y la fecha de ejecucion del programa.
// Positivo si "fecha" es futura, negativo si ya paso.
long ArbolBinario::diferenciaEnDias(const std::string& fecha) {
    int anio = std::stoi(fecha.substr(0, 4));
    int mes  = std::stoi(fecha.substr(5, 2));
    int dia  = std::stoi(fecha.substr(8, 2));

    std::tm fechaFin = {};
    fechaFin.tm_year = anio - 1900;
    fechaFin.tm_mon  = mes - 1;
    fechaFin.tm_mday = dia;
    fechaFin.tm_hour = 12; // mediodia, para no tener problemas con cambios de horario

    std::time_t tiempoFin = std::mktime(&fechaFin);

    std::time_t ahoraRaw = std::time(nullptr);
    std::tm* hoyTmp = std::localtime(&ahoraRaw);
    std::tm hoy = *hoyTmp; // se copia: localtime reutiliza un buffer interno
    hoy.tm_hour = 12;
    hoy.tm_min = 0;
    hoy.tm_sec = 0;
    std::time_t tiempoHoy = std::mktime(&hoy);

    double segundos = std::difftime(tiempoFin, tiempoHoy);
    return static_cast<long>(segundos / 86400.0 + (segundos >= 0 ? 0.5 : -0.5));
}

std::string ArbolBinario::colorSegunFecha(const std::string& fechaFinCartelera) {
    long dias = diferenciaEnDias(fechaFinCartelera);

    if (dias < 0) {
        return "#e74c3c"; // rojo: la cartelera ya termino
    }
    else if (dias <= 3) {
        return "#f1c40f"; // amarillo: termina en 3 dias o menos
    }
    else {
        return "#2ecc71"; // verde: falta mas de 3 dias
    }
}

void ArbolBinario::generarDotAux(NodoPelicula* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) {
        return;
    }

    // Recorrido inorder: primero el subarbol izquierdo...
    generarDotAux(nodo->izq, archivo);

    // ...luego se describe el nodo actual...
    std::string idNodo = "nodo_" + nodo->pelicula->id;
    std::string color = colorSegunFecha(nodo->pelicula->fechaFinCartelera);

    archivo << idNodo << " [label=\""
            << "ID: " << nodo->pelicula->id << "\\n"
            << "Titulo: " << nodo->pelicula->titulo << "\\n"
            << "Genero: " << nodo->pelicula->genero << "\\n"
            << "Duracion: " << nodo->pelicula->duracion << " min\\n"
            << "Clasificacion: " << nodo->pelicula->clasificacion << "\\n"
            << "Idioma: " << nodo->pelicula->idioma << "\\n"
            << "Estreno: " << nodo->pelicula->fechaEstreno << "\\n"
            << "Fin cartelera: " << nodo->pelicula->fechaFinCartelera
            << "\", shape=box, style=filled, fillcolor=\"" << color << "\"];\n";

    if (nodo->izq != nullptr) {
        std::string idIzq = "nodo_" + nodo->izq->pelicula->id;
        archivo << idNodo << " -> " << idIzq << ";\n";
    }
    if (nodo->der != nullptr) {
        std::string idDer = "nodo_" + nodo->der->pelicula->id;
        archivo << idNodo << " -> " << idDer << ";\n";
    }

    // ...y por ultimo el subarbol derecho
    generarDotAux(nodo->der, archivo);
}

void ArbolBinario::generarDot() {
    const auto directorio = rutasReportes::directorio();
    const auto rutaDot = directorio + "/arbol.dot";
    const auto rutaPng = directorio + "/arbol_binario_peliculas.png";
    std::ofstream archivo(rutaDot);
    archivo << "digraph ArbolBinarioDeBusqueda {\n";
    archivo << "bgcolor=lightblue;\n";
    archivo << "label=\"Arbol de peliculas (verde: +3 dias, amarillo: <=3 dias, rojo: cartelera vencida)\";\n";
    archivo << "labelloc=t;\n";
    archivo << "Node [shape=square, style=filled, fillcolor=beige, color=black, penwidth=2];\n";
    archivo << "edge [splines=polyline, arrowhead=curve];\n";

    generarDotAux(raiz, archivo);
    archivo << "}\n";
    archivo.close();

    rutasReportes::convertirAPng(rutaDot, rutaPng);
}

ArbolBinario::~ArbolBinario() {
    eliminarNodos(raiz);
}

void ArbolBinario::eliminarNodos(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        eliminarNodos(nodo->izq);
        eliminarNodos(nodo->der);
        delete nodo; // ~NodoPelicula ya libera su Pelicula; antes esto se borraba dos veces
    }
}

bool ArbolBinario::cargarCSV(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo CSV: " << ruta << std::endl;
        return false;
    }

    std::string linea;
    std::getline(archivo, linea); // encabezado, se descarta

    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string codigo, titulo, genero, duracionStr, clasificacion, idioma, fechaEstreno, fechaFinCartelera;

        std::getline(ss, codigo, ',');
        std::getline(ss, titulo, ',');
        std::getline(ss, genero, ',');
        std::getline(ss, duracionStr, ',');
        std::getline(ss, clasificacion, ',');
        std::getline(ss, idioma, ',');
        std::getline(ss, fechaEstreno, ',');
        std::getline(ss, fechaFinCartelera);

        int duracion = 0;
        try {
            duracion = std::stoi(duracionStr);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error al convertir duracion a entero para la pelicula: " << titulo << ". Se asignara 0." << std::endl;
            duracion = 0;
        }

        insertar(codigo, titulo, genero, duracion, clasificacion, idioma, fechaEstreno, fechaFinCartelera);
    }
    archivo.close();
    return true;
}