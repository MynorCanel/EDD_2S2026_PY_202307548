#include "agregarbeneficio.h"
#include "ui/ui_agregarbeneficio.h"
#include <QInputDialog>
#include <QMessageBox>
#include "admin.h"
#include "../servicios/guardarDatosService.h"

agregarBeneficio::agregarBeneficio(guardarDatosService* servicio, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::agregarBeneficio)
    , guardar(servicio)
    , codigoPromocion("")
{
    ui->setupUi(this);
}

void agregarBeneficio::setCodigoPromocion(const std::string& codigo)
{
    codigoPromocion = codigo;
}

agregarBeneficio::~agregarBeneficio()
{
    delete ui;
}

void agregarBeneficio::on_botonRegresar_clicked()
{
    this -> close();
}


void agregarBeneficio::on_botonAgregarBeneficio_clicked()
{
    if (guardar == nullptr) {
        QMessageBox::critical(this, "Error", "No se pudo inicializar el servicio de datos.");
        return;
    }

    if (codigoPromocion.empty()) {
        QMessageBox::warning(this, "Error", "Debes seleccionar una promoción antes de agregar un beneficio.");
        return;
    }

    QString tipoBeneficio = ui->comboBoxTipoBeneficio->currentText();
    QString descripcion = ui->textoDescripcionBeneficio->text();
    QString valor = ui->comboBoxValorBeneficio->currentText();

    if (tipoBeneficio.isEmpty() || descripcion.isEmpty() || valor.isEmpty()){
        QMessageBox::information(this, "Error","Es necesario llenar todos los campos");
        return;
    }

    if (guardar->guardarBeneficioAPromocion(codigoPromocion, tipoBeneficio.toStdString(), descripcion.toStdString(), valor.toStdString())) {
        QMessageBox::information(this, "Éxito", "El beneficio se ha agregado correctamente a la promoción.");
        emit beneficioGuardado();
        close();
    } else {
        QMessageBox::warning(this, "Error", "No se pudo agregar el beneficio a la promoción seleccionada.");
    }


}

