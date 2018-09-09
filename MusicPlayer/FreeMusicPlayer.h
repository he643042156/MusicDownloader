#ifndef FREEMUSICPLAYER_H
#define FREEMUSICPLAYER_H

#include <QMediaPlayer>
#include "musicCommon.h"
#include <QThread>
#include "PlayModeFactory.h"
#include <QTime>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QMediaPlaylist>

class FreeMusicPlayer:public QThread
{
    Q_OBJECT
public:
    static FreeMusicPlayer &getInstance(){
        static FreeMusicPlayer instance;
        return instance;
    }

    FreeMusicPlayer(QObject *parent = Q_NULLPTR);
    ~FreeMusicPlayer();

    void initPlayList(SongList list);

    void initPlayList(Song song);

    void clearPlayList();

    bool startPlay(QMediaPlaylist::PlaybackMode mode = QMediaPlaylist::Sequential);

    void stopPlay();

    bool pausePlay();

    bool continuePlay();

    void next();

    void last();

    void setPlayMode(QMediaPlaylist::PlaybackMode mode);

    //×´Ì¬²éÑ¯

    bool isPause();
public slots:
    void slot_clikedPlay(int currentSong, SongList list);
    void slot_gotoPersent(int persent);
signals:
    void currentDuration(quint64 ms);
    void currentPos(quint64 ms);
    void playStarted();
    void playStoped();
    void playComplete();
    void playStatus(QString);
    void playError(QString);

    void currentSongChanged(Song);

protected:
    virtual void run();
private:

    typedef struct{
        qint64 duration;
        qint64 pausePos;
        qint64 currentPos;
        qint64 index;
    }SongProperty;

    QMediaPlayer *m_pMusicPlayer;
    QMediaPlaylist m_playList;
    SongList m_songInfoList;

    SongProperty m_currentSong;

    bool m_isPause;

    void resetCurrentSong(SongProperty &song){
        song.currentPos = 0;
        song.duration = 0;
        song.pausePos = 0;
        m_isPause = false;
    }
};

#endif // FREEMUSICPLAYER_H
