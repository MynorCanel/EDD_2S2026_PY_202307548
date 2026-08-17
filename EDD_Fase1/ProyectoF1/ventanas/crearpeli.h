#ifndef CREARPELI_H
#define CREARPELI_H
#include <QMainWindow>
#include "../servicios/guardarDatosService.h"

namespace Ui {
class CrearPeli;
}

class CrearPeli : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrearPeli(guardarDatosService* servicio, QWidget *parent = nullptr);
    ~CrearPeli();
    
signals:
    void peliculaGuardada();


private slots:
    void on_botonAgregarPeli_clicked();
    void on_botonRegresar_clicked();

private:
    Ui::CrearPeli *ui;
    guardarDatosService* guardar; // Puntero a la instancia compartida de guardarDatosService

};

#endif // CREARPELI_H
