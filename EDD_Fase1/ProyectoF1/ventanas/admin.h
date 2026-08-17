#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "crearpeli.h"
#include "../servicios/guardarDatosService.h"

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
private:
    Ui::Dialog *ui;
    CrearPeli *crearPeli; // Instancia única persistente de CrearPeli
    guardarDatosService guardar; // Instancia de la clase guardarDatosService compartida con CrearPeli
};

#endif // ADMIN_H

