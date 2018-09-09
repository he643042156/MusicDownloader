#include "MusicProgressbar.h"
#include <qDebug>
#include <QMouseEvent>
#include <cmath>
#define M_PI       3.14159265358979323846

MusicProgressbar::MusicProgressbar(QWidget *parent) :
    QWidget(parent),
    m_persent(0)
{

}

void MusicProgressbar::mousePressEvent(QMouseEvent *event)
{
    auto gPoint = event->globalPos();                          //获取全局位置

    int offset = qMin(width(), height());

    int x = this->mapFromGlobal(gPoint).x() - offset/2;
    int y = offset/2 - this->mapFromGlobal(gPoint).y();

    double angel = -atan2(y, x) + M_PI/2.0;
    if(angel < 0){
        angel = 2*M_PI +angel;
    }
    angel = 180 * (angel) / M_PI;

    m_persent = angel*100.0/360.0;
    emit currentPersent(m_persent);
    update();
    QWidget::mouseMoveEvent(event);
}
