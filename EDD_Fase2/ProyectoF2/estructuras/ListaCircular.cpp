#include <iostream>
#include <fstream>
#include "ListaCircular.h"
#include "../servicios/rutasReportes.h"

using namespace std;

namespace {
string escaparHtml(const string& texto) {
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
}

ListaCircular::ListaCircular() {
    cabeza = nullptr;
}

bool ListaCircular::codigoExiste(const string& codigo) {
    if (cabeza == nullptr) return false;

    NodoPromocion* actual = cabeza;
    do {
        if (actual->promocion->codigo == codigo) {
            return true;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    return false;
}

void ListaCircular::insertar(string codigo, string nombre, string fechaInicio, string fechaFin, string diasAplicables) {
    NodoPromocion* nuevo = new NodoPromocion(new Promocion(codigo, nombre, fechaInicio, fechaFin, diasAplicables));

    if (cabeza == nullptr) {
        cabeza = nuevo;
        nuevo->siguiente = cabeza;
    } else {
        NodoPromocion* actual = cabeza;
        while (actual->siguiente != cabeza) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
        nuevo->siguiente = cabeza;
    }
}

bool ListaCircular::agregarBeneficioAPromocion(const string& codigoPromo, string tipo, string descripcion, string valor) {
    if (cabeza == nullptr) return false;

    NodoPromocion* actual = cabeza;
    do {
        if (actual->promocion->codigo == codigoPromo) {
            actual->listaBeneficios.agregar(tipo, descripcion, valor);
            return true;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);

    return false; // Promoción no encontrada
}

void ListaCircular::recorrer(const std::function<void(const Promocion*, const ListaDoble&)>& funcion) const {
    if (cabeza == nullptr) {
        return;
    }

    NodoPromocion* actual = cabeza;
    do {
        funcion(actual->promocion, actual->listaBeneficios);
        actual = actual->siguiente;
    } while (actual != cabeza);
}

void ListaCircular::eliminar(string nombre) {
    if (cabeza == nullptr) return;

    NodoPromocion* actual = cabeza;
    NodoPromocion* anterior = nullptr;

    do {
        if (actual->promocion->nombre == nombre) {
            if (actual == cabeza) {
                if (actual->siguiente == cabeza) {
                    cabeza = nullptr;
                } else {
                    NodoPromocion* ultimo = cabeza;
                    while (ultimo->siguiente != cabeza) {
                        ultimo = ultimo->siguiente;
                    }
                    cabeza = cabeza->siguiente;
                    ultimo->siguiente = cabeza;
                }
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual->promocion;
            delete actual;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    } while (actual != cabeza);
}

void ListaCircular::graficar() {
    if (cabeza == nullptr) {
        cout << "La lista esta vacia, no hay nada que graficar." << endl;
        return;
    }

    const auto directorio = rutasReportes::directorio();
    const auto rutaDot = directorio + "/lista_unificada.dot";
    const auto rutaPng = directorio + "/lista_unificada.png";
    ofstream archivo(rutaDot);
    archivo << "digraph EstratificacionPromociones {\n";
    archivo << "    splines=true;\n";

    NodoPromocion* actual = cabeza;
    int i = 0;

    // Declaración e impresión de nodos de la Lista Circular (Nivel Superior)
    archivo << "    { rank=same; ";
    do {
        string promoId = "promo" + to_string(i);
        archivo << promoId << " ";
        actual = actual->siguiente;
        i++;
    } while (actual != cabeza);
    archivo << "}\n\n";

    actual = cabeza;
    int index = 0;
    do {
        string promoId = "promo" + to_string(index);
        archivo << "    " << promoId
            << " [shape=box, style=\"filled,rounded\", fillcolor=\"#d4f8f4\", penwidth=2, label=<"
            << "<TABLE BORDER=\"0\" CELLBORDER=\"0\" CELLPADDING=\"2\">"
            << "<TR><TD ALIGN=\"LEFT\"><B>Promocion</B></TD></TR>"
            << "<TR><TD ALIGN=\"LEFT\">Codigo: " << escaparHtml(actual->promocion->codigo) << "</TD></TR>"
            << "<TR><TD ALIGN=\"LEFT\">Nombre: " << escaparHtml(actual->promocion->nombre) << "</TD></TR>"
            << "<TR><TD ALIGN=\"LEFT\">Inicio: " << escaparHtml(actual->promocion->fechaInicio) << "</TD></TR>"
            << "<TR><TD ALIGN=\"LEFT\">Fin: " << escaparHtml(actual->promocion->fechaFin) << "</TD></TR>"
            << "<TR><TD ALIGN=\"LEFT\">Dias: " << escaparHtml(actual->promocion->diasAplicables) << "</TD></TR>"
            << "</TABLE>>];\n";
        
        // Delegar a la ListaDoble la escritura de sus nodos y conexiones
        actual->listaBeneficios.generarDotUnificado(archivo, promoId, actual->promocion->codigo);

        actual = actual->siguiente;
        index++;
    } while (actual != cabeza);

    // Conexiones de la lista circular
    for (int j = 0; j < index; j++) {
        int siguienteIndex = (j + 1) % index;
        archivo << "    promo" << j << " -> promo" << siguienteIndex << ";\n";
    }

    archivo << "}\n";
    archivo.close();

    int resultado = rutasReportes::convertirAPng(rutaDot, rutaPng);
    if (resultado == 0) {
        cout << "Grafico unificado generado: reportes/lista_unificada.png" << endl;
    } else {
        cout << "Error al compilar con Graphviz." << endl;
    }
}

ListaCircular::~ListaCircular() {
    if (cabeza == nullptr) return;
    NodoPromocion* actual = cabeza;
    do {
        NodoPromocion* temp = actual;
        actual = actual->siguiente;
        delete temp->promocion;
        delete temp;
    } while (actual != cabeza);
}