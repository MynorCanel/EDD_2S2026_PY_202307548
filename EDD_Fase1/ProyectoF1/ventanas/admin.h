#ifndef ADMIN_H
#define ADMIN_H
#include <string>
#include <vector>
#include <utility>
#include <QDialog>
#include <QPixmap>
#include "crearpeli.h"
#include "agregarpromocion.h"
#include "agregarbeneficio.h"
#include "../servicios/guardarDatosService.h"

class QFileSystemWatcher;
class QComboBox;
class QLabel;
class QScrollArea;
class QResizeEvent;


extern std::string codigoPromocion; // Variable global para almacenar el código de promoción

namespace Ui {
class Dialog;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(guardarDatosService& servicio, QWidget *parent = nullptr);
    ~admin();
    


private slots:
    void on_botonAgregar_clicked();

    void on_botonEliminar_clicked();

    void on_botonCargarCSV_clicked();

    void actualizarTabla(); // Nueva función para actualizar la tabla de películas
    void on_botonAgregarBeneficio_clicked();

    void on_botonAgregarPromocion_clicked();
    void actualizarTreePromociones();

    void on_botonAgregarPromocion_2_clicked();

    void on_botonCrearFuncion_clicked();

    void on_botonReservarAsiento_clicked();
    void actualizarTablaFuncion();

    void on_botonCrearSolicitud_clicked();
    void actualizarTablaSolicitudes();
    void on_botonSalir_clicked();

    void onReporteSeleccionado(int index);
    void onArchivoReporteCambiado(const QString& ruta);
    void onDirectorioReportesCambiado(const QString& ruta);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void inicializarTabReportes();
    void actualizarVigilanciaReportes();
    void refrescarReporteActual();
    void ajustarEscalaReporte();

    Ui::Dialog *ui;
    CrearPeli *crearPeli; // Instancia única persistente de CrearPeli
    agregarPromocion *ventanaPromocion;
    agregarBeneficio *ventanaBeneficio;
    guardarDatosService& guardar; // Referencia a la instancia compartida de guardarDatosService

    QFileSystemWatcher* watcherReportes;
    QComboBox* comboReportes;
    QLabel* visorReporte;
    QScrollArea* scrollReporte;
    QPixmap reporteOriginal;
    QString directorioReportes;
    std::vector<std::pair<QString, QString>> definicionesReportes;
};

#endif // ADMIN_H

