#include "login.h"
#include "ui_login.h"
#include "admin.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_botonIniciarSesion_clicked()
{
    QString correo=ui->textoCorreo->text();
    QString contra=ui->textoContra->text();

    if (correo=="admin.gmail.com" && contra=="admin123"){
        // Iniciar sesión como administrador
        ui->statusbar->showMessage("Iniciando sesión como administrador...");
        //Se abre la ventana de administrador
        admin *adminWindow = new admin();
        adminWindow->show();
        this->close(); // Cierra la ventana de inicio de sesión
    }
}

