//Arbol AVL, arbol binario que se balancea automaticamente, aqui se almacenaran las funciones
#include "ArbolAVL.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <algorithm>
#include "../servicios/rutasReportes.h"



ArbolAVL::ArbolAVL() {  // Constructor
    raiz = nullptr;
}


int ArbolAVL::altura(NodoFuncion* nodo) {
    return nodo ? nodo->altura : -1;
}

//Destructor del arbol AVL, libera todos los nodos y sus funciones asociadas
ArbolAVL::~ArbolAVL() {
    eliminarNodos(raiz);
}

void ArbolAVL::eliminarNodos(NodoFuncion* nodo) {
    if (nodo != nullptr) {
        eliminarNodos(nodo->izquierdo);
        eliminarNodos(nodo->derecho);
        delete nodo; // ~NodoFuncion ya libera su MatrizCine
    }
    nodo = nullptr; // Evita que el puntero quede colgando
}

//funcion para verificar si un nodo existe en el AVL

bool ArbolAVL::elementoExiste(const NodoFuncion* nodo, const std::string& codigo) {
    if (nodo == nullptr) {
        return false;
    }
    if (nodo->funcion->codigoFuncion == codigo) {
        return true;
    }
    return elementoExiste(nodo->izquierdo, codigo) || elementoExiste(nodo->derecho, codigo);
}

//Rotaciones para balancear el arbol AVL

void ArbolAVL::rotacionSimpleIzquierda(NodoFuncion*& nodo) {
    NodoFuncion* temp = nodo->derecho;
    nodo->derecho = temp->izquierdo;
    temp->izquierdo = nodo;
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    temp->altura = std::max(altura(temp->izquierdo), altura(temp->derecho)) + 1;
    nodo = temp;
}

void ArbolAVL::rotacionSimpleDerecha(NodoFuncion*& nodo) {
    NodoFuncion* temp = nodo->izquierdo;
    nodo->izquierdo = temp->derecho;
    temp->derecho = nodo;
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    temp->altura = std::max(altura(temp->izquierdo), altura(temp->derecho)) + 1;
    nodo = temp;
}

void ArbolAVL::rotacionDobleIzquierda(NodoFuncion*& nodo) {
    rotacionSimpleDerecha(nodo->derecho);
    rotacionSimpleIzquierda(nodo);
}

void ArbolAVL::rotacionDobleDerecha(NodoFuncion*& nodo) {
    rotacionSimpleIzquierda(nodo->izquierdo);
    rotacionSimpleDerecha(nodo);
}

void ArbolAVL::ajustarAVL(NodoFuncion*& nodo) {
    if (nodo == nullptr) return;

    int balance = altura(nodo->izquierdo) - altura(nodo->derecho);

    if (balance > 1) {
        if (altura(nodo->izquierdo->izquierdo) >= altura(nodo->izquierdo->derecho)) {
            rotacionSimpleDerecha(nodo);
        } else {
            rotacionDobleDerecha(nodo);
        }
    } else if (balance < -1) {
        if (altura(nodo->derecho->derecho) >= altura(nodo->derecho->izquierdo)) {
            rotacionSimpleIzquierda(nodo);
        } else {
            rotacionDobleIzquierda(nodo);
        }
    }

    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
}

void ArbolAVL::insertar(std::string codigoFuncion, int filas, int columnas, std::string titulo, std::string horario, std::string sala) {
    MatrizCine* nuevaFuncion = new MatrizCine(codigoFuncion);
    nuevaFuncion->inicializarSala(filas, columnas, titulo, horario, sala); // Inicializa la sala con 5 filas y 5 columnas como ejemplo
    raiz = insertarNodo(raiz, nuevaFuncion);
}

NodoFuncion* ArbolAVL::insertarNodo(NodoFuncion* nodo, MatrizCine* funcion) {
    if (nodo == nullptr) {
        return new NodoFuncion(funcion, 0);
    }

    if (funcion->codigoFuncion < nodo->funcion->codigoFuncion) {
        nodo->izquierdo = insertarNodo(nodo->izquierdo, funcion);
    } else if (funcion->codigoFuncion > nodo->funcion->codigoFuncion) {
        nodo->derecho = insertarNodo(nodo->derecho, funcion);
    } else {
        // Código duplicado, no se permite
        std::cout << "Error: Ya existe una función con el código " << funcion->codigoFuncion << std::endl;
        delete funcion; // Liberar memoria si no se inserta
        return nodo;
    }

    ajustarAVL(nodo); //en caso se inserta, se ajusta el arbol AVL para mantener el balanceo
    return nodo;
}


