#include "cliente.h"
#include "ui_cliente.h"

cliente::cliente(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cliente)
{
    ui->setupUi(this);
}

cliente::~cliente()
{
    delete ui;
}
