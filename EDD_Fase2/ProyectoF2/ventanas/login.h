#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "../servicios/guardarDatosService.h"

class admin;
class cliente;

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

private:
    Ui::MainWindow *ui;
    guardarDatosService guardarCompartido;
    admin* adminWindow;
    cliente* clienteWindow;
};
#endif // LOGIN_H
