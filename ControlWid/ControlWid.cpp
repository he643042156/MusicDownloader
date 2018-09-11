#include "ControlWid.h"
#include "ui_ControlWid.h"
#include "FreeMusicPlayer.h"

ControlWid::ControlWid(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ControlWid),
    m_playFlag(false)
{
    ui->setupUi(this);
    //    ui->toolButtonPlay->setChecked(true);

    m_Progressbar = dynamic_cast<RingsProgressbar *>(ui->widgetBar);

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::currentDuration,
            this, &ControlWid::slot_setDuration);

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::currentPos,
            this, &ControlWid::slot_currentPos);

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::playStarted,
            this, [&](){
        ui->toolButtonPlay->setIcon(QIcon(":/image/player_pause.png"));
        m_playFlag = true;
    });

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::playListEnd,
            this, [&](){
        ui->toolButtonPlay->setIcon(QIcon(":/image/icon_player.png"));
        m_playFlag = false;
        ui->labelSongInfo->setScrollText(QStringLiteral(""));
    });

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::playComplete,
            this, [&](){/*ui->toolButtonPlay->setIcon(QIcon(":/image/icon_player.png"));*/});

    connect(&FreeMusicPlayer::getInstance(), &FreeMusicPlayer::currentSongChanged,
            ui->labelSongInfo, [&](Song song){
        ui->labelSongInfo->setScrollText(song);
    });

    connect(m_Progressbar, &MusicProgressbar::currentPersent,
            &FreeMusicPlayer::getInstance(), &FreeMusicPlayer::slot_gotoPersent);
}

ControlWid::~ControlWid()
{
    delete ui;
}

void ControlWid::slot_setDuration(quint64 duration)
{
    m_Progressbar->slot_setDuration(duration);
}

void ControlWid::slot_currentPos(quint64 pos)
{
    m_Progressbar->slot_currentPos(pos);
}

void ControlWid::on_toolButtonNext_clicked()
{
    FreeMusicPlayer::getInstance().next();
}

void ControlWid::on_toolButtonLast_clicked()
{
    FreeMusicPlayer::getInstance().last();
}

void ControlWid::on_toolButtonPlay_clicked()
{
    if(m_playFlag){
        if(FreeMusicPlayer::getInstance().pausePlay()){
            ui->toolButtonPlay->setIcon(QIcon(":/image/icon_player.png"));
            m_playFlag = false;
        }
    }
    else{
        if(FreeMusicPlayer::getInstance().continuePlay()){
            ui->toolButtonPlay->setIcon(QIcon(":/image/player_pause.png"));
            m_playFlag = true;
        }
    }
}
