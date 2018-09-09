#ifndef SCROLLLABEL_H
#define SCROLLLABEL_H

#include <QLabel>
#include "musicCommon.h"

class ScrollLabel : public QLabel
{
public:
    ScrollLabel(QWidget *parent=Q_NULLPTR);

    void setScrollText(QString str);
    void setScrollText(Song song);

protected:
    void paintEvent(QPaintEvent *event);
    void updateIndex();


private:
    int m_charWidth;
    int m_curIndex;
    int m_rightIndex;
    QString m_showText;

    int m_strLen;
    QTimer *m_timer;
};

#endif // SCROLLLABEL_H
