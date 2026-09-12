#ifndef CLIENTE_H
#define CLIENTE_H
#include "../servicios/guardarDatosService.h"

#include <QMainWindow>

namespace Ui {
class cliente;
}

class cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit cliente(guardarDatosService& servicio, QWidget *parent = nullptr);
    ~cliente();
    void setNombreCliente(const std::string& nombre);

    void actualizarTabla();

private slots:
    void on_botonBuscarPeli_5_clicked();

    void on_botonSalir_clicked();

private:
    guardarDatosService& guardar;
    Ui::cliente *ui;
    std::string nombreCliente;
};

#endif // CLIENTE_H
