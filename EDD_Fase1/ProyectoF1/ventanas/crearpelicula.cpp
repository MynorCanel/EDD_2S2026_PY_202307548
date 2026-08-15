#include <QDialog>
#include "crearpelicula.h"
#include "ui_crearpelicula.h"

#pragma comment(lib, "Qt5Cored.lib")
#pragma comment(lib, "Qt5Guid.lib")
#pragma comment(lib, "Qt5Widgetsd.lib")

crearPelicula::crearPelicula(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::crearPelicula)
{
    ui->setupUi(this);
}

crearPelicula::~crearPelicula()
{
    delete ui;
}
