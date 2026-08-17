#include "crearpeli.h"
#include "ui/ui_crearpeli.h"
#include "../servicios/guardarDatosService.h"
#include <QMessageBox>

CrearPeli::CrearPeli(guardarDatosService* servicio, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CrearPeli)
    , guardar(servicio)
{
    ui->setupUi(this);
}

CrearPeli::~CrearPeli()
{
    delete ui;
}

void CrearPeli::on_botonAgregarPeli_clicked()
{
    QString codigo = ui->textoCodigo->text();
    QString titulo = ui->textoTitulo->text();
    QString genero = ui->textoGenero->text();
    QString clasificacion = ui->comboBoxClasificacion->currentText();
    QString duracionText = ui->textoDuracion->text();
    QString idioma = ui->comboBoxIdioma->currentText();
    QString fechaEstreno = ui->fechaEstreno->date().toString("yyyy-MM-dd");
    QString fechaFinCartelera = ui->fechaFin->date().toString("yyyy-MM-dd");
    
    // Validar campos obligatorios
    if (codigo.isEmpty() || titulo.isEmpty() || genero.isEmpty() || clasificacion.isEmpty() || idioma.isEmpty() || duracionText.isEmpty()) {
        QMessageBox::information(this,"Error","Es necesario llenar todos los campos.");
        return;
    }

    // Validar que la duración sea un número entero válido
    bool ok;
    int duracion = duracionText.toInt(&ok);
    
    if (!ok) {
        QMessageBox::information(this,"Error","La duración debe ser un número entero válido.");
        return;
    }

    if (duracion <= 0) {
        QMessageBox::information(this,"Error","La duración debe ser un número entero positivo.");
        return;
    }
    
    if (guardar->guardarPelicula(codigo.toStdString(), titulo.toStdString(), genero.toStdString(), duracion, clasificacion.toStdString(), idioma.toStdString(), fechaEstreno.toStdString(), fechaFinCartelera.toStdString())) {
        // Película guardada correctamente
        QMessageBox::information(this, "Éxito", "La película se ha guardado correctamente.");
        emit peliculaGuardada();
        // Limpiar campos
        ui->textoCodigo->clear();
        ui->textoTitulo->clear();
        ui->textoGenero->clear();
        ui->textoDuracion->clear();
    } else {
        // Error al guardar la película
        QMessageBox::warning(this, "Error", "No se pudo guardar la película. Verifica que el código sea único.");
    }
}


void CrearPeli::on_botonRegresar_clicked()
{
    this->close();
}

