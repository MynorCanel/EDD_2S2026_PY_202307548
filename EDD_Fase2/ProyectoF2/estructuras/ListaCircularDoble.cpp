#include "ListaCircularDoble.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <cstdlib>
#include "../servicios/rutasReportes.h"

ListaCircularDoble::ListaCircularDoble() {
    cabeza = nullptr;
    totalSolicitudes = 0;
    solicitudesPendientes = 0;
}

// genera un numero de 4 digitos que no este ya usado por otra solicitud en la lista
int ListaCircularDoble::generarNumeroUnico() {
    static std::mt19937 generador(std::random_device{}());
    std::uniform_int_distribution<int> distribucion(1000, 9999);

    int numero;
    do {
        numero = distribucion(generador);
    } while (buscarNodo(numero) != nullptr);

    return numero;
}

// fecha de hoy tomada del reloj del sistema, en formato AAAA-MM-DD
std::string ListaCircularDoble::obtenerFechaActual() {
    std::time_t ahoraRaw = std::time(nullptr);
    std::tm* tmPtr = std::localtime(&ahoraRaw);
    std::tm hoy = *tmPtr; // se copia porque localtime reutiliza un buffer interno

    std::ostringstream oss;
    oss << (hoy.tm_year + 1900) << "-"
        << std::setfill('0') << std::setw(2) << (hoy.tm_mon + 1) << "-"
        << std::setfill('0') << std::setw(2) << hoy.tm_mday;
    return oss.str();
}

NodoSolicitud* ListaCircularDoble::buscarNodo(int numeroSolicitud) {
    if (cabeza == nullptr) {
        return nullptr;
    }
    NodoSolicitud* actual = cabeza;
    do {
        if (actual->solicitud->numeroSolicitud == numeroSolicitud) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);
    return nullptr;
}

// inserta una nueva solicitud al final de la lista, siempre en estado "en espera"
// el numero se genera aleatoriamente (sin chocar con uno existente) y la fecha
// se toma del reloj del sistema; conecta en ambas direcciones y mantiene el ciclo cerrado
void ListaCircularDoble::insertarSolicitud(const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion) {
    int numero = generarNumeroUnico();
    std::string fecha = obtenerFechaActual();

    NodoSolicitud* nuevo = new NodoSolicitud(new Solicitud(numero, nombreCliente, telefonoContacto, tipoSolicitud, descripcion, fecha, EstadoSolicitud::EnEspera));
    totalSolicitudes++;
    solicitudesPendientes++;

    if (cabeza == nullptr) {
        // si esta vacia, el nuevo nodo se apunta a si mismo en ambos sentidos
        cabeza = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
    } else {
        // el ultimo nodo es cabeza->anterior por ser circular
        NodoSolicitud* ultimo = cabeza->anterior;
        ultimo->siguiente = nuevo;
        nuevo->anterior = ultimo;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
    }
}

// cambia el estado de una solicitud existente sin eliminarla de la lista
bool ListaCircularDoble::cambiarEstado(int numeroSolicitud, EstadoSolicitud nuevoEstado) {
    NodoSolicitud* nodo = buscarNodo(numeroSolicitud);
    if (nodo == nullptr) {
        std::cout << "No se encontro la solicitud #" << numeroSolicitud << std::endl;
        return false;
    }

    EstadoSolicitud estadoAnterior = nodo->solicitud->estadoSolicitud;
    if (estadoAnterior == nuevoEstado) {
        return true; // ya estaba en ese estado
    }

    if (estadoAnterior == EstadoSolicitud::EnEspera) {
        solicitudesPendientes--;
    }
    if (nuevoEstado == EstadoSolicitud::EnEspera) {
        solicitudesPendientes++;
    }

    nodo->solicitud->estadoSolicitud = nuevoEstado;
    std::cout << "Solicitud #" << numeroSolicitud << " ahora esta " << textoDesdeEstado(nuevoEstado) << "." << std::endl;
    return true;
}

// elimina una solicitud por completo (distinto de cambiar su estado)
// reconecta los punteros siguiente y anterior del nodo removido
void ListaCircularDoble::eliminar(int numeroSolicitud) {
    NodoSolicitud* nodo = buscarNodo(numeroSolicitud);
    if (nodo == nullptr) {
        std::cout << "No se encontro la solicitud #" << numeroSolicitud << std::endl;
        return;
    }

    if (nodo->solicitud->estadoSolicitud == EstadoSolicitud::EnEspera) {
        solicitudesPendientes--;
    }
    totalSolicitudes--;

    if (nodo->siguiente == nodo) {
        // era el unico nodo en la lista
        cabeza = nullptr;
    } else {
        nodo->anterior->siguiente = nodo->siguiente;
        nodo->siguiente->anterior = nodo->anterior;
        if (nodo == cabeza) {
            cabeza = nodo->siguiente;
        }
    }

    std::cout << "Solicitud #" << nodo->solicitud->numeroSolicitud << " eliminada." << std::endl;
    delete nodo->solicitud; // antes esto no se liberaba (memory leak)
    delete nodo;
}

