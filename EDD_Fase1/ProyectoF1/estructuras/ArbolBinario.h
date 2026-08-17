#ifndef ARBOLBINARIO_H
#define ARBOLBINARIO_H

#include <string>
#include <fstream>
#include <functional>
#include "../modelos/NodoPelicula.h"

// Arbol binario de busqueda para almacenar peliculas, ordenado por codigo

class ArbolBinario {
private:

    NodoPelicula* raiz; //El nodo raiz del arbol

    NodoPelicula* insertarNodo(NodoPelicula* nodo, Pelicula* pelicula); //Inserta un nodo en el árbol
    NodoPelicula* buscarNodo(NodoPelicula* nodo, const std::string& codigo); // Busca un nodo en el arbol por el codigo de la pelicula
    NodoPelicula* eliminarNodo(NodoPelicula* nodo, const std::string& codigo); // Elimina un nodo y reestructura el arbol

    void inOrdenAux(NodoPelicula* nodo);
    void inOrdenAux(NodoPelicula* nodo, const std::function<void(Pelicula*)>& funcion);
    void generarDotAux(NodoPelicula* nodo, std::ofstream& archivo);
    void eliminarNodos(NodoPelicula* nodo);
    std::string quitarComillas(const std::string& campo);

    std::string colorSegunFecha(const std::string& fechaFinCartelera); // Color del nodo segun cuanto falta para el fin de cartelera
    long diferenciaEnDias(const std::string& fecha); // Dias entre "fecha" (AAAA-MM-DD) y la fecha de ejecucion del programa

public:
    ArbolBinario();
    ArbolBinario(const ArbolBinario&) = delete;
    ArbolBinario& operator=(const ArbolBinario&) = delete;
    bool estaVacio();
    bool CodigoExiste(const std::string& codigo);
    void insertar(std::string codigo, std::string titulo, std::string genero, int duracion, std::string clasificacion, std::string idioma, std::string fechaEstreno, std::string fechaFinCartelera);
    void eliminarConCodigo(const std::string& codigo);
    Pelicula* buscar(const std::string& codigo);
    bool cargarCSV(const std::string& ruta);
    void inOrden(); // imprime por consola (como antes)
    void inOrden(const std::function<void(Pelicula*)>& funcion); // recorre y ejecuta funcion() con cada pelicula (ej. llenar una tabla)
    void generarDot();
    ~ArbolBinario();
};

#endif // ARBOLBINARIO_H