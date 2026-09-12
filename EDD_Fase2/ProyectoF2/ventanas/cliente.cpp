#include "cliente.h"
#include "ui_cliente.h"
#include "../servicios/guardarDatosService.h"
#include <QPushButton>

cliente::cliente(guardarDatosService& servicio, QWidget *parent)
    : QMainWindow(parent)
    , guardar(servicio)
    , ui(new Ui::cliente)

{
    ui->setupUi(this);

    QPushButton* botonSalir = findChild<QPushButton*>("botonSalir");
    if (botonSalir == nullptr) {
        botonSalir = new QPushButton("Salir", ui->centralwidget);
        botonSalir->setObjectName("botonSalir");
        botonSalir->setGeometry(790, 0, 110, 28);
    }
    connect(botonSalir, &QPushButton::clicked, this, &cliente::on_botonSalir_clicked);

    actualizarTabla();
}

cliente::~cliente()
{
    delete ui;
}

void cliente::setNombreCliente(const std::string& nombre) {
    nombreCliente = nombre;
    ui->labelNombreCliente->setText(QString::fromStdString(nombreCliente));
    ui->labelNombreCliente2->setText(QString::fromStdString(nombreCliente));
}

void cliente::actualizarTabla() {
    ui->tablaPeliculas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaPeliculas->setRowCount(0); // limpia lo que hubiera antes de repoblar

    guardar.arbol.inOrden([this](Pelicula* p) {
        int fila = ui->tablaPeliculas->rowCount();
        ui->tablaPeliculas->insertRow(fila);

        ui->tablaPeliculas->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p->id)));
        ui->tablaPeliculas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->titulo)));
        ui->tablaPeliculas->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->fechaEstreno)));
        ui->tablaPeliculas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p->fechaFinCartelera)));
    });
}

void cliente::on_botonBuscarPeli_5_clicked()
{

}

void cliente::on_botonSalir_clicked()
{
    QWidget* loginWindow = parentWidget();
    if (loginWindow != nullptr) {
        loginWindow->show();
        loginWindow->raise();
        loginWindow->activateWindow();
    }

    this->close();

}

