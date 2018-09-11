#ifndef PLAYMODELABEL_H
#define PLAYMODELABEL_H

#include <QLabel>
#include "FreeMusicPlayer.h"

class PlayModeLabel : public QLabel
{
public:
    PlayModeLabel(QWidget *parent=Q_NULLPTR);

private:
    void setplayMode(QMediaPlaylist::PlaybackMode mode);

protected:
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

private:
    QMap<QMediaPlaylist::PlaybackMode, QPixmap> m_pixMap = {
        {QMediaPlaylist::CurrentItemInLoop, QPixmap(":/image/btn_circle_single.png")},
        {QMediaPlaylist::Sequential, QPixmap(":/image/btn_listOnce.png")},
        {QMediaPlaylist::Loop, QPixmap(":/image/btn_listcircle.png")},
        {QMediaPlaylist::Random, QPixmap(":/image/btn_listrandom.png")},
    };

    QMediaPlaylist::PlaybackMode m_currentMode;
};

#endif // PLAYMODELABEL_H
