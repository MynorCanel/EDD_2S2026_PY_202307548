#include "registrocliente.h"
#include "ui_registrocliente.h"

registroCliente::registroCliente(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::registroCliente)
{
    ui->setupUi(this);
}

registroCliente::~registroCliente()
{
    delete ui;
}
