#include "MatrizCine.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

// Constructor del Nodo
Nodo::Nodo(string f, string c, string v) : fila(f), columna(c), valor(v), arriba(nullptr), abajo(nullptr), izquierda(nullptr), derecha(nullptr) {}

// Constructor de la Matriz
MatrizCine::MatrizCine() {
    cabeza = new Nodo("RAIZ", "RAIZ", "RAIZ");
    totalFilas = 0;
    totalColumnas = 0;
    asientosOcupados = 0;
}

// Destructor
MatrizCine::~MatrizCine() {
    eliminarTodosLosNodos(cabeza);
}

// Liberación de memoria con punteros (Evita memory leaks)
void MatrizCine::eliminarTodosLosNodos(Nodo* nodo) {
    if (nodo == nullptr) return;

    Nodo* filaActual = nodo->abajo;
    while (filaActual != nullptr) {
        Nodo* siguienteFila = filaActual->abajo;
        Nodo* celda = filaActual->derecha;
        while (celda != nullptr) {
            Nodo* siguienteCelda = celda->derecha;
            delete celda;
            celda = siguienteCelda;
        }
        delete filaActual;
        filaActual = siguienteFila;
    }

    Nodo* columnaActual = nodo->derecha;
    while (columnaActual != nullptr) {
        Nodo* siguienteColumna = columnaActual->derecha;
        delete columnaActual;
        columnaActual = siguienteColumna;
    }
    delete nodo;
}

// Inicializa las cabeceras base de la matriz simulando la sala
void MatrizCine::inicializarSala(int filas, int columnas, string nombrePelicula, string horario, string sala) {
    this->pelicula = nombrePelicula;
    this->horario = horario;
    this->sala = sala;
    this->totalFilas = filas;
    this->totalColumnas = columnas;
    this->asientosOcupados = 0;

    // Genera cabeceras de fila en orden alfabético (A, B, C...)
    for (int i = 0; i < filas; i++) {
        string f = "";
        f += (char)('A' + i);
        crearCabeceraVertical(f);
    }
    // Genera cabeceras de columna numéricas (1, 2, 3...)
    for (int j = 1; j <= columnas; j++) {
        crearCabeceraHorizontal(to_string(j));
    }
}

// Búsqueda de cabecera de fila
Nodo* MatrizCine::buscarCabeceraVertical(string f) {
    Nodo* temp = cabeza->abajo;
    while (temp != nullptr) {
        if (temp->fila == f) return temp;
        temp = temp->abajo;
    }
    return nullptr;
}

// Búsqueda de cabecera de columna
Nodo* MatrizCine::buscarCabeceraHorizontal(string c) {
    Nodo* temp = cabeza->derecha;
    while (temp != nullptr) {
        if (temp->columna == c) return temp;
        temp = temp->derecha;
    }
    return nullptr;
}

// Creación e inserción ordenada de cabecera de fila
Nodo* MatrizCine::crearCabeceraVertical(string f) {
    Nodo* nuevoNodo = buscarCabeceraVertical(f);
    if (nuevoNodo != nullptr) return nuevoNodo;

    nuevoNodo = new Nodo(f, "", "");
    Nodo* temp = cabeza;

    while (temp->abajo != nullptr) {
        if (temp->abajo->fila > f) break;
        temp = temp->abajo;
    }
    nuevoNodo->abajo = temp->abajo;
    if (temp->abajo != nullptr) temp->abajo->arriba = nuevoNodo;
    temp->abajo = nuevoNodo;
    nuevoNodo->arriba = temp;
    return nuevoNodo;
}

// Creación e inserción ordenada de cabecera de columna
Nodo* MatrizCine::crearCabeceraHorizontal(string c) {
    Nodo* nuevoNodo = buscarCabeceraHorizontal(c);
    if (nuevoNodo != nullptr) return nuevoNodo;

    nuevoNodo = new Nodo("", c, "");
    Nodo* temp = cabeza;

    while (temp->derecha != nullptr) {
        int colActual = atoi(temp->derecha->columna.c_str());
        int colNueva = atoi(c.c_str());
        if (colActual > colNueva) break;
        temp = temp->derecha;
    }
    nuevoNodo->derecha = temp->derecha;
    if (temp->derecha != nullptr) temp->derecha->izquierda = nuevoNodo;
    temp->derecha = nuevoNodo;
    nuevoNodo->izquierda = temp;
    return nuevoNodo;
}

