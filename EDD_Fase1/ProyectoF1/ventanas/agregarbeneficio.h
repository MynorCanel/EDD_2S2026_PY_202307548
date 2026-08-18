#ifndef AGREGARBENEFICIO_H
#define AGREGARBENEFICIO_H
#include "../servicios/guardarDatosService.h"
#include <QMainWindow>

namespace Ui {
class agregarBeneficio;
}

class agregarBeneficio : public QMainWindow
{
    Q_OBJECT

public:
    explicit agregarBeneficio(guardarDatosService* servicio, QWidget *parent = nullptr);
    ~agregarBeneficio();

    void setCodigoPromocion(const std::string& codigo);

signals:
    void beneficioGuardado();

private slots:
    void on_botonRegresar_clicked();

    void on_botonAgregarBeneficio_clicked();

private:
    Ui::agregarBeneficio *ui;
    guardarDatosService *guardar;
    std::string codigoPromocion;
};

#endif // AGREGARBENEFICIO_H
