#include "ArbolB.h"
#include "../servicios/json.hpp"
#include "../servicios/rutasReportes.h"
#include <algorithm>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

namespace {

 //para escapar caracteres especiales en los nodos del archivo .dot para el reporte de graphviz
std::string escaparDot(const std::string& texto)
{
    std::string resultado;
    for (char caracter : texto) {
        if (caracter == '"' || caracter == '\\') resultado += '\\';
        resultado += caracter;
    }
    return resultado;
}
}

ArbolB::ArbolB() : raiz(nullptr) {} //se inicializa el arbol con la raiz en nullptr, indicando que el arbol esta vacio

ArbolB::~ArbolB() //constructuor
{
    destruirNodo(raiz);
}

bool ArbolB::estaVacio() const  //se compruba si el arbol esta vacio
{
    return raiz == nullptr || raiz->cantidad == 0;
}


//Elimina un nodo del arbol B y lo reestructura para mantener las propiedades del arbol B
void ArbolB::destruirNodo(NodoCliente* nodo)
{
    if (nodo == nullptr) return;
    if (!nodo->hoja) {
        destruirNodo(nodo->hijoIzquierdo);
        destruirNodo(nodo->hijoCentroIzquierdo);
        destruirNodo(nodo->hijoCentroDerecho);
        destruirNodo(nodo->hijoDerecho);
    }
    delete nodo;
}

//busca un nodo en el arbol de manera recursiva
//La forma en que funciona es que se compara el id del cliente con los ids de los clientes en el nodo actual, y dependiendo de la comparación, se decide si buscar en el hijo izquierdo, centro izquierdo, centro derecho o derecho del nodo actual.
//Se recorre de tal manera de que se mantiene el orden de los ids de los clientes en el arbol B, lo que permite una busqueda eficiente.
Cliente* ArbolB::buscarNodo(NodoCliente* nodo, const std::string& id) const
{
    if (nodo == nullptr || nodo->cantidad == 0) return nullptr;
    if (nodo->primero != nullptr && id == nodo->primero->id) return nodo->primero;
    if (nodo->cantidad > 1 && id == nodo->segundo->id) return nodo->segundo;
    if (nodo->cantidad > 2 && id == nodo->tercero->id) return nodo->tercero;

    if (id < nodo->primero->id) return buscarNodo(nodo->hijoIzquierdo, id);
    if (nodo->cantidad == 1 || id < nodo->segundo->id) return buscarNodo(nodo->hijoCentroIzquierdo, id);
    if (nodo->cantidad == 2 || id < nodo->tercero->id) return buscarNodo(nodo->hijoCentroDerecho, id);
    return buscarNodo(nodo->hijoDerecho, id);
}


// retorna un puntero al cliente con el id especificado, o nullptr si no se encuentra
Cliente* ArbolB::buscar(const std::string& id) const
{
    return buscarNodo(raiz, id);
}

//comprueba si un cliente con el id especificado existe en el arbol B
bool ArbolB::existe(const std::string& id) const
{
    return buscar(id) != nullptr;
}

// para no permitir la insercion de clientes con el mismo correo, se recorre el arbol y se compara el correo de cada cliente con el correo especificado. Si se encuentra un cliente con el mismo correo, se retorna true, indicando que el correo ya existe en el arbol B.
bool ArbolB::correoExiste(const std::string& correo) const
{
    bool encontrado = false;
    recorrer([&](Cliente* cliente) {
        if (cliente != nullptr && cliente->correo == correo) encontrado = true;
    });
    return encontrado;
}

//inserta un cliente en una hoja del arbol B, si la hoja esta llena, se divide el nodo y se inserta el cliente en el nodo correspondiente. Si la hoja no esta llena, se inserta el cliente en la hoja.
//La funcion insertar es la funcion principal para insertar un cliente en el arbol B, y se encarga de manejar los casos especiales, como cuando el arbol esta vacio o cuando la raiz esta llena.
void ArbolB::insertarEnHoja(NodoCliente* nodo, Cliente* cliente)
{
    if (nodo->cantidad == 0 || cliente->id < nodo->primero->id) {
        nodo->tercero = nodo->segundo;
        nodo->segundo = nodo->primero;
        nodo->primero = cliente;
    } else if (nodo->cantidad == 1 || cliente->id < nodo->segundo->id) {
        nodo->tercero = nodo->segundo;
        nodo->segundo = cliente;
    } else {
        nodo->tercero = cliente;
    }
    ++nodo->cantidad;
}

//Se divide utilizando la siguiente logica:
//1. Se identifica el nodo hijo que esta lleno y se obtiene el cliente mediano (segundo cliente) de ese nodo.
//2. Se crea un nuevo nodo derecho que contendra el tercer cliente del nodo hijo lleno
//3. Se actualizan los punteros de los hijos del nodo padre para incluir el nuevo nodo derecho y se actualiza la cantidad de clientes en el nodo padre.