NodoFuncion* ArbolAVL::eliminarNodo(NodoFuncion* nodo, const std::string& codigo) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (codigo < nodo->funcion->codigoFuncion) {
        nodo->izquierdo = eliminarNodo(nodo->izquierdo, codigo);
    } else if (codigo > nodo->funcion->codigoFuncion) {
        nodo->derecho = eliminarNodo(nodo->derecho, codigo);
    } else {
        // Nodo encontrado
        if (nodo->izquierdo == nullptr || nodo->derecho == nullptr) {
            NodoFuncion* temp = nodo->izquierdo ? nodo->izquierdo : nodo->derecho;
            delete nodo; // Destructor de NodoFuncion libera MatrizCine
            return temp;
        } else {
            // Nodo con dos hijos: obtener el sucesor en orden
            NodoFuncion* temp = nodo->derecho;
            while (temp && temp->izquierdo != nullptr) {
                temp = temp->izquierdo;
            }
            // Intercambiar punteros evita copiar la matriz dispersa superficialmente.
            MatrizCine* funcionActual = nodo->funcion;
            nodo->funcion = temp->funcion;
            temp->funcion = funcionActual;
            // Eliminar el sucesor
            nodo->derecho = eliminarNodo(nodo->derecho, temp->funcion->codigoFuncion);
        }
    }

    ajustarAVL(nodo); // Ajustar el árbol después de la eliminación
    return nodo;
}

NodoFuncion* ArbolAVL::buscarNodo(NodoFuncion* nodo, const std::string& codigo) {
    if (nodo == nullptr || nodo->funcion->codigoFuncion == codigo) {
        return nodo;
    }

    if (codigo < nodo->funcion->codigoFuncion) {
        return buscarNodo(nodo->izquierdo, codigo);
    } else {
        return buscarNodo(nodo->derecho, codigo);
    }
}

bool ArbolAVL::estaVacio() { return raiz == nullptr; }

bool ArbolAVL::CodigoExiste(const std::string& codigo) {
    return buscar(codigo) != nullptr;
}

MatrizCine* ArbolAVL::buscar(const std::string& codigo) {
    NodoFuncion* nodo = buscarNodo(raiz, codigo);
    return nodo == nullptr ? nullptr : nodo->funcion;
}

bool ArbolAVL::eliminar(const std::string& codigo) {
    if (!CodigoExiste(codigo)) return false;
    raiz = eliminarNodo(raiz, codigo);
    return true;
}

void ArbolAVL::inOrdenAux(NodoFuncion* nodo, const std::function<void(MatrizCine*)>& funcion) {
    if (nodo == nullptr) return;
    inOrdenAux(nodo->izquierdo, funcion);
    funcion(nodo->funcion);
    inOrdenAux(nodo->derecho, funcion);
}

void ArbolAVL::inOrden(const std::function<void(MatrizCine*)>& funcion) {
    inOrdenAux(raiz, funcion);
}

namespace {
void preOrdenFunciones(NodoFuncion* nodo, const std::function<void(MatrizCine*)>& funcion) {
    if (nodo == nullptr) return;
    funcion(nodo->funcion);
    preOrdenFunciones(nodo->izquierdo, funcion);
    preOrdenFunciones(nodo->derecho, funcion);
}
void postOrdenFunciones(NodoFuncion* nodo, const std::function<void(MatrizCine*)>& funcion) {
    if (nodo == nullptr) return;
    postOrdenFunciones(nodo->izquierdo, funcion);
    postOrdenFunciones(nodo->derecho, funcion);
    funcion(nodo->funcion);
}
}

void ArbolAVL::preOrden(const std::function<void(MatrizCine*)>& funcion) { preOrdenFunciones(raiz, funcion); }
void ArbolAVL::postOrden(const std::function<void(MatrizCine*)>& funcion) { postOrdenFunciones(raiz, funcion); }

void ArbolAVL::generarDotAux(NodoFuncion* nodo, std::ofstream& archivo) {
    if (nodo == nullptr) return;
    const std::string id = "funcion_" + nodo->funcion->codigoFuncion;
    archivo << id << " [label=\"" << nodo->funcion->codigoFuncion
            << "\\n" << nodo->funcion->obtenerPelicula() << "\\n"
            << nodo->funcion->obtenerHorario() << "\\n" << nodo->funcion->obtenerSala()
            << "\\naltura: " << nodo->altura << "\", shape=box];\n";
    if (nodo->izquierdo != nullptr) archivo << id << " -> funcion_" << nodo->izquierdo->funcion->codigoFuncion << ";\n";
    if (nodo->derecho != nullptr) archivo << id << " -> funcion_" << nodo->derecho->funcion->codigoFuncion << ";\n";
    generarDotAux(nodo->izquierdo, archivo);
    generarDotAux(nodo->derecho, archivo);
}

void ArbolAVL::generarDot() {
    const std::string dir = rutasReportes::directorio();
    const std::string dot = dir + "/arbol_avl.dot";
    std::ofstream archivo(dot);
    archivo << "digraph ArbolAVL {\nnode [shape=box, style=filled, fillcolor=lightblue];\n";
    generarDotAux(raiz, archivo);
    archivo << "}\n";
    archivo.close();
    rutasReportes::convertirAPng(dot, dir + "/arbol_avl.png");
}