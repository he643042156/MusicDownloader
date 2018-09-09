#ifndef OPERATIONWID_H
#define OPERATIONWID_H

#include <QWidget>

namespace Ui {
class OperationWid;
}

class OperationWid : public QWidget
{
    Q_OBJECT

public:
    explicit OperationWid(QWidget *parent = 0);
    ~OperationWid();

private slots:
    void on_pushButtonPlay_clicked();

private:
    Ui::OperationWid *ui;
};

#endif // OPERATIONWID_H
