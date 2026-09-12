#ifndef ARBOLAVL_H
#define ARBOLAVL_H

#include <string>
#include <fstream>
#include <functional>
#include "../modelos/NodoFuncion.h"

// Arbol binario de busqueda para almacenar peliculas, ordenado por codigo

class ArbolAVL {
private:

    NodoFuncion* raiz; //El nodo raiz del arbol

    NodoFuncion* insertarNodo(NodoFuncion* nodo, MatrizCine* funcion); //Inserta un nodo en el árbol
    NodoFuncion* buscarNodo(NodoFuncion* nodo, const std::string& codigo); // Busca un nodo en el arbol por el codigo de la pelicula
    NodoFuncion* eliminarNodo(NodoFuncion* nodo, const std::string& codigo); // Elimina un nodo y reestructura el arbol

    int altura(NodoFuncion* nodo); // Retorna la altura de un nodo
    bool elementoExiste(const NodoFuncion* nodo, const std::string& codigo); // Verifica si un nodo existe en el arbol
    void rotacionSimpleIzquierda(NodoFuncion*& nodo); // Rotación simple a la izquierda
    void rotacionSimpleDerecha(NodoFuncion*& nodo); // Rotación simple a la derecha
    void rotacionDobleIzquierda(NodoFuncion*& nodo); // Rotación doble a la izquierda
    void rotacionDobleDerecha(NodoFuncion*& nodo); // Rotación doble a la derecha
    
    
    
    void inOrdenAux(NodoFuncion* nodo);
    void inOrdenAux(NodoFuncion* nodo, const std::function<void(MatrizCine*)>& funcion);
    void generarDotAux(NodoFuncion* nodo, std::ofstream& archivo);
    void eliminarNodos(NodoFuncion* nodo);
    std::string quitarComillas(const std::string& campo);
    std::string colorSegunFecha(const std::string& fechaInicioCartelera, const std::string& fechaFinCartelera); // Color del nodo segun la diferencia en dias entre inicio y fin

public:
    ArbolAVL();
    ArbolAVL(const ArbolAVL&) = delete;
    ArbolAVL& operator=(const ArbolAVL&) = delete;
    bool estaVacio();
    bool CodigoExiste(const std::string& codigo);
    void insertar(std::string codigo, std::string titulo, std::string genero, int duracion, std::string clasificacion, std::string idioma, std::string fechaEstreno, std::string fechaFinCartelera);
    void eliminarConCodigo(const std::string& codigo);
    MatrizCine* buscar(const std::string& codigo);
    bool cargarCSV(const std::string& ruta);
    void inOrden(); // imprime por consola (como antes)
    void inOrden(const std::function<void(MatrizCine*)>& funcion); // recorre y ejecuta funcion() con cada matriz de cine (ej. llenar una tabla)
    void generarDot();
    ~ArbolAVL();
};

#endif // ARBOLAVL_H