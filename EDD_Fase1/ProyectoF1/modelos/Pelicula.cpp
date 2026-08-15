#include <iostream>


struct Pelicula {
    int id;
    std::string titulo;
    std::string genero;
    double duracion;
    std::string clasificacion;
    std::string idioma;
    std::string fechaEstreno;
    std::string fechaFinCartelera;


    Pelicula(int id, const std::string& t, const std::string& g, double d, const std::string& c, const std::string& i, const std::string& f1, const std::string& f2) // Constructor
        : id(id), titulo(t), genero(g), duracion(d), clasificacion(c), idioma(i), fechaEstreno(f1), fechaFinCartelera(f2) {} 
        

};