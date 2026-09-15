#include "registrocliente.h"
#include "ui_registrocliente.h"
#include "../servicios/autentificacionService.h"
#include <QMessageBox>

registroCliente::registroCliente(guardarDatosService& servicio, QWidget *parent)
    : QMainWindow(parent)
    , guardar(servicio)
    , ui(new Ui::registroCliente)
{
    ui->setupUi(this);
}

void registroCliente::on_botonRegistrarse_clicked()
{
    const QString nombre = ui->textoNombre->text().trimmed();
    const QString correo = ui->textoCorreo->text().trimmed();
    const QString telefono = ui->textoTelefono->text().trimmed();
    const QString password = ui->textoContrasenia->text();
    if (nombre.isEmpty() || correo.isEmpty() || telefono.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Registro", "Completa todos los campos.");
        return;
    }
    AutentificacionService servicio(guardar);
    std::string id;
    if (!servicio.registrar(nombre.toStdString(), correo.toStdString(), telefono.toStdString(), password.toStdString(), id)) {
        QMessageBox::warning(this, "Registro", "El correo ya existe o los datos no son validos.");
        return;
    }
    QMessageBox::information(this, "Registro", QString("Cliente creado con ID %1.").arg(QString::fromStdString(id)));
    emit registroCompletado();
    close();
}

void registroCliente::on_botonInicio_clicked()
{
    close();
}

registroCliente::~registroCliente()
{
    delete ui;
}
