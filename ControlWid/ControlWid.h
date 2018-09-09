#ifndef CONTROLWID_H
#define CONTROLWID_H

#include <QFrame>
#include "ringsprogressbar.h"
#include <QToolButton>

namespace Ui {
class ControlWid;
}

class ControlWid : public QFrame
{
    Q_OBJECT

public:
    explicit ControlWid(QWidget *parent = 0);
    ~ControlWid();

private slots:
    void slot_setDuration(quint64 duration);
    void slot_currentPos(quint64 pos);

    void on_toolButtonNext_clicked();

    void on_toolButtonLast_clicked();

    void on_toolButtonPlay_clicked();

private:
    Ui::ControlWid *ui;
    RingsProgressbar *m_Progressbar;
    bool m_playFlag;
};

#endif // CONTROLWID_H
