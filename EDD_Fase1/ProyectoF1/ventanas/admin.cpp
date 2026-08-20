#include "admin.h"
#include "ui_admin.h"
#include "crearpeli.h"
#include "agregarbeneficio.h"
#include "agregarpromocion.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSizePolicy>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QSignalBlocker>
#include "../servicios/guardarDatosService.h"
#include "../servicios/rutasReportes.h"


admin::admin(guardarDatosService& servicio, QWidget *parent) // Constructor de la clase admin
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , crearPeli(nullptr)
    , guardar(servicio)
    , ventanaPromocion(nullptr)
    , ventanaBeneficio(nullptr)
    , watcherReportes(nullptr)
    , comboReportes(nullptr)
    , visorReporte(nullptr)
    , scrollReporte(nullptr)
{ // Inicializa la interfaz de usuario y las instancias de las ventanas secundarias
    ui->setupUi(this);

    QPushButton* botonSalir = findChild<QPushButton*>("botonSalir");
    if (botonSalir == nullptr) {
        botonSalir = new QPushButton("Salir", this);
        botonSalir->setObjectName("botonSalir");
        botonSalir->setGeometry(940, 8, 120, 30);
    }
    connect(botonSalir, &QPushButton::clicked, this, &admin::on_botonSalir_clicked);

    // Crear la instancia única de CrearPeli pasando la referencia a guardarDatosService
    crearPeli = new CrearPeli(&guardar);
    ventanaPromocion = new agregarPromocion(&guardar);
    ventanaBeneficio = new agregarBeneficio(&guardar);
    connect(crearPeli, &CrearPeli::peliculaGuardada, this, &admin::actualizarTabla);
    connect(ventanaPromocion, &agregarPromocion::promocionGuardada, this, &admin::actualizarTreePromociones);
    connect(ventanaBeneficio, &agregarBeneficio::beneficioGuardado, this, &admin::actualizarTreePromociones);
    actualizarTabla();
    actualizarTreePromociones();
    actualizarTablaFuncion();
    actualizarTablaSolicitudes();
    inicializarTabReportes();
}

admin::~admin() // Destructor de la clase admin
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

    if (watcherReportes != nullptr) {
        delete watcherReportes;
    }

    delete ui;

}

void admin::inicializarTabReportes()  // Inicializa la interfaz de usuario para la pestaña de reportes
{
    directorioReportes = QString::fromStdString(rutasReportes::directorio());

    definicionesReportes = {
        {"Arbol de peliculas", "arbol_binario_peliculas.png"},
        {"Promociones y beneficios", "lista_unificada.png"},
        {"Solicitudes (lista circular doble)", "lista_circular_doble.png"},
        {"Matriz de asientos", "MatrizFuncion.png"}
    };

    auto* layoutPrincipal = new QVBoxLayout(ui->tab_4);
    layoutPrincipal->setContentsMargins(12, 12, 12, 12);
    layoutPrincipal->setSpacing(8);

    auto* etiquetaSelector = new QLabel("Selecciona el reporte:", ui->tab_4);
    layoutPrincipal->addWidget(etiquetaSelector);

    comboReportes = new QComboBox(ui->tab_4);
    for (const auto& definicion : definicionesReportes) {
        comboReportes->addItem(definicion.first, definicion.second);
    }
    layoutPrincipal->addWidget(comboReportes);

    scrollReporte = new QScrollArea(ui->tab_4);
    scrollReporte->setWidgetResizable(true);
    scrollReporte->setAlignment(Qt::AlignCenter);

    visorReporte = new QLabel(scrollReporte);
    visorReporte->setAlignment(Qt::AlignCenter);
    visorReporte->setMinimumSize(320, 240);
    visorReporte->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    visorReporte->setText("Genera un reporte para visualizarlo aqui");
    scrollReporte->setWidget(visorReporte);

    layoutPrincipal->addWidget(scrollReporte, 1);

    watcherReportes = new QFileSystemWatcher(this);
    connect(watcherReportes, &QFileSystemWatcher::fileChanged,
            this, &admin::onArchivoReporteCambiado);
    connect(watcherReportes, &QFileSystemWatcher::directoryChanged,
            this, &admin::onDirectorioReportesCambiado);
    connect(comboReportes, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &admin::onReporteSeleccionado);

    actualizarVigilanciaReportes();
    refrescarReporteActual();
}

