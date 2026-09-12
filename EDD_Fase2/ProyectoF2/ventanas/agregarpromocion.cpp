#include "agregarpromocion.h"
#include "ui/ui_agregarpromocion.h"
#include "../servicios/guardarDatosService.h"
#include <QMessageBox>


agregarPromocion::agregarPromocion(guardarDatosService* servicio, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::agregarPromocion)
    , guardar(servicio)
{
    ui->setupUi(this);
}

agregarPromocion::~agregarPromocion()
{
    delete ui;
}

void agregarPromocion::on_botonAgregarPromo_clicked()
{
    QString codigo = ui->textoCodigoPromo->text();
    QString nombre = ui->textoNombrePromo->text();
    QString fechaInicio = ui->fechaInicioPromo->text();
    QString fechaFin = ui->fechaFinPromo->text();
    QString diasAplicables = ui->textoDiasAplicables->text();

    if (codigo.isEmpty() || nombre.isEmpty() || fechaInicio.isEmpty() || fechaFin.isEmpty() || diasAplicables.isEmpty()) {
        QMessageBox::information(this, "Error", "Es necesario llenar todos los campos.");
        return;
    }


    
    if (guardar != nullptr && guardar->guardarPromocion(codigo.toStdString(), nombre.toStdString(), fechaInicio.toStdString(), fechaFin.toStdString(), diasAplicables.toStdString())) {
        QMessageBox::information(this, "Exito", "Promocion guardada correctamente.");
        emit promocionGuardada();
        ui->textoCodigoPromo->clear();
        ui->textoNombrePromo->clear();
        ui->textoDiasAplicables->clear();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo guardar la promocion. Verifica codigo duplicado o servicio no inicializado.");
    }
}


void agregarPromocion::on_botonRegresar_clicked()
{
    this -> close();
}

