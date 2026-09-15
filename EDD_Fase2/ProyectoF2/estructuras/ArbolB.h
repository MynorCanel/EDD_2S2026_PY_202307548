#ifndef ARBOLB_H
#define ARBOLB_H

#include <functional>
#include <fstream>
#include <string>
#include "../modelos/NodoCliente.h"

// Arbol B de orden 4: cada nodo admite hasta 3 clientes y 4 hijos.
class ArbolB {
private:
    NodoCliente* raiz;

    Cliente* buscarNodo(NodoCliente* nodo, const std::string& id) const;
    void destruirNodo(NodoCliente* nodo);
    void recorrerNodo(NodoCliente* nodo, const std::function<void(Cliente*)>& funcion) const;
    void insertarEnHoja(NodoCliente* nodo, Cliente* cliente);
    void dividirHijo(NodoCliente* padre, int indice, NodoCliente* hijo);
    void insertarNoLleno(NodoCliente* nodo, Cliente* cliente);
    void generarDotNodo(NodoCliente* nodo, std::ofstream& archivo, int& consecutivo) const;
    bool eliminarDeHoja(NodoCliente* nodo, const std::string& id);

public:
    static constexpr int ORDEN = 4;

    ArbolB();
    ~ArbolB();
    ArbolB(const ArbolB&) = delete;
    ArbolB& operator=(const ArbolB&) = delete;

    bool estaVacio() const;
    bool existe(const std::string& id) const;
    bool correoExiste(const std::string& correo) const;
    bool insertar(Cliente* cliente);
    bool insertar(const Cliente& cliente);
    Cliente* buscar(const std::string& id) const;
    bool eliminar(const std::string& id);
    bool modificar(const std::string& id, const std::string& nombre, const std::string& correo, const std::string& telefono, const std::string& password);
    bool agregarReferenciaReserva(const std::string& idCliente, const std::string& codigoReserva);
    void recorrer(const std::function<void(Cliente*)>& funcion) const;
    bool cargarJSON(const std::string& ruta);
    void generarDot() const;
};

#endif
