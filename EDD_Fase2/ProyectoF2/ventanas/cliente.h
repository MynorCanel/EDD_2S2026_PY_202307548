#ifndef CLIENTE_H
#define CLIENTE_H
#include "../servicios/guardarDatosService.h"

#include <QMainWindow>
#include <QResizeEvent>

namespace Ui {
class cliente;
}

class cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit cliente(guardarDatosService& servicio, QWidget *parent = nullptr);
    ~cliente();
    void setLoginWindow(QWidget* ventana);
    void setNombreCliente(const std::string& nombre);
    void setCliente(const std::string& id, const std::string& nombre);

    void actualizarTabla();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_botonBuscarPeli_5_clicked();

    void on_botonSalir_clicked();
    void on_botonReservarAsiento_clicked();
    void on_botonCancelarReserva_clicked();
    void actualizarTablaFuncionesCliente();
    void actualizarTablaFuncionCliente();

private:
    guardarDatosService& guardar;
    Ui::cliente *ui;
    std::string nombreCliente;
    std::string idCliente;
    std::string codigoFuncionSeleccionada;
    QWidget* loginWindow;
};

#endif // CLIENTE_H
