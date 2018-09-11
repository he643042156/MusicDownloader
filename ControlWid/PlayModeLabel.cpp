#include "PlayModeLabel.h"

PlayModeLabel::PlayModeLabel(QWidget *parent):
    QLabel(parent),
    m_currentMode(QMediaPlaylist::Sequential)
{
    setplayMode(m_currentMode);
}

void PlayModeLabel::setplayMode(QMediaPlaylist::PlaybackMode mode)
{
    this->setPixmap(m_pixMap[mode]);
    FreeMusicPlayer::getInstance().setPlayMode(mode);
}

void PlayModeLabel::mousePressEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    int index = m_pixMap.keys().indexOf(m_currentMode);
    if(index == m_pixMap.keys().size() - 1){
        index = -1;
    }
    m_currentMode = m_pixMap.keys().at(++index);
    setplayMode(m_currentMode);
}
