#ifndef CREARPELICULA_H
#define CREARPELICULA_H

#include <QDialog>

namespace Ui {
class crearPelicula;
}

class crearPelicula : public QDialog
{
    Q_OBJECT

public:
    explicit crearPelicula(QWidget *parent = nullptr);
    ~crearPelicula();

private:
    Ui::crearPelicula *ui;
};

#endif // CREARPELICULA_H
