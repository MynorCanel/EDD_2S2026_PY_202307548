#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private:
    Ui::Dialog *ui;
};

#endif // ADMIN_H
