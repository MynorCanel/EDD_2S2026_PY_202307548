#include <iostream>
#include "ListaDoble.h"

using namespace std;

ListaDoble::ListaDoble() : cabeza(nullptr) {}

void ListaDoble::agregar(string tipo, string descripcion, string valor) {
    NodoBeneficio* nuevo = new NodoBeneficio(new Beneficio(tipo, descripcion, valor));

    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        NodoBeneficio* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
        nuevo->anterior = actual;
    }
}

void ListaDoble::eliminar(string tipo) {
    if (cabeza == nullptr) {
        cout << "La lista esta vacia." << endl;
        return;
    }

    NodoBeneficio* actual = cabeza;
    while (actual != nullptr && actual->beneficio->tipoBeneficio != tipo) {
        actual = actual->siguiente;
    }

    if (actual == nullptr) {
        cout << "No se encontro Beneficio con tipo " << tipo << endl;
        return;
    }

    if (actual == cabeza) {
        cabeza = actual->siguiente;
        if (cabeza != nullptr) {
            cabeza->anterior = nullptr;
        }
    } else {
        actual->anterior->siguiente = actual->siguiente;
        if (actual->siguiente != nullptr) {
            actual->siguiente->anterior = actual->anterior;
        }
    }

    delete actual->beneficio;
    delete actual;
}

bool ListaDoble::estaVacia() const {
    return cabeza == nullptr;
}

void ListaDoble::recorrer(const std::function<void(const Beneficio*)>& funcion) const {
    NodoBeneficio* actual = cabeza;
    while (actual != nullptr) {
        funcion(actual->beneficio);
        actual = actual->siguiente;
    }
}

string ListaDoble::escaparHtml(const string& texto) {
    string resultado;
    resultado.reserve(texto.size());

    for (char caracter : texto) {
        switch (caracter) {
            case '&': resultado += "&amp;"; break;
            case '<': resultado += "&lt;"; break;
            case '>': resultado += "&gt;"; break;
            case '"': resultado += "&quot;"; break;
            default: resultado += caracter; break;
        }
    }
    return resultado;
}

void ListaDoble::generarDotUnificado(ofstream& archivo, const string& idPadre) const {
    if (cabeza == nullptr) return;

    NodoBeneficio* actual = cabeza;
    int j = 0;

    while (actual != nullptr) {
        string nodeId = idPadre + "_b" + to_string(j);
        
        archivo << "    " << nodeId << " [label=\"" 
                << escaparHtml(actual->beneficio->tipoBeneficio) << "\", "
                << escaparHtml(actual->beneficio->descripcion) << "\", "
                << escaparHtml(actual->beneficio->valor) << "\", shape=box, style=\"filled,rounded\", fillcolor=\"#ffffcc\"];\n";

        if (j == 0) {
            archivo << "    " << idPadre << " -> " << nodeId << " [style=dashed];\n";
        } else {
            string prevId = idPadre + "_b" + to_string(j - 1);
            archivo << "    " << prevId << " -> " << nodeId << " [dir=both];\n";
        }

        if (actual->siguiente == nullptr) {
            string nullId = idPadre + "_null";
            archivo << "    " << nullId << " [label=\"null\", shape=box, style=rounded];\n";
            archivo << "    " << nodeId << " -> " << nullId << ";\n";
        }

        actual = actual->siguiente;
        j++;
    }
}

void ListaDoble::mostrar() const {
    if (cabeza == nullptr) {
        cout << "La lista esta vacia." << endl;
        return;
    }

    NodoBeneficio* actual = cabeza;
    while (actual != nullptr) {
        cout << "Tipo: " << actual->beneficio->tipoBeneficio 
             << ", Descripcion: " << actual->beneficio->descripcion 
             << ", Valor: " << actual->beneficio->valor << endl;
        actual = actual->siguiente;
    }
}

ListaDoble::~ListaDoble() {
    NodoBeneficio* actual = cabeza;
    while (actual != nullptr) {
        NodoBeneficio* temp = actual;
        actual = actual->siguiente;
        delete temp->beneficio;
        delete temp;
    }
}