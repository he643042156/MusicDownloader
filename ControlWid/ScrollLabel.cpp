#include "ScrollLabel.h"
#include <QTimer>
#include <QPainter>
#include <qDebug>

ScrollLabel::ScrollLabel(QWidget *parent):
    QLabel(parent)
{
    setMinimumWidth(200);
    setMinimumHeight(40);

    m_curIndex = 0;//当前角码
    m_rightIndex = 0;
//    m_showText = QStringLiteral("123");//显示的文字
    m_charWidth = 1;//().width("a");//每个字符的宽度

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ScrollLabel::updateIndex);
    m_timer->start(20);
}

void ScrollLabel::setScrollText(QString str)
{
    m_showText = str;
}

void ScrollLabel::setScrollText(Song song)
{
    m_timer->stop();
    m_curIndex = 0;//当前角码
    m_rightIndex = 0;
    m_showText = QString(QStringLiteral("%1  %2  %3")).arg(song.name).arg(song.singer).arg(song.src);
    QFont f = QFont("Microsoft YaHei", 13, QFont::Bold);
    QFontMetrics fm(f);
    QRect rec = fm.boundingRect(m_showText);
    m_strLen = rec.width();
    m_timer->start(20);
}

void ScrollLabel::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);
    QPainter painter(this);
    QFont f = QFont("Microsoft YaHei", 13, QFont::Bold);
    painter.setFont(f);

    if(m_charWidth*m_curIndex < m_strLen){
        painter.drawText(0-m_charWidth*m_curIndex, 30, m_showText);//.mid(m_curIndex));
        m_curIndex++;
    }

    int leftLen =(m_strLen - m_charWidth*m_curIndex);

    //10在这里是为了保持左右绘制的时候有一段空白的间距
    if((leftLen+10) >= width()){
        return;
    }
    if(width()-m_charWidth*m_rightIndex > 0){
        m_rightIndex++;
        painter.drawText(width()-m_charWidth*m_rightIndex, 30, m_showText.left(m_rightIndex));
    }
    else
    {
        painter.drawText(width()-m_charWidth*(++m_rightIndex), 30, m_showText.left(m_rightIndex));
        m_curIndex = 2;
        m_rightIndex = 0;
    }

    //    qDebug() << leftLen <<" " << m_charWidth*m_curIndex <<" "<< m_strLen;
}

void ScrollLabel::updateIndex()
{
    update();
}
