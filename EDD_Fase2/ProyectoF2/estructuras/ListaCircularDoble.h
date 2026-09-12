#ifndef LISTACIRCULARDOBLE_H
#define LISTACIRCULARDOBLE_H

#include <string>
#include <functional>
#include "../modelos/NodoSolicitud.h"
#include "../modelos/EstadoSolicitud.h"

// Lista circular doblemente enlazada de solicitudes.
// Las solicitudes NO se eliminan al aprobarse o rechazarse: solo cambia su
// estado y se quedan almacenadas. "eliminar" borra la solicitud por completo,
// y es una operacion distinta de cambiar su estado.
class ListaCircularDoble {
private:
    NodoSolicitud* cabeza;
    int totalSolicitudes;      // cuenta todas las solicitudes almacenadas, sin importar el estado
    int solicitudesPendientes; // cuenta solo las que estan en espera

    NodoSolicitud* buscarNodo(int numeroSolicitud);
    int generarNumeroUnico();          // numero aleatorio de 4 digitos que no choque con uno existente
    std::string obtenerFechaActual();  // fecha de hoy en formato AAAA-MM-DD

public:
    ListaCircularDoble();
    ListaCircularDoble(const ListaCircularDoble&) = delete;
    ListaCircularDoble& operator=(const ListaCircularDoble&) = delete;
    ~ListaCircularDoble();

    // El numero de solicitud se genera aleatoriamente y la fecha se toma del reloj del sistema;
    // toda solicitud nueva entra en estado "en espera"
    void insertarSolicitud(const std::string& nombreCliente, const std::string& telefonoContacto, const std::string& tipoSolicitud, const std::string& descripcion);

    // Cambia el estado de una solicitud existente (aprobar/rechazar), sin borrarla de la lista
    bool cambiarEstado(int numeroSolicitud, EstadoSolicitud nuevoEstado);

    // Elimina una solicitud por completo de la lista (borrado real)
    void eliminar(int numeroSolicitud);

    int obtenerTotalSolicitudes() const;
    int obtenerSolicitudesPendientes() const;

    // Recorre la lista y ejecuta funcion() con cada solicitud (ej. llenar tarjetas en Qt)
    void recorrer(const std::function<void(Solicitud*)>& funcion);

    void graficar();
    void mostrar();
};

#endif // LISTACIRCULARDOBLE_H