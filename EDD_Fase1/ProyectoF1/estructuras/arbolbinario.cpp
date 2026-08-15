#include <iostream>
#include <fstream>
#include <stdexcept>
#include "../modelos/NodoPelicula.cpp"


//Arbol binario de búsqueda para almacenar películas

class ArbolBinario {
private:

    NodoPelicula* raiz; //El nodo raiz del arbol

    NodoPelicula* insertarNodo(NodoPelicula* nodo, Pelicula* pelicula); //Inserta un nodo en el árbol
    NodoPelicula* buscarNodo(NodoPelicula* nodo, const std::string& titulo); // Busca un nodo en el arbol por el título de la película

    void preOrdenAux(NodoPelicula* nodo);
    void inOrdenAux(NodoPelicula* nodo);
    void postOrdenAux(NodoPelicula* nodo);
    void generarDotAux(NodoPelicula* nodo, std::ofstream& archivo);
    void eliminarNodos(NodoPelicula* nodo);
    std::string quitarComillas(const std::string& campo);

public:
    ArbolBinario();
    ArbolBinario(const ArbolBinario&) = delete;
    ArbolBinario& operator=(const ArbolBinario&) = delete;
    bool estaVacio();
    void insertar(Pelicula* pelicula);
    Pelicula* buscar(const std::string& titulo);
    void cargarCSV(const std::string& ruta);
    void preOrden();
    void inOrden();
    void postOrden();
    void generarDot();
    ~ArbolBinario();
};

ArbolBinario::ArbolBinario() {
    raiz = nullptr;
}

bool ArbolBinario::estaVacio() {
    return raiz == nullptr;
}

void ArbolBinario::insertar(Pelicula* pelicula) {
    if (pelicula == nullptr) {
        return;
    }

    if (estaVacio()) {
        raiz = new NodoPelicula(new Pelicula(*pelicula));
    }
    else {
        raiz = insertarNodo(raiz, pelicula);
    }
}

NodoPelicula* ArbolBinario::insertarNodo(NodoPelicula* nodo, Pelicula* pelicula) {
    if (nodo == nullptr) {
        return new NodoPelicula(new Pelicula(*pelicula));
    }

    if (pelicula->titulo == nodo->pelicula->titulo) {
        return nodo;
    }
    else if (pelicula->titulo < nodo->pelicula->titulo) {
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



NodoPelicula* ArbolBinario::buscarNodo(NodoPelicula* nodo, const std::string& titulo) {
    if (nodo == nullptr || nodo->pelicula->titulo == titulo) {
        return nodo;
    }

    if (titulo < nodo->pelicula->titulo) {
        return buscarNodo(nodo->izq, titulo);
    }
    else {
        return buscarNodo(nodo->der, titulo);
    }
}

Pelicula* ArbolBinario::buscar(const std::string& titulo) {
    NodoPelicula* nodo = buscarNodo(raiz, titulo);
    if (nodo != nullptr) {
        return nodo-> pelicula;
    }
    else {
        throw std::runtime_error("Pelicula no encontrada");
    }
}




void ArbolBinario::preOrdenAux(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        std::cout << nodo->pelicula->titulo << std::endl;
        preOrdenAux(nodo->izq);
        preOrdenAux(nodo->der);
    }
}

void ArbolBinario::preOrden() {
    preOrdenAux(raiz);
}

void ArbolBinario::inOrdenAux(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        inOrdenAux(nodo-> izq);
        std::cout << nodo->pelicula->titulo << std::endl;
        inOrdenAux(nodo->der);
    }
}

void ArbolBinario::inOrden() {
    inOrdenAux(raiz);
}

void ArbolBinario::postOrdenAux(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        postOrdenAux(nodo->izq);
        postOrdenAux(nodo->der);
        std::cout << nodo->pelicula->titulo << std::endl;
    }
}

void ArbolBinario::postOrden() {
    postOrdenAux(raiz);
}

void ArbolBinario::generarDotAux(NodoPelicula* nodo, std::ofstream& archivo) {
    if (nodo != nullptr) {
        std::string nombreNodo = "\"" + nodo->pelicula->titulo + "\"";
        if (nodo->izq != nullptr) {
            std::string nombreIzq = "\"" + nodo->izq->pelicula->titulo + "\"";
            archivo << nombreNodo << " -> " << nombreIzq << ";\n";
        }
        if (nodo->der != nullptr) {
            std::string nombreDer = "\"" + nodo->der->pelicula->titulo + "\"";
            archivo << nombreNodo << " -> " << nombreDer << ";\n";
        }
        generarDotAux(nodo->izq, archivo);
        generarDotAux(nodo->der, archivo);
    }
}

void ArbolBinario::generarDot() {
    std::ofstream archivo("arbol.dot");
    archivo << "digraph ArbolBinarioDeBusqueda {\n";
    archivo << "bgcolor=lightblue;\n";
    archivo << "Node [shape=square, style=filled, fillcolor=beige, color=black, penwidth=2];\n";
    archivo << "edge [splines=polyline, arrowhead=curve];\n";

    generarDotAux(raiz, archivo);
    archivo << "}\n";
    archivo.close();

    system("dot -Tpng arbol.dot -o arbol_binario_peliculas.png");
    system("start arbol_binario_peliculas.png");
}

ArbolBinario::~ArbolBinario() {
    eliminarNodos(raiz);
}

void ArbolBinario::eliminarNodos(NodoPelicula* nodo) {
    if (nodo != nullptr) {
        eliminarNodos(nodo->izq);
        eliminarNodos(nodo->der);
        delete nodo;
    }
}

