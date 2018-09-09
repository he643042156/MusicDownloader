#include "ringsprogressbar.h"
#include <QPainter>
#include <QTime>
#include <QTimer>

RingsProgressbar::RingsProgressbar(QWidget *parent) : MusicProgressbar(parent),
    m_rotateAngle(0),
    m_isBreathOff(false)
{
    initPwm(m_inCircle, false, 60);
    initPwm(m_outCircle, true, 30);


    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RingsProgressbar::timerUpdate);
    timer->start(20);
}

void RingsProgressbar::setPersent(int persent)
{
    if(persent != m_persent)
    {
        m_persent = persent;
        update();
    }
}

void RingsProgressbar::slot_setDuration(quint64 duration)
{
    m_duration = duration;
}

void RingsProgressbar::slot_currentPos(quint64 pos)
{
    QTime currentPos(0,0,0);
    m_currentTimeStr = currentPos.addMSecs(pos).toString("mm:ss");

    if(m_duration > 0)
        m_persent = int((double)pos/(double)m_duration * 100.0);

    update();
}

void RingsProgressbar::paintEvent(QPaintEvent *)
{
//    breathe(m_inCircle, m_outCircle);
    breathe(m_inCircle);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    m_rotateAngle = 360*m_persent/100;

    int side = qMin(width(), height());
//    QRectF outRect(m_outCircle.count/2, m_outCircle.count/2, side-m_outCircle.count, side-m_outCircle.count);
    QRectF outRect(0, 0, side, side);
    QRectF inRect(m_inCircle.count/2, m_inCircle.count/2, side-m_inCircle.count, side-m_inCircle.count);
    QString valueStr = m_currentTimeStr;//QString("03:%1").arg(QString::number(m_persent));

    //»­ÍâÔ²
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(97, 117, 118)));
    p.drawEllipse(outRect);
    p.setBrush(QBrush(QColor(100, 107, 250)));
    p.drawPie(outRect, (90-m_rotateAngle)*16, m_rotateAngle*16);
    //»­ÕÚÕÖ
    p.setBrush(palette().window().color());
    p.drawEllipse(inRect);
    //»­ÎÄ×Ö
    QFont f = QFont("Microsoft YaHei", 15, QFont::Bold);
    p.setFont(f);
    p.setFont(f);
    p.setPen(QColor("#555555"));
    p.drawText(inRect, Qt::AlignCenter, valueStr);

    //»­Ê¡ÂÔºÅ
//    int r1=50, r2=30, r3 = 20;
//    p.setBrush(QBrush(QColor(100, 107, 250)));
//    QRectF ellipsisRectB(80, 120, r1, r1);
//    QRectF ellipsisRectM(60, 105, r2, r2);
//    QRectF ellipsisRectS(40, 90, r3, r3);
//    p.drawEllipse(ellipsisRectB);
//    p.drawEllipse(ellipsisRectM);
//    p.drawEllipse(ellipsisRectS);
}

void RingsProgressbar::timerUpdate()
{
    this->update();
}

void RingsProgressbar::initPwm(RingsProgressbar::Pwm &obj, bool isH, int deep)
{
    if(isH){
        obj.flag = false;
        obj.count = deep;
        obj.deep = deep;
    }
    else{
        obj.flag = true;
        obj.count = 0;
        obj.deep = deep;
    }
}

void RingsProgressbar::breathe(RingsProgressbar::Pwm &obj)
{
    if(m_isBreathOff)
        return;
    if(obj.flag){
        obj.count+=2;
        if(obj.count == obj.deep){
            obj.flag = false;
        }
    }
    else{
        obj.count-=2;
        if(obj.count == 10)
            obj.flag = true;
    }
}

void RingsProgressbar::breathe(RingsProgressbar::Pwm &obj1, Pwm &obj2)
{
    if(m_isBreathOff)
        return;
    static bool flag = true;
    if(flag){
        obj1.count+=2;
        obj2.count-=2;
        if(obj1.count == obj1.deep){
            flag = false;
        }
        if(obj2.count == 0){
            flag = false;
        }
    }
    else{
        obj1.count-=2;
        obj2.count+=2;
        if(obj1.count == 0){
            flag = true;
        }
        if(obj2.count == obj2.deep){
            flag = true;
        }
    }
}

void RingsProgressbar::setBreathe(bool isOff)
{
    m_isBreathOff = isOff;
}
