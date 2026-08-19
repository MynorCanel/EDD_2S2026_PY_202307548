#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include <QDialog>
#include "crearpeli.h"
#include "agregarpromocion.h"
#include "agregarbeneficio.h"
#include "../servicios/guardarDatosService.h"


extern std::string codigoPromocion; // Variable global para almacenar el código de promoción

namespace Ui {
class Dialog;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();
    


private slots:
    void on_botonAgregar_clicked();

    void on_botonEliminar_clicked();

    void on_botonCargarCSV_clicked();

    void actualizarTabla(); // Nueva función para actualizar la tabla de películas
    void on_botonAgregarBeneficio_clicked();

    void on_botonAgregarPromocion_clicked();
    void actualizarTreePromociones();

    void on_botonAgregarPromocion_2_clicked();

    void on_botonCrearFuncion_clicked();

    void on_botonReservarAsiento_clicked();
    void actualizarTablaFuncion();

private:
    Ui::Dialog *ui;
    CrearPeli *crearPeli; // Instancia única persistente de CrearPeli
    agregarPromocion *ventanaPromocion;
    agregarBeneficio *ventanaBeneficio;
    guardarDatosService guardar; // Instancia de la clase guardarDatosService compartida con CrearPeli
};

#endif // ADMIN_H

