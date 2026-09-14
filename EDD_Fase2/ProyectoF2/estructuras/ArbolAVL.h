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
    void ajustarAVL(NodoFuncion*& nodo); // Ajusta el árbol AVL después de una inserción o eliminación
    
    
    void inOrdenAux(NodoFuncion* nodo);
    void inOrdenAux(NodoFuncion* nodo, const std::function<void(MatrizCine*)>& funcion);
    void generarDotAux(NodoFuncion* nodo, std::ofstream& archivo);
    void eliminarNodos(NodoFuncion* nodo);

public:
    ArbolAVL();
    ArbolAVL(const ArbolAVL&) = delete;
    ArbolAVL& operator=(const ArbolAVL&) = delete;
    bool estaVacio();
    bool CodigoExiste(const std::string& codigo);
    void insertar(std::string codigoFuncion, int filas, int columnas, std::string nombrePelicula, std::string horario, std::string sala);
    MatrizCine* buscar(const std::string& codigo);
    bool eliminar(const std::string& codigo);
    void inOrden(const std::function<void(MatrizCine*)>& funcion);
    void generarDot();
    
    ~ArbolAVL();
};

#endif // ARBOLAVL_H