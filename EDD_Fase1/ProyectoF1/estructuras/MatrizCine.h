#ifndef MATRIZCINE_H
#define MATRIZCINE_H

#include <iostream>
#include <string>

// Nodo adaptado para representar un asiento o una cabecera
class Nodo {
public:
    std::string fila;
    std::string columna;
    std::string valor; // Guardará el nombre de quien reserva el asiento
    Nodo* arriba;
    Nodo* abajo;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(std::string f = "", std::string c = "", std::string v = "");
};

class MatrizCine {
private:
    Nodo* cabeza;
    std::string pelicula;
    std::string horario;
    std::string sala;
    int totalFilas;
    int totalColumnas;
    int asientosOcupados;

    // Métodos privados para manejo interno de punteros
    Nodo* crearCabeceraVertical(std::string fila);
    Nodo* crearCabeceraHorizontal(std::string columna);
    Nodo* buscarCabeceraVertical(std::string fila);
    Nodo* buscarCabeceraHorizontal(std::string columna);
    void eliminarTodosLosNodos(Nodo* nodo);

public:
    MatrizCine();
    ~MatrizCine();

    // 1. Método para inicializar el tamaño de la matriz
    // Recibe la cantidad de filas, columnas y la película de la función
    void inicializarSala(int filas, int columnas, std::string nombrePelicula, std::string horario, std::string sala);

    // 2. Método para reservar un asiento con manejo de errores
    bool reservarAsiento(std::string nombre, std::string fila, std::string columna);

    // 3. Generación del reporte visual adaptado al diseño de la imagen
    bool generarGraphviz();

    // 4. Métodos auxiliares para proyectar la matriz en UI
    int obtenerTotalFilas() const;
    int obtenerTotalColumnas() const;
    std::string obtenerValorAsiento(const std::string& fila, const std::string& columna);
};

#endif