void admin::actualizarVigilanciaReportes()  // Actualiza la vigilancia de archivos y directorios para los reportes
{
    if (watcherReportes == nullptr) {
        return;
    }

    const QStringList rutasActuales = watcherReportes->files();
    if (!rutasActuales.isEmpty()) {
        watcherReportes->removePaths(rutasActuales);
    }

    const QStringList directoriosActuales = watcherReportes->directories();
    if (!directoriosActuales.isEmpty()) {
        watcherReportes->removePaths(directoriosActuales);
    }

    QDir directorio(directorioReportes);
    if (directorio.exists()) {
        watcherReportes->addPath(directorioReportes);
    }

    for (const auto& definicion : definicionesReportes) {
        const QString rutaCompleta = directorio.absoluteFilePath(definicion.second);
        if (QFileInfo::exists(rutaCompleta)) {
            watcherReportes->addPath(rutaCompleta);
        }
    }
}

void admin::onReporteSeleccionado(int)
{
    refrescarReporteActual();
}

void admin::onArchivoReporteCambiado(const QString&)
{
    actualizarVigilanciaReportes();
    refrescarReporteActual();
}

void admin::onDirectorioReportesCambiado(const QString&)
{
    actualizarVigilanciaReportes();
    refrescarReporteActual();
}

void admin::refrescarReporteActual() // Refresca la visualización del reporte actualmente seleccionado en la interfaz de usuario
{
    if (comboReportes == nullptr || visorReporte == nullptr) {
        return;
    }

    const QString archivoSeleccionado = comboReportes->currentData().toString();
    QDir directorio(directorioReportes);
    const QString rutaCompleta = directorio.absoluteFilePath(archivoSeleccionado);

    QSignalBlocker bloqueador(comboReportes);
    const int indiceActual = comboReportes->currentIndex();
    comboReportes->clear();
    for (const auto& definicion : definicionesReportes) {
        const QString rutaDef = directorio.absoluteFilePath(definicion.second);
        const bool existe = QFileInfo::exists(rutaDef);
        const QString texto = existe
            ? definicion.first
            : definicion.first + " (pendiente)";
        comboReportes->addItem(texto, definicion.second);
    }
    if (indiceActual >= 0 && indiceActual < comboReportes->count()) {
        comboReportes->setCurrentIndex(indiceActual);
    }

    reporteOriginal = QPixmap();
    if (!QFileInfo::exists(rutaCompleta) || !reporteOriginal.load(rutaCompleta)) {
        visorReporte->setPixmap(QPixmap());
        visorReporte->setText("No existe la imagen del reporte seleccionado.\nGenera el reporte para visualizarlo.");
        return;
    }

    visorReporte->setText(QString());
    ajustarEscalaReporte();
}

