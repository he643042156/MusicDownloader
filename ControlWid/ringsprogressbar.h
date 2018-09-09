#ifndef RINGSPROGRESSBAR_H
#define RINGSPROGRESSBAR_H

#include <QWidget>
#include "MusicProgressbar.h"

class RingsProgressbar : public MusicProgressbar
{
    Q_OBJECT
public:
    explicit RingsProgressbar(QWidget *parent = 0);
    void setRotateDelta(int delta);
    void setPersent(int persent);

signals:

public slots:
    virtual void slot_setDuration(quint64 duration);
    virtual void slot_currentPos(quint64 pos);
protected:
    void paintEvent(QPaintEvent *);

private slots:
    void timerUpdate();

private:
    int m_rotateAngle;//Ðý×ª½Ç¶È
    quint64 m_duration;
    QString m_currentTimeStr;

    typedef struct {
        bool flag;
        int count;
        int deep;
    }Pwm;

    Pwm m_inCircle;
    Pwm m_outCircle;

    void initPwm(Pwm &obj, bool isH, int deep);
    void breathe(Pwm &obj);
    void breathe(Pwm &obj1, Pwm &obj2);
    void setBreathe(bool isOff);
    bool m_isBreathOff;
};

#endif // RINGSPROGRESSBAR_H