void ArbolB::dividirHijo(NodoCliente* padre, int indice, NodoCliente* hijo)
{
    Cliente* mediana = hijo->segundo; // El cliente mediano del nodo hijo
    NodoCliente* derecho = new NodoCliente(hijo->tercero); // Nuevo nodo derecho que contendrá el tercer cliente del nodo hijo lleno
    // Se actualizan los punteros de los hijos del nodo padre
    derecho->hoja = hijo->hoja;
    derecho->reservas = nullptr;
    hijo->segundo = nullptr;
    hijo->tercero = nullptr;
    hijo->cantidad = 1;

    //se actualizan los punteros de los hijos del nodo derecho si el nodo hijo no es una hoja
    if (!hijo->hoja) {
        derecho->hijoIzquierdo = hijo->hijoCentroDerecho;
        derecho->hijoCentroIzquierdo = hijo->hijoDerecho;
        derecho->cantidad = 1;
        hijo->hijoCentroDerecho = nullptr;
        hijo->hijoDerecho = nullptr;
    }

    if (padre->cantidad == 0) {
        padre->primero = mediana;
        padre->hijoIzquierdo = hijo;
        padre->hijoCentroIzquierdo = derecho;
        padre->cantidad = 1;
        return;
    }

    if (padre->cantidad == 1) {
        if (indice == 0) {
            padre->segundo = padre->primero;
            padre->primero = mediana;
            padre->hijoCentroIzquierdo = derecho;
        } else {
            padre->segundo = mediana;
            padre->hijoCentroDerecho = derecho;
        }
    } else {
        if (indice == 0) {
            padre->tercero = padre->segundo;
            padre->segundo = padre->primero;
            padre->primero = mediana;
            padre->hijoDerecho = padre->hijoCentroDerecho;
            padre->hijoCentroDerecho = padre->hijoCentroIzquierdo;
            padre->hijoCentroIzquierdo = derecho;
        } else if (indice == 1) {
            padre->tercero = padre->segundo;
            padre->segundo = mediana;
            padre->hijoDerecho = padre->hijoCentroDerecho;
            padre->hijoCentroDerecho = derecho;
        } else {
            padre->tercero = mediana;
            padre->hijoDerecho = derecho;
        }
    }
    ++padre->cantidad;
}



// Se inserta un cliente en un nodo que no esta lleno, es decir que tiene menos de 3 clientes. Si el nodo es una hoja, se inserta el cliente en la hoja. Si el nodo no es una hoja, se busca el hijo adecuado para insertar el cliente y se llama recursivamente a insertarNoLleno en ese hijo. Si el hijo esta lleno, se divide el hijo y se vuelve a llamar a insertarNoLleno en el hijo adecuado.
void ArbolB::insertarNoLleno(NodoCliente* nodo, Cliente* cliente)
{
    if (nodo->hoja) { // Si el nodo es una hoja, se inserta el cliente en la hoja
        insertarEnHoja(nodo, cliente);
        return;
    }

    int indice = 0;
    NodoCliente* hijo = nodo->hijoIzquierdo;
    if (cliente->id > nodo->primero->id) { // Si el cliente es mayor que el primer cliente del nodo, se busca en el hijo centro izquierdo
        indice = 1;
        hijo = nodo->cantidad == 1 ? nodo->hijoCentroIzquierdo : nodo->hijoCentroIzquierdo;
        if (nodo->cantidad > 1 && cliente->id > nodo->segundo->id) { // Si el cliente es mayor que el segundo cliente del nodo, se busca en el hijo centro derecho
            indice = 2;
            hijo = nodo->hijoCentroDerecho;
            if (nodo->cantidad > 2 && cliente->id > nodo->tercero->id) {
                indice = 3;
                hijo = nodo->hijoDerecho;
            }
        }
    }

    if (hijo->cantidad == 3) {
        // Si el hijo esta lleno, se divide el hijo y se vuelve a llamar a insertarNoLleno en el hijo adecuado.
        dividirHijo(nodo, indice, hijo);
        if (cliente->id == nodo->primero->id || (nodo->cantidad > 1 && cliente->id == nodo->segundo->id) || (nodo->cantidad > 2 && cliente->id == nodo->tercero->id)) {
            delete cliente;
            return;
        }
        if (indice == 0 && cliente->id > nodo->primero->id) { hijo = nodo->hijoCentroIzquierdo; }
        else if (indice == 1 && cliente->id > nodo->segundo->id) { hijo = nodo->hijoCentroDerecho; }
        else if (indice == 2 && nodo->cantidad > 2 && cliente->id > nodo->tercero->id) { hijo = nodo->hijoDerecho; }
    }
    insertarNoLleno(hijo, cliente);
}