int ListaCircularDoble::obtenerTotalSolicitudes() const {
    return totalSolicitudes;
}

int ListaCircularDoble::obtenerSolicitudesPendientes() const {
    return solicitudesPendientes;
}

void ListaCircularDoble::recorrer(const std::function<void(Solicitud*)>& funcion) {
    if (cabeza == nullptr) {
        return;
    }
    NodoSolicitud* actual = cabeza;
    do {
        funcion(actual->solicitud);
        actual = actual->siguiente;
    } while (actual != cabeza);
}

// genera un archivo .dot y lo compila a png con graphviz
// nodos redondeados, color tipo semaforo segun el estado, flechas dir=both
// mostrando la lista circular doblemente enlazada completa
void ListaCircularDoble::graficar() {
    if (cabeza == nullptr) {
        std::cout << "La lista esta vacia, no hay nada que graficar." << std::endl;
        return;
    }

    const auto directorio = rutasReportes::directorio();
    const auto rutaDot = directorio + "/lista_circular_doble.dot";
    const auto rutaPng = directorio + "/lista_circular_doble.png";
    std::ofstream archivo(rutaDot);
    archivo << "digraph ListaCircularDoble {" << std::endl;
    archivo << "    rankdir=LR;" << std::endl;
    archivo << "    label=\"Solicitudes (verde: aprobado, amarillo: en espera, rojo: rechazado)\";" << std::endl;
    archivo << "    labelloc=t;" << std::endl;
    archivo << "    fontname=\"Arial\";" << std::endl;
    archivo << "    node [shape=circle, style=\"filled\", fontname=\"Arial\", fontsize=10, margin=\"0.12,0.08\"];" << std::endl;
    archivo << "    edge [dir=both, color=\"#5d6d7e\"];" << std::endl;

    NodoSolicitud* actual = cabeza;
    int i = 0;

    // creamos los nodos del grafo recorriendo el ciclo
    do {
        std::string color = colorDesdeEstado(actual->solicitud->estadoSolicitud);
        archivo << "    nodo" << i << " [fillcolor=\"" << color << "\", label=\""
                << "Solicitud #" << actual->solicitud->numeroSolicitud << "\\n"
                << actual->solicitud->nombreCliente << "\\n"
                << "Tel: " << actual->solicitud->telefonoContacto << "\\n"
                << "Tipo: " << actual->solicitud->tipoSolicitud << "\\n"
                << actual->solicitud->descripcion << "\\n"
                << "Fecha: " << actual->solicitud->fechaSolicitud << "\\n"
                << "Estado: " << textoDesdeEstado(actual->solicitud->estadoSolicitud)
                << "\"];" << std::endl;
        actual = actual->siguiente;
        i++;
    } while (actual != cabeza);

    // flechas bidireccionales entre nodos consecutivos
    for (int j = 0; j < i - 1; j++) {
        archivo << "    nodo" << j << " -> nodo" << j + 1 << ";" << std::endl;
    }
    // flecha bidireccional del ultimo al primero (cierra el ciclo)
    archivo << "    nodo" << i - 1 << " -> nodo0;" << std::endl;

    archivo << "}" << std::endl;
    archivo.close();

    int resultado = rutasReportes::convertirAPng(rutaDot, rutaPng);
    if (resultado == 0) {
        std::cout << "Grafico generado: reportes/lista_circular_doble.png" << std::endl;
    } else {
        std::cout << "Error: no se pudo generar el grafico. Verifica que Graphviz este instalado." << std::endl;
    }
}

// muestra todas las solicitudes en consola
void ListaCircularDoble::mostrar() {
    if (cabeza == nullptr) {
        std::cout << "La lista esta vacia." << std::endl;
        return;
    }

    NodoSolicitud* actual = cabeza;
    do {
        std::cout << actual->solicitud->numeroSolicitud << ", "
                  << actual->solicitud->nombreCliente << ", "
                  << actual->solicitud->telefonoContacto << ", "
                  << actual->solicitud->tipoSolicitud << ", "
                  << actual->solicitud->descripcion << ", "
                  << actual->solicitud->fechaSolicitud << ", "
                  << textoDesdeEstado(actual->solicitud->estadoSolicitud) << std::endl;
        actual = actual->siguiente;
    } while (actual != cabeza);
}

// libera toda la memoria recorriendo el ciclo
ListaCircularDoble::~ListaCircularDoble() {
    if (cabeza == nullptr) return;
    NodoSolicitud* actual = cabeza;
    NodoSolicitud* temp;
    do {
        temp = actual;
        actual = actual->siguiente;
        delete temp->solicitud;
        delete temp;
    } while (actual != cabeza);
}