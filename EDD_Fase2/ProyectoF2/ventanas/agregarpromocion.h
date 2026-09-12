#ifndef AGREGARPROMOCION_H
#define AGREGARPROMOCION_H
#include "../servicios/guardarDatosService.h"

#include <QMainWindow>

namespace Ui {
class agregarPromocion;
}

class agregarPromocion : public QMainWindow
{
    Q_OBJECT

public:
    explicit agregarPromocion(guardarDatosService* servicio, QWidget *parent = nullptr);
    ~agregarPromocion();

signals:
    void promocionGuardada();

private slots:
    void on_botonAgregarPromo_clicked();

    void on_botonRegresar_clicked();

private:
    Ui::agregarPromocion *ui;
    guardarDatosService* guardar;
};

#endif // AGREGARPROMOCION_H