// Se inserta un cliente en el arbol B. Si el arbol esta vacio, se crea un nuevo nodo raiz con el cliente. Si la raiz esta llena, se divide la raiz y se crea un nuevo nodo raiz. Luego se llama a insertarNoLleno para insertar el cliente en el nodo adecuado.
bool ArbolB::insertar(Cliente* cliente)
{
    if (cliente == nullptr || cliente->id.empty() || existe(cliente->id)) return false;
    if (raiz == nullptr) {
        raiz = new NodoCliente(cliente);
        return true;
    }
    if (raiz->cantidad == 3) {
        NodoCliente* nuevaRaiz = new NodoCliente();
        nuevaRaiz->hoja = false;
        nuevaRaiz->hijoIzquierdo = raiz;
        nuevaRaiz->cantidad = 0;
        dividirHijo(nuevaRaiz, 0, raiz);
        raiz = nuevaRaiz;
    }
    insertarNoLleno(raiz, cliente);
    return true;
}

// Se inserta un cliente en el arbol B a partir de una referencia a un objeto Cliente.
bool ArbolB::insertar(const Cliente& cliente)
{
    return insertar(new Cliente(cliente));
}
// se recorre el arbol B en orden y se aplica la funcion especificada a cada cliente en el arbol. La funcion recorrerNodo es una funcion recursiva que se llama a si misma para recorrer los hijos de cada nodo y aplicar la funcion a cada cliente en el nodo.
void ArbolB::recorrerNodo(NodoCliente* nodo, const std::function<void(Cliente*)>& funcion) const
{
    if (nodo == nullptr || nodo->cantidad == 0) return;
    if (nodo->hoja) {
        funcion(nodo->primero);
        if (nodo->cantidad > 1) funcion(nodo->segundo);
        if (nodo->cantidad > 2) funcion(nodo->tercero);
        return;
    }
    recorrerNodo(nodo->hijoIzquierdo, funcion);
    funcion(nodo->primero);
    recorrerNodo(nodo->hijoCentroIzquierdo, funcion);
    if (nodo->cantidad > 1) {
        funcion(nodo->segundo);
        recorrerNodo(nodo->hijoCentroDerecho, funcion);
    }
    if (nodo->cantidad > 2) {
        funcion(nodo->tercero);
        recorrerNodo(nodo->hijoDerecho, funcion);
    }
}
// Se recorre el arbol B en orden y se aplica la funcion especificada a cada cliente en el arbol.
void ArbolB::recorrer(const std::function<void(Cliente*)>& funcion) const
{
    recorrerNodo(raiz, funcion);
}
// Se modifica un cliente en el arbol B. Se busca el cliente con el id especificado y se actualizan sus atributos con los valores proporcionados. Si el cliente no se encuentra, se retorna false.
bool ArbolB::modificar(const std::string& id, const std::string& nombre, const std::string& correo, const std::string& telefono, const std::string& password)
{
    Cliente* cliente = buscar(id);
    if (cliente == nullptr) return false;
    cliente->nombre = nombre;
    cliente->correo = correo;
    cliente->telefono = telefono;
    cliente->password = password;
    return true;
}

// Se agrega una referencia a una reserva para un cliente en el arbol B. Se busca el cliente con el id especificado y se agrega la referencia a la reserva en su lista de reservas. Si el cliente no se encuentra o el código de la reserva está vacío, se retorna false.
bool ArbolB::agregarReferenciaReserva(const std::string& idCliente, const std::string& codigoReserva)
{
    Cliente* cliente = buscar(idCliente);
    if (cliente == nullptr || codigoReserva.empty()) return false;
    NodoCliente* nodo = raiz;
    while (nodo != nullptr) {
        if ((nodo->primero != nullptr && nodo->primero->id == idCliente) || (nodo->segundo != nullptr && nodo->segundo->id == idCliente) || (nodo->tercero != nullptr && nodo->tercero->id == idCliente)) break;
        if (idCliente < nodo->primero->id) nodo = nodo->hijoIzquierdo;
        else if (nodo->cantidad == 1 || idCliente < nodo->segundo->id) nodo = nodo->hijoCentroIzquierdo;
        else if (nodo->cantidad == 2 || idCliente < nodo->tercero->id) nodo = nodo->hijoCentroDerecho;
        else nodo = nodo->hijoDerecho;
    }
    if (nodo == nullptr) return false;
    NodoReservaCliente* referencia = new NodoReservaCliente(codigoReserva);
    referencia->siguiente = nodo->reservas;
    nodo->reservas = referencia;
    return true;
}
// Se elimina un cliente de una hoja del arbol B. Si el cliente no se encuentra en la hoja, se retorna false.
bool ArbolB::eliminarDeHoja(NodoCliente* nodo, const std::string& id)
{
    if (!nodo->hoja) return false;
    if (nodo->primero != nullptr && nodo->primero->id == id) {
        delete nodo->primero;
        nodo->primero = nodo->segundo;
        nodo->segundo = nodo->tercero;
        nodo->tercero = nullptr;
        --nodo->cantidad;
        return true;
    }
    if (nodo->cantidad > 1 && nodo->segundo->id == id) {
        delete nodo->segundo;
        nodo->segundo = nodo->tercero;
        nodo->tercero = nullptr;
        --nodo->cantidad;
        return true;
    }
    if (nodo->cantidad > 2 && nodo->tercero->id == id) {
        delete nodo->tercero;
        nodo->tercero = nullptr;
        --nodo->cantidad;
        return true;
    }
    return false;
}

