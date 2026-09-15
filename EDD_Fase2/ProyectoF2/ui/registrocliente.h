#ifndef REGISTROCLIENTE_H
#define REGISTROCLIENTE_H

#include <QMainWindow>
#include "../servicios/guardarDatosService.h"

namespace Ui {
class registroCliente;
}

class registroCliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit registroCliente(guardarDatosService& servicio, QWidget *parent = nullptr);
    ~registroCliente();

private:
    guardarDatosService& guardar;
    Ui::registroCliente *ui;

signals:
    void registroCompletado();

private slots:
    void on_botonRegistrarse_clicked();
    void on_botonInicio_clicked();
};

#endif // REGISTROCLIENTE_H
