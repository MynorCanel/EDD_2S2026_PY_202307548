#include <iostream>
#include <fstream>
#include <stdexcept>
#include "../modelos/NodoPromocion.cpp"

using namespace std;
// maneja todos los nodos formando un ciclo
// la cabeza apunta al primer nodo, y el ultimo apunta a la cabeza
class ListaCircular {
private:
    NodoPromocion* cabeza;

public:
    ListaCircular() {
        cabeza = nullptr;
    }

    // inserta un nuevo amigo al final de la lista
    // el ultimo nodo siempre apunta a la cabeza para mantener el ciclo
    void insertar(string codigo, string nombre, string vigencia, string diasAplicables) {
        NodoPromocion* nuevo = new NodoPromocion(new Promocion(codigo, nombre, vigencia, diasAplicables));

        if (cabeza == nullptr) {
            // si esta vacia, el nuevo nodo se apunta a si mismo
            cabeza = nuevo;
            nuevo->siguiente = cabeza;
        } else {
            // recorremos hasta el ultimo nodo (el que apunta a cabeza)
            NodoPromocion* actual = cabeza;
            while (actual->siguiente != cabeza) {
                actual = actual->siguiente;
            }
            // conectamos el ultimo con el nuevo, y el nuevo con la cabeza
            actual->siguiente = nuevo;
            nuevo->siguiente = cabeza;
        }
    }

    // reconecta los punteros para mantener el ciclo cerrado
    void eliminar(string nombre) {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia." << endl;
            return;
        }

        // recorremos el ciclo buscando el nombre
        NodoPromocion* actual = cabeza;
        NodoPromocion* anterior = nullptr;

        do {
            if (actual->promocion -> nombre == nombre) {
                if (actual == cabeza) {
                    // caso especial eliminar la cabeza
                    if (actual->siguiente == cabeza) {
                        // unico nodo en la lista
                        cabeza = nullptr;
                    } else {
                        // buscamos el ultimo nodo para reconectar el ciclo
                        NodoPromocion* ultimo = cabeza;
                        while (ultimo->siguiente != cabeza) {
                            ultimo = ultimo->siguiente;
                        }
                        cabeza = cabeza->siguiente;
                        ultimo->siguiente = cabeza;
                    }
                } else {
                    // saltamos el nodo actual
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                return;
            }
            // avanzamos al siguiente nodo
            anterior = actual;
            actual = actual->siguiente;
        } while (actual != cabeza);

    }

    // genera un archivo .dot y lo compila a png con graphviz
    // dibuja los nodos en orden y una flecha del ultimo al primero
    void graficar() {
        if (cabeza == nullptr) {
            cout << "La lista esta vacia, no hay nada que graficar." << endl;
            return;
        }

        ofstream archivo("simple/lista_circular.dot");
        archivo << "digraph ListaCircular {" << endl;
        archivo << "    rankdir=LR;" << endl;
        archivo << "    node [shape=record, style=filled, fillcolor=\"#eaff06\"];" << endl;

        NodoPromocion* actual = cabeza;
        int i = 0;

        // creamos los nodos del grafo recorriendo el ciclo
        do {
            archivo << "    nodo" << i << " [label=\"{"
                    << actual->promocion->codigo << " | "
                    << actual->promocion->nombre << " | "
                    << actual->promocion->vigencia << " | "
                    << actual->promocion->diasAplicables << " | "
                    << "}\"];" << endl;
            actual = actual->siguiente;
            i++;
        } while (actual != cabeza);

        // flechas entre nodos consecutivos
        for (int j = 0; j < i - 1; j++) {
            archivo << "    nodo" << j << " -> nodo" << j + 1 << ";" << endl;
        }
        // flecha del ultimo nodo al primero 
        archivo << "    nodo" << i - 1 << " -> nodo0;" << endl;

        archivo << "}" << endl;
        archivo.close();

        int resultado = system("dot -Tpng simple/lista_circular.dot -o simple/lista_circular.png");
        if (resultado == 0) {
            cout << "Grafico generado: lista_circular.png" << endl;
        } else {
            cout << "Error: no se pudo generar el grafico. Verifica que Graphviz este instalado." << endl;
        }
    }


    // libera toda la memoria recorriendo el ciclo
    ~ListaCircular() {
        if (cabeza == nullptr) return;
        NodoPromocion* actual = cabeza;
        NodoPromocion* temp;
        do {
            temp = actual;
            actual = actual->siguiente;
            delete temp->promocion; // liberar memoria de la promoción
            delete temp; //Liberar memoria del nodo
        } while (actual != cabeza);
    }
};