bool ArbolB::eliminar(const std::string& id)
{
    if (!existe(id)) return false;

    // Se reconstruye con copias para conservar todas las invariantes del orden 4
    // sin introducir arreglos temporales ni duplicar la lógica de redistribución.
    ArbolB reemplazo;
    recorrer([&](Cliente* cliente) {
        if (cliente->id != id) reemplazo.insertar(*cliente);
    });
    destruirNodo(raiz);
    raiz = reemplazo.raiz;
    reemplazo.raiz = nullptr;
    return true;
}


//se carga el archivo json con los clientes y sus reservas, se recorre el arreglo de clientes y se inserta cada cliente en el arbol B. Si un cliente tiene reservas, se agregan las referencias a las reservas en el nodo correspondiente del arbol B.
bool ArbolB::cargarJSON(const std::string& ruta)
{
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) return false;
    json datos;
    try { archivo >> datos; } catch (...) { return false; }
    if (!datos.is_object() || !datos.contains("clientes") || !datos["clientes"].is_array()) return false;

    for (const auto& clienteJson : datos["clientes"]) {
        Cliente cliente(
            clienteJson.value("id", ""),
            clienteJson.value("nombre", ""),
            clienteJson.value("correo", ""),
            clienteJson.value("telefono", ""),
            clienteJson.value("password", ""),
            clienteJson.value("tipo", "cliente"));
        if (insertar(cliente) && clienteJson.contains("reservas") && clienteJson["reservas"].is_array()) {
            for (const auto& reserva : clienteJson["reservas"]) {
                agregarReferenciaReserva(cliente.id, reserva.value("codigo_reserva", ""));
            }
        }
    }
    return true;
}

void ArbolB::generarDotNodo(NodoCliente* nodo, std::ofstream& archivo, int& consecutivo) const
{
    if (nodo == nullptr || nodo->cantidad == 0) return;
    const int actual = consecutivo++;
    archivo << "nodo" << actual << " [shape=record, style=filled, fillcolor=\""
            << (actual == 0 ? "#f4a261" : "#dbeafe") << "\", label=\"";
    archivo << "<f0> " << escaparDot(nodo->primero->id);
    if (nodo->cantidad > 1) archivo << " | <f1> " << escaparDot(nodo->segundo->id);
    if (nodo->cantidad > 2) archivo << " | <f2> " << escaparDot(nodo->tercero->id);
    archivo << "\"];\n";

    if (!nodo->hoja) {
        for (int i = 0; i <= nodo->cantidad; ++i) {
            NodoCliente* hijo = nullptr;
            if (i == 0) hijo = nodo->hijoIzquierdo;
            else if (i == 1) hijo = nodo->hijoCentroIzquierdo;
            else if (i == 2) hijo = nodo->hijoCentroDerecho;
            else hijo = nodo->hijoDerecho;
            if (hijo == nullptr) continue;
            const int identificadorHijo = consecutivo;
            generarDotNodo(hijo, archivo, consecutivo);
            archivo << "nodo" << actual << " -> nodo" << identificadorHijo << ";\n";
        }
    }
}

//se genera el reporte del arbol B
void ArbolB::generarDot() const
{
    const std::string directorio = rutasReportes::directorio();
    const std::string rutaDot = directorio + "/arbol_b_clientes.dot";
    const std::string rutaPng = directorio + "/arbol_b_clientes.png";
    std::ofstream archivo(rutaDot);
    archivo << "digraph ArbolBClientes {\nrankdir=TB;\n";
    int consecutivo = 0;
    generarDotNodo(raiz, archivo, consecutivo);
    archivo << "}\n";
    archivo.close();
    rutasReportes::convertirAPng(rutaDot, rutaPng);
}