void admin::ajustarEscalaReporte() //Se ajusta la escala del reporte visualizado para que se adapte al tamaño del área visible en la interfaz de usuario
{
    if (visorReporte == nullptr || scrollReporte == nullptr || reporteOriginal.isNull()) {
        return;
    }

    QSize areaVisible = scrollReporte->viewport()->size();
    if (areaVisible.width() < 16 || areaVisible.height() < 16) {
        areaVisible = QSize(800, 520);
    }

    const QPixmap escalado = reporteOriginal.scaled(
        areaVisible,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    visorReporte->setPixmap(escalado);
    visorReporte->resize(escalado.size());
}

void admin::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    ajustarEscalaReporte();
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


void admin::actualizarTabla() { // Actualiza la tabla de películas en la interfaz de usuario
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


void admin::on_botonAgregarPromocion_2_clicked()
{

}


void admin::on_botonCrearFuncion_clicked()  //Boton que crea la funcion, toma los datos de los textos en el panel de funciones y los envia a la funcion crearFuncion de guardarDatosService
{
    QString codigoPelicula = ui->textoCodigoPelicula->text();
    QString filas = ui->textoFilas->text();
    QString columnas = ui->textoColumnas->text();
    QString sala = ui->comboBoxSala->currentText();
    QString fechaHora = ui->tiempoHorarioFuncion->text();
  

    if (filas.isEmpty() || columnas.isEmpty() || sala.isEmpty() || fechaHora.isEmpty()) {
        QMessageBox::information(this, "Error", "Es necesario llenar todos los campos.");
        return;
    }

    bool ok;
    int numFilas = filas.toInt(&ok);
    if (!ok || numFilas <= 0) {
        QMessageBox::information(this, "Error", "El número de filas debe ser un entero positivo.");
        return;
    }
    int numColumnas = columnas.toInt(&ok);
    if (!ok || numColumnas <= 0) {
        QMessageBox::information(this, "Error", "El número de columnas debe ser un entero positivo.");
        return;
    }

    if (!guardar.arbol.CodigoExiste(codigoPelicula.toStdString())) {
        QMessageBox::information(this, "Error", "El código de película no existe.");
        return;
    }

    if (guardar.crearFuncion(codigoPelicula.toStdString(), numFilas, numColumnas, fechaHora.toStdString(), sala.toStdString())) {
        QMessageBox::information(this, "Éxito", "La función se ha creado correctamente.");
        actualizarTablaFuncion();
    } else {
        QMessageBox::information(this, "Error", "No se pudo crear la función. Verifica los datos ingresados.");
    }

}


void admin::on_botonReservarAsiento_clicked()
{
    QString filaReserva = ui->textoFilaReserva->text();
    QString columnaReserva = ui->textoColumnaReserva->text();
    QString nombreReserva = ui->textoNombreCliente->text();

    if (filaReserva.isEmpty() || columnaReserva.isEmpty() || nombreReserva.isEmpty()) {
        QMessageBox::information(this, "Error", "Es necesario llenar todos los campos.");
        return;
    }

    bool ok;

    // La matriz interna maneja filas como letras (A, B, C...).
    std::string filaNormalizada;
    QString filaTexto = filaReserva.trimmed().toUpper();
    if (filaTexto.size() == 1 && filaTexto[0].isLetter()) {
        filaNormalizada = filaTexto.toStdString();
    } else {
        int fila = filaReserva.toInt(&ok);
        if (!ok || fila <= 0 || fila > 26) {
            QMessageBox::information(this, "Error", "La fila debe ser una letra (A-Z) o un número entre 1 y 26.");
            return;
        }
        char letraFila = static_cast<char>('A' + (fila - 1));
        filaNormalizada = std::string(1, letraFila);
    }

    int columna = columnaReserva.toInt(&ok);
    if (!ok || columna <= 0) {
        QMessageBox::information(this, "Error", "El número de columna debe ser un entero positivo.");
        return;
    }

    if (guardar.reservarAsiento(nombreReserva.toStdString(), filaNormalizada, std::to_string(columna))) {
        QMessageBox::information(this, "Éxito", "El asiento se ha reservado correctamente.");
        actualizarTablaFuncion();
    } else {
        QMessageBox::information(this, "Error", "El asiento esta ocupado o los datos son incorrectos");
    }
}

void admin::actualizarTablaFuncion()
{
    int filas = guardar.matrizFunciones.obtenerTotalFilas();
    int columnas = guardar.matrizFunciones.obtenerTotalColumnas();

    ui->tablaFuncion->clear();
    ui->tablaFuncion->setRowCount(filas);
    ui->tablaFuncion->setColumnCount(columnas);
    ui->tablaFuncion->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headersColumnas;
    for (int c = 1; c <= columnas; ++c) {
        headersColumnas << QString::number(c);
    }
    ui->tablaFuncion->setHorizontalHeaderLabels(headersColumnas);

    for (int r = 0; r < filas; ++r) {
        char letraFila = static_cast<char>('A' + r);
        QString filaEtiqueta(letraFila);
        ui->tablaFuncion->setVerticalHeaderItem(r, new QTableWidgetItem(filaEtiqueta));

        std::string filaStr(1, letraFila);
        for (int c = 1; c <= columnas; ++c) {
            std::string valor = guardar.matrizFunciones.obtenerValorAsiento(filaStr, std::to_string(c));
            if (valor.empty()) {
                valor = "--";
            }
            ui->tablaFuncion->setItem(r, c - 1, new QTableWidgetItem(QString::fromStdString(valor)));
        }
    }
}


void admin::on_botonCrearSolicitud_clicked()
{
    QString nombreCliente = ui->textoClienteSolicitud->text();
    QString telefonoCliente = ui->textoTelefonoSolicitud->text();
    QString tipoSolicitud = ui->comboBoxTipoSolicitud->currentText();
    QString descripcionSolicitud = ui->textoDescripcionSolicitud->text();

    if (nombreCliente.isEmpty() || telefonoCliente.isEmpty() || descripcionSolicitud.isEmpty()){
        QMessageBox::information(this, "Error", "Es necesario llenar todos los espacios");
        return;
    }

    guardar.guardarSolicitud(nombreCliente.toStdString(),telefonoCliente.toStdString(), tipoSolicitud.toStdString(), descripcionSolicitud.toStdString());
    actualizarTablaSolicitudes();
    QMessageBox::information(this,"Exito", "Se ha creado la solicitud con exito");
}

void admin::actualizarTablaSolicitudes()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Solicitud", "Cliente", "Estado", "Acciones"});
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->setColumnWidth(2, 85);

    guardar.listaSolicitudes.recorrer([this](Solicitud* solicitud) {
        int fila = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setRowHeight(fila, 58);

        auto* itemNumero = new QTableWidgetItem(QString::number(solicitud->numeroSolicitud));
        auto* itemCliente = new QTableWidgetItem(QString::fromStdString(solicitud->nombreCliente));
        auto* itemEstado = new QTableWidgetItem(QString::fromStdString(textoDesdeEstado(solicitud->estadoSolicitud)));
        itemCliente->setToolTip(QString::fromStdString(solicitud->descripcion));
        itemEstado->setToolTip(QString::fromStdString(
            "Tipo: " + solicitud->tipoSolicitud + "\nDescripcion: " + solicitud->descripcion));

        if (solicitud->estadoSolicitud == EstadoSolicitud::Aprobado) {
            itemEstado->setBackground(QColor("#2ecc71"));
        } else if (solicitud->estadoSolicitud == EstadoSolicitud::Rechazado) {
            itemEstado->setBackground(QColor("#e74c3c"));
        } else {
            itemEstado->setBackground(QColor("#f1c40f"));
        }

        ui->tableWidget->setItem(fila, 0, itemNumero);
        ui->tableWidget->setItem(fila, 1, itemCliente);
        ui->tableWidget->setItem(fila, 2, itemEstado);

        auto* acciones = new QWidget(ui->tableWidget);
        auto* layoutAcciones = new QHBoxLayout(acciones);
        auto* botonAceptar = new QPushButton("Aceptar", acciones);
        auto* botonRechazar = new QPushButton("Rechazar", acciones);
        auto* botonDetalles = new QPushButton("Detalles", acciones);
        layoutAcciones->setContentsMargins(2, 2, 2, 2);
        layoutAcciones->addWidget(botonAceptar);
        layoutAcciones->addWidget(botonRechazar);
        layoutAcciones->addWidget(botonDetalles);
        ui->tableWidget->setCellWidget(fila, 3, acciones);

        bool pendiente = solicitud->estadoSolicitud == EstadoSolicitud::EnEspera;
        botonAceptar->setEnabled(pendiente);
        botonRechazar->setEnabled(pendiente);

        int numeroSolicitud = solicitud->numeroSolicitud;
        QString nombreCliente = QString::fromStdString(solicitud->nombreCliente);
        QString telefonoContacto = QString::fromStdString(solicitud->telefonoContacto);
        QString tipoSolicitud = QString::fromStdString(solicitud->tipoSolicitud);
        QString descripcion = QString::fromStdString(solicitud->descripcion);
        QString fechaSolicitud = QString::fromStdString(solicitud->fechaSolicitud);
        QString estadoSolicitud = QString::fromStdString(textoDesdeEstado(solicitud->estadoSolicitud));

        connect(botonDetalles, &QPushButton::clicked, this, [this, numeroSolicitud,
                                                              nombreCliente, telefonoContacto,
                                                              tipoSolicitud, descripcion,
                                                              fechaSolicitud, estadoSolicitud]() {
            QMessageBox detalles(this);
            detalles.setWindowTitle("Detalles de la solicitud");
            detalles.setIcon(QMessageBox::Information);
            detalles.setText(QString("Solicitud #%1\n\n"
                                      "Cliente: %2\n"
                                      "Contacto: %3\n"
                                      "Tipo: %4\n"
                                      "Descripcion: %5\n"
                                      "Fecha: %6\n"
                                      "Estado: %7")
                                  .arg(numeroSolicitud)
                                  .arg(nombreCliente)
                                  .arg(telefonoContacto)
                                  .arg(tipoSolicitud)
                                  .arg(descripcion)
                                  .arg(fechaSolicitud)
                                  .arg(estadoSolicitud));
            detalles.exec();
        });

        connect(botonAceptar, &QPushButton::clicked, this, [this, numeroSolicitud]() {
            if (guardar.listaSolicitudes.cambiarEstado(numeroSolicitud, EstadoSolicitud::Aprobado)) {
                actualizarTablaSolicitudes();
            }
        });
        connect(botonRechazar, &QPushButton::clicked, this, [this, numeroSolicitud]() {
            if (guardar.listaSolicitudes.cambiarEstado(numeroSolicitud, EstadoSolicitud::Rechazado)) {
                actualizarTablaSolicitudes();
            }
        });
    });
}


void admin::on_botonSalir_clicked()
{
    if (crearPeli != nullptr) {
        crearPeli->close();
    }
    if (ventanaPromocion != nullptr) {
        ventanaPromocion->close();
    }
    if (ventanaBeneficio != nullptr) {
        ventanaBeneficio->close();
    }

    QWidget* loginWindow = parentWidget();
    if (loginWindow != nullptr) {
        loginWindow->show();
        loginWindow->raise();
        loginWindow->activateWindow();
    }

    this->close();
}