// Función principal para la reserva del asiento (inserción del nodo interno)
bool MatrizCine::reservarAsiento(string nombre, string f, string c) {
    Nodo* cabeceraFila = buscarCabeceraVertical(f);
    Nodo* cabeceraCol = buscarCabeceraHorizontal(c);

    // Manejo de Error: Asiento fuera de los límites inicializados
    if (cabeceraFila == nullptr || cabeceraCol == nullptr) {
        cout << "Error: La fila o columna ingresada no existe en esta sala." << endl;
        return false;
    }

    // Manejo de Error: Asiento ya ocupado
    Nodo* verificador = cabeceraFila->derecha;
    while (verificador != nullptr) {
        if (verificador->columna == c) {
            cout << "Error: El asiento " << f << c << " ya esta ocupado por " << verificador->valor << "." << endl;
            return false;
        }
        verificador = verificador->derecha;
    }

    // Creación del nuevo asiento ocupado
    Nodo* nuevoNodo = new Nodo(f, c, nombre);

    // Enlace Horizontal (Fila)
    Nodo* tempFila = cabeceraFila;
    while (tempFila->derecha != nullptr) {
        int colActual = atoi(tempFila->derecha->columna.c_str());
        int colNueva = atoi(c.c_str());
        if (colActual > colNueva) break;
        tempFila = tempFila->derecha;
    }
    nuevoNodo->derecha = tempFila->derecha;
    if (tempFila->derecha != nullptr) tempFila->derecha->izquierda = nuevoNodo;
    tempFila->derecha = nuevoNodo;
    nuevoNodo->izquierda = tempFila;

    // Enlace Vertical (Columna)
    Nodo* tempCol = cabeceraCol;
    while (tempCol->abajo != nullptr) {
        if (tempCol->abajo->fila > f) break;
        tempCol = tempCol->abajo;
    }
    nuevoNodo->abajo = tempCol->abajo;
    if (tempCol->abajo != nullptr) tempCol->abajo->arriba = nuevoNodo;
    tempCol->abajo = nuevoNodo;
    nuevoNodo->arriba = tempCol;

    asientosOcupados++;
    cout << "Reserva Exitosa: Asiento " << f << c << " asignado a " << nombre << "." << endl;
    return true;
}

