#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "../servicios/guardarDatosService.h"

class admin;
class cliente;
class registroCliente;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_botonIniciarSesion_clicked();
    void on_botonRegistrarse_clicked();

private:
    Ui::MainWindow *ui;
    guardarDatosService guardarCompartido;
    admin* adminWindow;
    cliente* clienteWindow;
    registroCliente* registroWindow;
};
#endif // LOGIN_H
