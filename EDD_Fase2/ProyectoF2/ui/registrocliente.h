#ifndef REGISTROCLIENTE_H
#define REGISTROCLIENTE_H

#include <QMainWindow>

namespace Ui {
class registroCliente;
}

class registroCliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit registroCliente(QWidget *parent = nullptr);
    ~registroCliente();

private:
    Ui::registroCliente *ui;
};

#endif // REGISTROCLIENTE_H
