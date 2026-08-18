#include "admin.h"
#include "ui_admin.h"
#include "crearpeli.h"
#include "agregarbeneficio.h"
#include "agregarpromocion.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "../servicios/guardarDatosService.h"

admin::admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , crearPeli(nullptr)
    , guardar()
    , ventanaPromocion(nullptr)
    , ventanaBeneficio(nullptr)
{
    ui->setupUi(this);
    // Crear la instancia única de CrearPeli pasando la referencia a guardarDatosService
    crearPeli = new CrearPeli(&guardar);
    ventanaPromocion = new agregarPromocion(&guardar);
    ventanaBeneficio = new agregarBeneficio(&guardar);
    connect(crearPeli, &CrearPeli::peliculaGuardada, this, &admin::actualizarTabla);
    connect(ventanaPromocion, &agregarPromocion::promocionGuardada, this, &admin::actualizarTreePromociones);
    connect(ventanaBeneficio, &agregarBeneficio::beneficioGuardado, this, &admin::actualizarTreePromociones);
    actualizarTabla();
    actualizarTreePromociones();
}

admin::~admin()
{
    if (crearPeli != nullptr) {
        delete crearPeli;
    }

    if (ventanaPromocion !=nullptr){
        delete ventanaPromocion;
    }

    if (ventanaBeneficio !=nullptr){
        delete ventanaBeneficio;
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

void admin::on_botonAgregarBeneficio_clicked()
{
    bool ok;
    QString codigo = QInputDialog::getText(
        this,
        "Ingresar codigo",
        "Ingresa el codigo de la promocion a la que quieres agregar un beneficio:",
        QLineEdit::Normal,
        "",
        &ok
        );
    std::string codigoPromo = codigo.toStdString();
    if (ok && guardar.guardarBeneficioAux(codigoPromo)){
        QMessageBox::information(this,"Agregar Beneficio", "Ingrese los datos del beneficio en la siguiente ventana");
        if (ventanaBeneficio == nullptr) {
            ventanaBeneficio = new agregarBeneficio(&guardar);
            connect(ventanaBeneficio, &agregarBeneficio::beneficioGuardado, this, &admin::actualizarTreePromociones);
        }
        ventanaBeneficio->setCodigoPromocion(codigoPromo);
        ventanaBeneficio->show();
        ventanaBeneficio->raise();
        ventanaBeneficio->activateWindow();
        return;
    } else{
        QMessageBox::information(this, "Error","Codigo no valido");
    }
}


void admin::on_botonAgregarPromocion_clicked()
{
    if (ventanaPromocion != nullptr) {
        ventanaPromocion->show();
    }
}

void admin::actualizarTreePromociones()
{
    ui->treeWidgetPromociones->clear();

    guardar.listaPromociones.recorrer([this](const Promocion* promo, const ListaDoble& beneficios) {
        QString tituloPromo = QString::fromStdString(promo->codigo + " - " + promo->nombre);
        auto* itemPromo = new QTreeWidgetItem(ui->treeWidgetPromociones);
        itemPromo->setText(0, tituloPromo);
        itemPromo->setText(1, QString::fromStdString(promo->fechaInicio + " | " + promo->fechaFin));
        itemPromo->setText(2, QString::fromStdString(promo->diasAplicables));

        bool tieneBeneficios = false;
        beneficios.recorrer([&](const Beneficio* beneficio) {
            auto* itemBeneficio = new QTreeWidgetItem(itemPromo);
            itemBeneficio->setText(0, QString::fromStdString("Beneficio: " + beneficio->tipoBeneficio));
            itemBeneficio->setText(1, QString::fromStdString(beneficio->descripcion));
            itemBeneficio->setText(2, QString::fromStdString(beneficio->valor));
            tieneBeneficios = true;
        });

        if (!tieneBeneficios) {
            auto* itemSinBeneficios = new QTreeWidgetItem(itemPromo);
            itemSinBeneficios->setText(0, "Sin beneficios");
        }
    });

    ui->treeWidgetPromociones->expandAll();
}

