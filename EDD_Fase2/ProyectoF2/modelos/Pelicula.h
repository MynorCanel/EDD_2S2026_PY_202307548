#ifndef PELICULA_H
#define PELICULA_H

#include <string>

struct Pelicula {
    std::string id;
    std::string titulo;
    std::string genero;
    int duracion;
    std::string clasificacion;
    std::string idioma;
    std::string fechaEstreno;
    std::string fechaFinCartelera;



    Pelicula(const std::string& id, const std::string& t, const std::string& g, double d, const std::string& c, const std::string& i, const std::string& f1, const std::string& f2) // Constructor
        : id(id), titulo(t), genero(g), duracion(d), clasificacion(c), idioma(i), fechaEstreno(f1), fechaFinCartelera(f2) {}


};

#endif // PELICULA_H