// Genera un diagrama de Graphviz de la matriz de asientos, mostrando solo los asientos ocupados
bool MatrizCine::generarGraphviz() {
    FILE* fp = fopen("MatrizFuncion.dot", "w");
    if (fp == nullptr) return false;

    fprintf(fp, "digraph MatrizCine {\n");
    fprintf(fp, "rankdir=TB;\n");
    fprintf(fp, "splines=ortho;\n");
    fprintf(fp, "bgcolor=\"#f8fafc\";\n");
    fprintf(fp, "pad=0.6;\n");
    fprintf(fp, "nodesep=0.6;\n");
    fprintf(fp, "ranksep=0.9;\n");
    fprintf(fp, "label=\"Matriz de Asientos\";\n");
    fprintf(fp, "labelloc=t;\n");
    fprintf(fp, "fontname=\"Arial\";\n");
    fprintf(fp, "node [shape=box, style=\"rounded,filled\", fontname=\"Arial\", fontsize=11, color=\"#1f2937\"];\n");
    fprintf(fp, "edge [color=\"#94a3b8\", arrowsize=0.8, penwidth=1.2];\n");

    // Nodo raíz
    string labelRaiz = pelicula.empty() ? "Funcion" : pelicula;
    fprintf(fp, "Raiz [shape=diamond, label=\"%s\", width=1.5, height=0.6, style=\"rounded,filled\", fillcolor=\"#0f172a\", fontcolor=white, fontsize=13, group=0, color=\"#020617\", penwidth=2];\n", labelRaiz.c_str());

    // Nivelación cabeceras de columnas
    fprintf(fp, "{ rank=same; Raiz; ");
    for (int c = 1; c <= totalColumnas; c++) {
        fprintf(fp, "Col%d; ", c);
    }
    fprintf(fp, "}\n");

    // Imprimir cabeceras de columnas
    for (int c = 1; c <= totalColumnas; c++) {
        fprintf(fp, "Col%d [label=\"%d\", style=\"rounded,filled\", fillcolor=\"#2563eb\", fontcolor=white, fontsize=12, group=%d, color=\"#1d4ed8\", penwidth=1.5];\n", c, c, c);
    }

    // Imprimir cabeceras de filas (verdes, letras)
    for (int r = 0; r < totalFilas; r++) {
        char letraFila = 'A' + r;
        fprintf(fp, "Fila%c [label=\"%c\", style=\"rounded,filled\", fillcolor=\"#059669\", fontcolor=white, fontsize=12, group=0, color=\"#047857\", penwidth=1.5];\n", letraFila, letraFila);
    }

    // Imprimir matriz visual solo con asientos ocupados
    for (int r = 0; r < totalFilas; r++) {
        char letraFila = 'A' + r;
        string filaStr(1, letraFila);
        fprintf(fp, "{ rank=same; Fila%c; ", letraFila);

        for (int c = 1; c <= totalColumnas; c++) {
            string colStr = to_string(c);
            Nodo* asiento = nullptr;
            Nodo* cabeceraFila = buscarCabeceraVertical(filaStr);
            
            if (cabeceraFila != nullptr) {
                Nodo* temp = cabeceraFila->derecha;
                while (temp != nullptr) {
                    if (temp->columna == colStr) {
                        asiento = temp;
                        break;
                    }
                    temp = temp->derecha;
                }
            }

            if (asiento != nullptr) {
                const char* colorNodo = ((r + c) % 2 == 0) ? "#fef3c7" : "#fde68a";
                fprintf(fp, "Nodo%c%d [label=\"%s\", style=\"rounded,filled\", fillcolor=\"%s\", fontcolor=\"#1f2937\", group=%d, color=\"#d97706\", penwidth=1.4];\n", letraFila, c, asiento->valor.c_str(), colorNodo, c);
                fprintf(fp, "Nodo%c%d; ", letraFila, c);
            }
        }
        fprintf(fp, "}\n");
    }

    // Enlaces de cabeceras
    if (totalFilas > 0) fprintf(fp, "Raiz -> FilaA [color=\"#0f172a\", penwidth=2, arrowhead=normal];\n");
    if (totalColumnas > 0) fprintf(fp, "Raiz -> Col1 [color=\"#0f172a\", penwidth=2, arrowhead=normal];\n");

    for (int c = 1; c < totalColumnas; c++) fprintf(fp, "Col%d -> Col%d [color=\"#2563eb\", penwidth=2, arrowhead=normal];\n", c, c + 1);
    for (int r = 0; r < totalFilas - 1; r++) fprintf(fp, "Fila%c -> Fila%c [color=\"#059669\", penwidth=2, arrowhead=normal];\n", 'A' + r, 'A' + r + 1);

    // Enlaces HORIZONTALES solo entre asientos ocupados de cada fila
    for (int r = 0; r < totalFilas; r++) {
        char letraFila = 'A' + r;
        string filaStr(1, letraFila);

        int primeraOcupada = 0;
        int anteriorOcupada = 0;

        for (int c = 1; c <= totalColumnas; c++) {
            string colStr = to_string(c);
            Nodo* asiento = nullptr;
            Nodo* cabeceraFila = buscarCabeceraVertical(filaStr);

            if (cabeceraFila != nullptr) {
                Nodo* temp = cabeceraFila->derecha;
                while (temp != nullptr) {
                    if (temp->columna == colStr) {
                        asiento = temp;
                        break;
                    }
                    temp = temp->derecha;
                }
            }

            if (asiento != nullptr) {
                if (primeraOcupada == 0) {
                    primeraOcupada = c;
                }
                anteriorOcupada = c;
            }
        }

        if (primeraOcupada != 0) {
            fprintf(fp, "Fila%c -> Nodo%c%d [color=\"#0ea5e9\", penwidth=1.5, arrowhead=normal];\n", letraFila, letraFila, primeraOcupada);
            fprintf(fp, "Nodo%c%d -> Fila%c [color=\"#0ea5e9\", penwidth=1.5, arrowhead=normal];\n", letraFila, primeraOcupada, letraFila);
        }
    }

    // Enlaces VERTICALES solo entre asientos ocupados de cada columna
    for (int c = 1; c <= totalColumnas; c++) {
        int primeraFilaOcupada = -1;
        int filaAnteriorOcupada = -1;

        for (int r = 0; r < totalFilas; r++) {
            char letraFila = 'A' + r;
            string filaStr(1, letraFila);
            string colStr = to_string(c);

            Nodo* asiento = nullptr;
            Nodo* cabeceraFila = buscarCabeceraVertical(filaStr);
            if (cabeceraFila != nullptr) {
                Nodo* temp = cabeceraFila->derecha;
                while (temp != nullptr) {
                    if (temp->columna == colStr) {
                        asiento = temp;
                        break;
                    }
                    temp = temp->derecha;
                }
            }

            if (asiento != nullptr) {
                if (primeraFilaOcupada == -1) {
                    primeraFilaOcupada = r;
                }
                if (filaAnteriorOcupada != -1) {
                    fprintf(fp, "Nodo%c%d -> Nodo%c%d [color=\"#f97316\", penwidth=1.5, arrowhead=normal];\n", 'A' + filaAnteriorOcupada, c, letraFila, c);
                    fprintf(fp, "Nodo%c%d -> Nodo%c%d [color=\"#f97316\", penwidth=1.5, arrowhead=normal];\n", letraFila, c, 'A' + filaAnteriorOcupada, c);
                }
                filaAnteriorOcupada = r;
            }
        }

        if (primeraFilaOcupada != -1) {
            fprintf(fp, "Col%d -> Nodo%c%d [color=\"#f97316\", penwidth=1.5, arrowhead=normal];\n", c, 'A' + primeraFilaOcupada, c);
            fprintf(fp, "Nodo%c%d -> Col%d [color=\"#f97316\", penwidth=1.5, arrowhead=normal];\n", 'A' + primeraFilaOcupada, c, c);
        }
    }

    // Conexiones horizontales dobles entre ocupados en cada fila
    for (int r = 0; r < totalFilas; r++) {
        char letraFila = 'A' + r;
        string filaStr(1, letraFila);

        int anteriorOcupada = 0;
        for (int c = 1; c <= totalColumnas; c++) {
            string colStr = to_string(c);
            Nodo* asiento = nullptr;
            Nodo* cabeceraFila = buscarCabeceraVertical(filaStr);
            if (cabeceraFila != nullptr) {
                Nodo* temp = cabeceraFila->derecha;
                while (temp != nullptr) {
                    if (temp->columna == colStr) {
                        asiento = temp;
                        break;
                    }
                    temp = temp->derecha;
                }
            }

            if (asiento != nullptr) {
                if (anteriorOcupada != 0) {
                    fprintf(fp, "Nodo%c%d -> Nodo%c%d [color=\"#0ea5e9\", penwidth=1.5, arrowhead=normal];\n", letraFila, anteriorOcupada, letraFila, c);
                    fprintf(fp, "Nodo%c%d -> Nodo%c%d [color=\"#0ea5e9\", penwidth=1.5, arrowhead=normal];\n", letraFila, c, letraFila, anteriorOcupada);
                }
                anteriorOcupada = c;
            }
        }
    }

    // Resumen de estadísticas 
    int total = totalFilas * totalColumnas;
    int libres = total - asientosOcupados;
    fprintf(fp, "Informacion [label=<<TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"5\" COLOR=\"#64748b\" BGCOLOR=\"#eef2ff\">"
                "<TR><TD COLSPAN=\"2\" BGCOLOR=\"#1e293b\" ALIGN=\"CENTER\"><FONT COLOR=\"white\" FACE=\"Arial\"><B>INFORMACION DE LA FUNCION</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Pelicula</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%s</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Horario</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%s</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Sala</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%s</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Asientos libres</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%d</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Asientos ocupados</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%d</B></FONT></TD></TR>"
                "<TR><TD ALIGN=\"LEFT\"><FONT FACE=\"Arial\">Total asientos</FONT></TD><TD ALIGN=\"RIGHT\"><FONT FACE=\"Arial\"><B>%d</B></FONT></TD></TR>"
                "</TABLE>>, shape=plain];\n", pelicula.c_str(), horario.c_str(), sala.c_str(), libres, asientosOcupados, total);

    // Alinear bloque informativo con raíz
    fprintf(fp, "{ rank=same; Raiz; Informacion; }\n");

    fprintf(fp, "}\n");
    fclose(fp);
    int resultado = system("dot -Tpng MatrizFuncion.dot -o MatrizFuncion.png");

    return true;
}

int MatrizCine::obtenerTotalFilas() const {
    return totalFilas;
}

int MatrizCine::obtenerTotalColumnas() const {
    return totalColumnas;
}

// Obtiene el valor del asiento en la posición especificada (fila, columna)
std::string MatrizCine::obtenerValorAsiento(const std::string& fila, const std::string& columna) {
    Nodo* cabeceraFila = buscarCabeceraVertical(fila);
    if (cabeceraFila == nullptr) {
        return "--";
    }

    Nodo* actual = cabeceraFila->derecha;
    while (actual != nullptr) {
        if (actual->columna == columna) {
            return actual->valor;
        }
        actual = actual->derecha;
    }

    return "--";
}