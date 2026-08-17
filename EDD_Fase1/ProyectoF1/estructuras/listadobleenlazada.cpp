#include <iostream>
#include <fstream>
#include "../modelos/NodoBeneficio.h"

using namespace std;

class ListaDoble {
private:
    NodoBeneficio* cabeza;  //Puntero al primer nodo de la lista, se inicializa como nullptr

public:
    ListaDoble() {   //Constructor: inicializa la cabeza como nullptr
        cabeza = nullptr;
    }

    //Agregar un auto a la lista
    void agregar(string tipo, string descripcion, string valor) {

        NodoBeneficio* nuevo = new NodoBeneficio(new Beneficio(tipo, descripcion, valor));

        if (cabeza == nullptr) {   //Si la lista esta vacia, el nuevo nodo es la cabeza
            cabeza = nuevo;
        } else {    //De lo contrario, recorremos la lista hasta el final y agregamos el nuevo nodo
            NodoBeneficio* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            //Se conectan ambos nodos, el nuevo nodo apunta al anterior y el anterior apunta al nuevo
            actual->siguiente = nuevo;
            nuevo->anterior = actual;
        }
    }

    //Eliminar un auto de la lista por Tipo de Beneficio
    void eliminar(string tipo) {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        NodoBeneficio* actual = cabeza;
        while (actual != nullptr && actual->beneficio->tipoBeneficio != tipo) {
            actual = actual->siguiente;
        }

        if (actual == nullptr) {
            cout << "No se encontro Beneficio con tipo " << tipo << endl;
            return;
        }

        cout << "Beneficio " << actual->beneficio->tipoBeneficio << " " << actual->beneficio->descripcion << " " << actual->beneficio->valor << " eliminado." << endl;

        // Si es la cabeza, movemos la cabeza
        if (actual == cabeza) {
            cabeza = actual->siguiente;
            if (cabeza != nullptr) {
                cabeza->anterior = nullptr;
            }
        } else {
            // Reconectamos el anterior y el siguiente
            actual->anterior->siguiente = actual->siguiente;
            if (actual->siguiente != nullptr) {
                actual->siguiente->anterior = actual->anterior;
            }
        }

        delete actual;
    }

    static string escaparHtml(const string& texto) { //Escapa caracteres especiales para HTML, se utiliza para mostrar los datos en el grafo de Graphviz
    string resultado;
    resultado.reserve(texto.size());

    for (char caracter : texto) {
        switch (caracter) {
            case '&': resultado += "&amp;"; break;
            case '<': resultado += "&lt;"; break;
            case '>': resultado += "&gt;"; break;
            case '"': resultado += "&quot;"; break;
            default: resultado += caracter; break;
        }
    }

    return resultado;
    }

    // Genera un archivo .dot y lo compila a PNG con Graphviz
    void graficar() {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia, no hay nada que graficar." << endl;
            return;
        }

        ofstream archivo("lista.dot");
        archivo << "digraph ListaDoble {" << endl;
        archivo << "    rankdir=LR;" << endl;
        archivo << "    node [shape=record, style=filled, fillcolor=\"#7ce964\"];" << endl;

        NodoBeneficio* actual = cabeza;
        int i = 0;

        // Creamos los nodos del grafo
        while (actual != nullptr) {
            archivo << "    nodo" << i << " [shape=plain, label=<" << endl;
            archivo << "        <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">" << endl;
            archivo << "            <TR><TD COLSPAN=\"3\"><B>Tipo / Descripcion / Valor</B></TD></TR>" << endl;
            archivo << "            <TR><TD>" << escaparHtml(actual->beneficio->tipoBeneficio) << "</TD>"
                << "<TD>" << escaparHtml(actual->beneficio->descripcion) << "</TD>"
                << "<TD>" << actual->beneficio->valor << "</TD></TR>" << endl;
            archivo << "        </TABLE>>];" << endl;
            actual = actual->siguiente;
            i++;
        }

        // Flechas dobles entre nodos (dir=both)
        for (int j = 0; j < i - 1; j++) {
            archivo << "    nodo" << j << " -> nodo" << j + 1 << " [dir=both];" << endl;
        }

        archivo << "}" << endl;
        archivo.close();

        int resultado = system("dot -Tpng lista.dot -o lista.png");
        if (resultado == 0) {
            cout << "Grafico generado: lista.png" << endl;
        } else {
            cout << "Error: no se pudo generar el grafico. Verifica que Graphviz este instalado." << endl;
        }
    }

    // Muestra la lista en consola de cabeza a cola
    void mostrar() {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        cout << "--- De inicio a fin ---" << endl;
        NodoBeneficio* actual = cabeza;
        while (actual != nullptr) {
            cout << "Tipo: " << actual->beneficio->tipoBeneficio << ", Descripcion: " << actual->beneficio->descripcion << ", Valor: " << actual->beneficio->valor << endl;
            actual = actual->siguiente;
        }
    }

    // Destructor: libera toda la memoria
    ~ListaDoble() {
        NodoBeneficio* actual = cabeza;
        while (actual != nullptr) {
            NodoBeneficio* temp = actual;
            actual = actual->siguiente;
            delete temp->beneficio; // Liberar memoria del beneficio
            delete temp;
        }
    }
};
