#include "admin.h"
#include "ui_admin.h"
#include "crearpeli.h"
#include <QInputDialog>
#include <QMessageBox>
#include "../servicios/guardarDatosService.h"

admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , crearPeli(nullptr)
    , guardar()
{
    ui->setupUi(this);
    // Crear la instancia única de CrearPeli pasando la referencia a guardarDatosService
    crearPeli = new CrearPeli(&guardar);
    connect(crearPeli, &CrearPeli::peliculaGuardada, this, &admin::actualizarTabla);
    actualizarTabla();
}

admin::~admin()
{
    if (crearPeli != nullptr) {
        delete crearPeli;
    }
    delete ui;
}

void admin::on_botonAgregar_clicked()
{
    // Se muestra la ventana de CrearPeli (única instancia persistente)
    if (crearPeli != nullptr) {
        crearPeli->show();
    }
}


void admin::on_botonEliminar_clicked()
{
    bool ok;
    QString codigo = QInputDialog::getText(
        this,
        "Intruducir codigo",
        "Ingresa el codigo de la pelicula que quieres eliminar:",
        QLineEdit::Normal,
        "",
        &ok
        );
    if (ok && guardar.eliminarPelicula(codigo.toStdString())){
        QMessageBox::information(this, "Accion Completada","Se elimino la pelicula correctamente");
        actualizarTabla();
        return;
    } else{
        QMessageBox::information(this, "Error","Codigo no encontrado");
        return;
    }
}


void admin::on_botonCargarCSV_clicked()
{
    bool ok;
    QString ruta = QInputDialog::getText(
        this,
        "Intruducir ruta",
        "Ingresa la ruta del archivo CSV:",
        QLineEdit::Normal,
        "",
        &ok
        );
    if (ok && guardar.cargarCSV(ruta.toStdString())){
        QMessageBox::information(this, "Accion Completada","Se ha cargado el archivo CSV con exito");
        actualizarTabla();
        return;
    } else{
        QMessageBox::information(this, "Error","Archivo no encontrado");
    }
}


void admin::actualizarTabla() {
    ui->tablaPeliculas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaPeliculas->setRowCount(0); // limpia lo que hubiera antes de repoblar

    guardar.arbol.inOrden([this](Pelicula* p) {
        int fila = ui->tablaPeliculas->rowCount();
        ui->tablaPeliculas->insertRow(fila);

        ui->tablaPeliculas->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p->id)));
        ui->tablaPeliculas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->titulo)));
        ui->tablaPeliculas->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->genero)));
        ui->tablaPeliculas->setItem(fila, 3, new QTableWidgetItem(QString::number(p->duracion)));
        ui->tablaPeliculas->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(p->clasificacion)));
        ui->tablaPeliculas->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(p->idioma)));
        ui->tablaPeliculas->setItem(fila, 6, new QTableWidgetItem(QString::fromStdString(p->fechaEstreno)));
        ui->tablaPeliculas->setItem(fila, 7, new QTableWidgetItem(QString::fromStdString(p->fechaFinCartelera)));
    });
}
