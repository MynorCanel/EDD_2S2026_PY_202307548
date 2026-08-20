#ifndef CLIENTE_H
#define CLIENTE_H

#include <QMainWindow>

namespace Ui {
class cliente;
}

class cliente : public QMainWindow
{
    Q_OBJECT

public:
    explicit cliente(QWidget *parent = nullptr);
    ~cliente();

private:
    Ui::cliente *ui;
};

#endif // CLIENTE_H
