#ifndef MUSICPROGRESSBAR_H
#define MUSICPROGRESSBAR_H

#include <QWidget>

class MusicProgressbar : public QWidget
{
    Q_OBJECT
public:
    explicit MusicProgressbar(QWidget *parent = nullptr);

signals:
    void currentPersent(int persent);

public slots:
    virtual void slot_setDuration(quint64 duration) = 0;
    virtual void slot_currentPos(quint64 pos) = 0;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    int m_persent; //°Ù·Ö±È
};

#endif // MUSICPROGRESSBAR_H
