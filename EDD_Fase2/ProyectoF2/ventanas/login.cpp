#include "login.h"
#include "ui_login.h"
#include "admin.h"
#include "cliente.h"
#include "../ui/registrocliente.h"
#include "../servicios/autentificacionService.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , guardarCompartido()
    , adminWindow(nullptr)
    , clienteWindow(nullptr)
    , registroWindow(nullptr)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window
                   | Qt::WindowTitleHint
                   | Qt::WindowSystemMenuHint
                   | Qt::WindowMinimizeButtonHint
                   | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_QuitOnClose, true);
    setWindowModality(Qt::NonModal);
}

MainWindow::~MainWindow()
{
    delete registroWindow;
    delete ui;
}

void MainWindow::on_botonRegistrarse_clicked()
{
    if (registroWindow == nullptr) {
        registroWindow = new registroCliente(guardarCompartido, this);
    }
    registroWindow->show();
    registroWindow->raise();
    registroWindow->activateWindow();
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
        adminWindow = new admin(guardarCompartido, nullptr);
        adminWindow->setLoginWindow(this);
        adminWindow->setAttribute(Qt::WA_DeleteOnClose);
        adminWindow->show();
        this->hide(); // Se oculta para mantenerla viva como parent y poder volver con "Salir"
        return;
    }

    AutentificacionService autentificacion(guardarCompartido);
    Cliente* clienteAutenticado = autentificacion.autenticar(correo.toStdString(), contra.toStdString());
    if (clienteAutenticado != nullptr) {
        clienteWindow = new cliente(guardarCompartido, nullptr);
        clienteWindow->setLoginWindow(this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setCliente(clienteAutenticado->id, clienteAutenticado->nombre);
        clienteWindow->show();
        this->hide();
        return;
    }

     //Clientes de prueba
    if (correo=="cliente1@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, nullptr);
        clienteWindow->setLoginWindow(this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setCliente("U001", "Mynor");
        clienteWindow->show();
        this->hide();
        return;
    }
    else if (correo=="cliente2@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, nullptr);
        clienteWindow->setLoginWindow(this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setCliente("U002", "Juan");
        clienteWindow->show();
        this->hide();
        return;
    }
    else if (correo=="cliente3@correo.com" && contra=="cliente123"){
        ui->statusbar->showMessage("Iniciando sesión como cliente...");
        clienteWindow = new cliente(guardarCompartido, nullptr);
        clienteWindow->setLoginWindow(this);
        clienteWindow->setAttribute(Qt::WA_DeleteOnClose);
        clienteWindow->setCliente("U003", "Pedro");
        clienteWindow->show();
        this->hide();
        return;
    }
    else{
        ui->statusbar->showMessage("Correo o contraseña incorrectos. Por favor, inténtelo de nuevo.");
        return;
    }
  
}

