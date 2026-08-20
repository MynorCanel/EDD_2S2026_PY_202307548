#include "login.h"
#include "ui_login.h"
#include "admin.h"
#include "cliente.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , guardarCompartido()
    , adminWindow(nullptr)
    , clienteWindow(nullptr)
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
        // Se abre la ventana de administrador usando la misma instancia compartida.
        adminWindow = new admin(guardarCompartido, this);
        adminWindow->setAttribute(Qt::WA_DeleteOnClose);
        adminWindow->show();
        this->hide(); // Se oculta para mantenerla viva como parent y poder volver con "Salir"
        return;
    }

     //Clientes de prueba
    if (correo=="cliente1@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setNombreCliente("Mynor");
        clienteWindow->show();
        this->hide();
        return;
    }
    else if (correo=="cliente2@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setNombreCliente("Juan");
        clienteWindow->show();
        this->hide();
        return;
    }
    else if (correo=="cliente3@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setNombreCliente("Pedro");
        clienteWindow->show();
        this->hide();
        return;
    }
    else{
        ui->statusbar->showMessage("Correo o contraseña incorrectos. Por favor, inténtelo de nuevo.");
        return;
    }
  
}

