#include "cliente.h"
#include "ui_cliente.h"
#include "../servicios/guardarDatosService.h"
#include <QPushButton>
#include <QMessageBox>

cliente::cliente(guardarDatosService& servicio, QWidget *parent)
    : QMainWindow(parent)
    , guardar(servicio)
    , ui(new Ui::cliente)
    , loginWindow(nullptr)

{
    ui->setupUi(this);

    QPushButton* botonSalir = findChild<QPushButton*>("botonSalir");
    if (botonSalir == nullptr) {
        botonSalir = new QPushButton("Salir", ui->centralwidget);
        botonSalir->setObjectName("botonSalir");
        botonSalir->setGeometry(790, 0, 110, 28);
    }
    connect(botonSalir, &QPushButton::clicked, this, &cliente::on_botonSalir_clicked);
    connect(ui->tablaFunciones_2, &QTableWidget::itemSelectionChanged, this, &cliente::actualizarTablaFuncionCliente);
    connect(ui->tablaFuncion_2, &QTableWidget::cellDoubleClicked, this, [this](int fila, int columna) {
        if (codigoFuncionSeleccionada.empty() || idCliente.empty()) return;
        MatrizCine* matriz = guardar.arbolFunciones.buscar(codigoFuncionSeleccionada);
        if (matriz == nullptr) return;
        const std::string filaTexto(1, static_cast<char>('A' + fila));
        const std::string columnaTexto = std::to_string(columna + 1);
        const std::string valor = matriz->obtenerValorAsiento(filaTexto, columnaTexto);
        bool resultado = false;
        if (valor == "--" || valor.empty()) {
            resultado = guardar.reservarAsientoCliente(idCliente, codigoFuncionSeleccionada, filaTexto, columnaTexto);
        } else if (valor == nombreCliente) {
            resultado = guardar.cancelarAsientoCliente(idCliente, codigoFuncionSeleccionada, filaTexto, columnaTexto);
        } else {
            QMessageBox::warning(this, "Asiento ocupado", "Este asiento pertenece a otro cliente.");
            return;
        }
        if (!resultado) QMessageBox::warning(this, "Reserva", "No se pudo modificar el asiento.");
        actualizarTablaFuncionCliente();
    });

    actualizarTabla();
    actualizarTablaFuncionesCliente();
}

void cliente::setLoginWindow(QWidget* ventana)
{
    loginWindow = ventana;
}

cliente::~cliente()
{
    delete ui;
}

void cliente::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (ui != nullptr && ui->tabWidget != nullptr) {
        ui->tabWidget->setGeometry(10, 30, width() - 20, height() - 80);
    }
}

void cliente::setNombreCliente(const std::string& nombre) {
    idCliente = nombre;
    nombreCliente = nombre;
    ui->labelNombreCliente->setText(QString::fromStdString(nombreCliente));
    ui->labelNombreCliente2->setText(QString::fromStdString(nombreCliente));
}

void cliente::setCliente(const std::string& id, const std::string& nombre)
{
    idCliente = id;
    nombreCliente = nombre;
    ui->labelNombreCliente->setText(QString::fromStdString(nombreCliente));
    ui->labelNombreCliente2->setText(QString::fromStdString(nombreCliente));
}

void cliente::actualizarTablaFuncionesCliente()
{
    ui->tablaFunciones_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaFunciones_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablaFunciones_2->setRowCount(0);
    guardar.arbolFunciones.inOrden([this](MatrizCine* funcion) {
        const int fila = ui->tablaFunciones_2->rowCount();
        ui->tablaFunciones_2->insertRow(fila);
        ui->tablaFunciones_2->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(funcion->codigoFuncion)));
        ui->tablaFunciones_2->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(funcion->obtenerPelicula())));
        ui->tablaFunciones_2->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(funcion->obtenerHorario())));
        ui->tablaFunciones_2->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(funcion->obtenerSala())));
        ui->tablaFunciones_2->setItem(fila, 4, new QTableWidgetItem(QString::number(funcion->obtenerTotalFilas())));
        ui->tablaFunciones_2->setItem(fila, 5, new QTableWidgetItem(QString::number(funcion->obtenerTotalColumnas())));
    });
    if (ui->tablaFunciones_2->rowCount() > 0) {
        ui->tablaFunciones_2->selectRow(0);
        actualizarTablaFuncionCliente();
    }
}

