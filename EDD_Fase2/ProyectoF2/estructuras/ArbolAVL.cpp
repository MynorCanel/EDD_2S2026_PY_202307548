//Arbol AVL, arbol binario que se balancea automaticamente, aqui se almacenaran las funciones
#include "ArbolAVL.h"
#include <iostream>



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
