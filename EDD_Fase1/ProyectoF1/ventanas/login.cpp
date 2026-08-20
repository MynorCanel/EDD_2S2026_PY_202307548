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

    if (correo.isEmpty() || contra.isEmpty()){
        ui->statusbar->showMessage("Ambos campos son obligatorios. Por favor, ingrese su correo y contraseñaaaaaa");
        return;
    }

    if (correo=="admin@correo.com" && contra=="admin123"){
        // Iniciar sesión como administrador
        ui->statusbar->showMessage("Iniciando sesión como administrador...");
        //Se abre la ventana de administrador
        admin *adminWindow = new admin();
        adminWindow->show();
        this->close(); // Cierra la ventana de inicio de sesión
        return;
    }

     //Clientes de prueba
    if (correo=="cliente1@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        // Aquí puedes abrir la ventana de cliente o realizar otras acciones
        return;
    }
    else if (correo=="cliente2@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        // Aquí puedes abrir la ventana de cliente o realizar otras acciones
        return;
    }
    else if (correo=="cliente3@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        // Aquí puedes abrir la ventana de cliente o realizar otras acciones
        return;
    }
    else{
        ui->statusbar->showMessage("Correo o contraseña incorrectos. Por favor, inténtelo de nuevo.");
        return;
    }
  
}