void cliente::actualizarTablaFuncionCliente()
{
    const int filaSeleccionada = ui->tablaFunciones_2->currentRow();
    if (filaSeleccionada < 0 || ui->tablaFunciones_2->item(filaSeleccionada, 0) == nullptr) return;
    codigoFuncionSeleccionada = ui->tablaFunciones_2->item(filaSeleccionada, 0)->text().toStdString();
    MatrizCine* matriz = guardar.arbolFunciones.buscar(codigoFuncionSeleccionada);
    if (matriz == nullptr) return;
    const int filas = matriz->obtenerTotalFilas();
    const int columnas = matriz->obtenerTotalColumnas();
    ui->tablaFuncion_2->setRowCount(filas);
    ui->tablaFuncion_2->setColumnCount(columnas);
    ui->tablaFuncion_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaFuncion_2->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int r = 0; r < filas; ++r) {
        const char letra = static_cast<char>('A' + r);
        ui->tablaFuncion_2->setVerticalHeaderItem(r, new QTableWidgetItem(QString(letra)));
        for (int c = 1; c <= columnas; ++c) {
            std::string valor = matriz->obtenerValorAsiento(std::string(1, letra), std::to_string(c));
            if (valor.empty()) valor = "--";
            auto* item = new QTableWidgetItem(QString::fromStdString(valor));
            item->setBackground(valor == "--" ? QColor("#c8f7c5") : QColor("#ffd8a8"));
            item->setForeground(QColor("#3f3f46"));
            ui->tablaFuncion_2->setItem(r, c - 1, item);
        }
    }
}

void cliente::on_botonReservarAsiento_clicked()
{
    if (codigoFuncionSeleccionada.empty() || idCliente.empty()) return;
    const QString fila = ui->textoFilaReserva->text().trimmed().toUpper();
    const QString columna = ui->textoColumnaReserva->text().trimmed();
    if (fila.size() != 1 || !fila[0].isLetter() || columna.isEmpty()) return;
    if (!guardar.reservarAsientoCliente(idCliente, codigoFuncionSeleccionada, fila.toStdString(), columna.toStdString())) {
        QMessageBox::warning(this, "Reserva", "El asiento esta ocupado o los datos no son validos.");
        return;
    }
    actualizarTablaFuncionCliente();
}

void cliente::on_botonCancelarReserva_clicked()
{
    const int fila = ui->tablaFuncion_2->currentRow();
    const int columna = ui->tablaFuncion_2->currentColumn();
    if (fila < 0 || columna < 0 || codigoFuncionSeleccionada.empty()) return;
    const std::string filaTexto(1, static_cast<char>('A' + fila));
    if (!guardar.cancelarAsientoCliente(idCliente, codigoFuncionSeleccionada, filaTexto, std::to_string(columna + 1))) {
        QMessageBox::warning(this, "Reserva", "No puedes cancelar una reserva de otro cliente o el asiento esta libre.");
        return;
    }
    actualizarTablaFuncionCliente();
}

void cliente::actualizarTabla() {
    ui->tablaPeliculas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablaPeliculas->setColumnCount(5);
    ui->tablaPeliculas->setHorizontalHeaderItem(4, new QTableWidgetItem("Funciones"));
    ui->tablaPeliculas->setRowCount(0); // limpia lo que hubiera antes de repoblar

    guardar.arbol.inOrden([this](Pelicula* p) {
        int fila = ui->tablaPeliculas->rowCount();
        ui->tablaPeliculas->insertRow(fila);

        ui->tablaPeliculas->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(p->id)));
        ui->tablaPeliculas->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p->titulo)));
        ui->tablaPeliculas->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(p->fechaEstreno)));
        ui->tablaPeliculas->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p->fechaFinCartelera)));
        auto* botonFunciones = new QPushButton("Ver funciones", ui->tablaPeliculas);
        ui->tablaPeliculas->setCellWidget(fila, 4, botonFunciones);
        const std::string titulo = p->titulo;
        connect(botonFunciones, &QPushButton::clicked, this, [this, titulo]() {
            QString texto = "Funciones de " + QString::fromStdString(titulo) + ":\n";
            bool encontrada = false;
            guardar.arbolFunciones.inOrden([&](MatrizCine* funcion) {
                if (funcion->obtenerPelicula() == titulo) {
                    texto += QString::fromStdString(funcion->codigoFuncion + " | " + funcion->obtenerHorario() + " | " + funcion->obtenerSala()) + "\n";
                    encontrada = true;
                }
            });
            if (!encontrada) texto += "No hay funciones disponibles.";
            QMessageBox::information(this, "Funciones disponibles", texto);
        });
    });
}

void cliente::on_botonBuscarPeli_5_clicked()
{

}

void cliente::on_botonSalir_clicked()
{
    if (loginWindow != nullptr) {
        loginWindow->show();
        loginWindow->raise();
        loginWindow->activateWindow();
    }

    this->close();

}

