#include "FreeMusicPlayer.h"
#include <QUrl>

const QMap<QMediaPlayer::Error, QString> m_errorStrMap = {
    {QMediaPlayer::NoError, QStringLiteral("没有错误.")},
    {QMediaPlayer::ResourceError, QStringLiteral("播放源无法解析.")},
    {QMediaPlayer::FormatError, QStringLiteral("资源格式不被支持.")},
    {QMediaPlayer::NetworkError, QStringLiteral("网络错误.")},
    {QMediaPlayer::AccessDeniedError, QStringLiteral("没有适当的权限来播放媒体资源.")},
    {QMediaPlayer::ServiceMissingError, QStringLiteral("没有找到有效的回放服务,回放不能继续..")},
};

const QMap<QMediaPlayer::MediaStatus, QString> m_statusStrMap = {
    {QMediaPlayer::UnknownMediaStatus, QStringLiteral("未知的状态")},
    {QMediaPlayer::NoMedia, QStringLiteral("当前没有媒体文件")},
    {QMediaPlayer::LoadingMedia, QStringLiteral("当前媒体文件正在加载")},
    {QMediaPlayer::LoadedMedia, QStringLiteral("当前媒体文件加载完毕")},
    {QMediaPlayer::StalledMedia, QStringLiteral("由于缓冲不足或其他一些临时中断，当前媒体文件的回放已经停止")},
    {QMediaPlayer::BufferingMedia, QStringLiteral("该播放器正在缓冲数据,但有足够的数据缓存,以便在不久的将来继续播放")},
    {QMediaPlayer::BufferedMedia, QStringLiteral("播放器已经完全缓冲了当前的媒体文件")},
    {QMediaPlayer::EndOfMedia, QStringLiteral("回放已经到达了当前媒体文件的结尾")},
    {QMediaPlayer::InvalidMedia, QStringLiteral("无效的媒体文件")},
};

FreeMusicPlayer::FreeMusicPlayer(QObject *parent):
    QThread(parent),
    m_pMusicPlayer(new QMediaPlayer(this))
{
    m_pMusicPlayer->setPlaylist(&m_playList);
    resetCurrentSong(m_currentSong);

    //更新当前歌曲时长
    connect(m_pMusicPlayer, &QMediaPlayer::durationChanged,
            this, [&](qint64 duration){
        emit FreeMusicPlayer::getInstance().currentDuration(duration);
        m_currentSong.duration = duration;
    });

    //更新当前歌曲进度
    connect(m_pMusicPlayer, &QMediaPlayer::positionChanged,
            this, [&](qint64 position){
        emit FreeMusicPlayer::getInstance().currentPos(position);
        if(position >= m_currentSong.duration){
            emit FreeMusicPlayer::getInstance().playComplete();
        }
        m_currentSong.currentPos = position;
    });

    //播放器状态改变
    connect(m_pMusicPlayer, &QMediaPlayer::mediaStatusChanged,
    this, [&](QMediaPlayer::MediaStatus status){
        qDebug() << m_statusStrMap[status];
        emit playStatus(m_statusStrMap[status]);
    });

    //当前播放歌曲发生改变
    connect(&m_playList, &QMediaPlaylist::currentIndexChanged,
            this, [&](int position){
        if(position == -1){
            m_pMusicPlayer->stop();
            m_playList.clear();
            emit playListEnd();
            emit playStatus(QStringLiteral("当前已到列表末尾"));
        }
        else if(position < m_songInfoList.size())
            emit currentSongChanged(m_songInfoList.at(position));
    });

    //播放异常
    connect(m_pMusicPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
        [=](QMediaPlayer::Error error){
        if(error != QMediaPlayer::NoError){
            emit playError(m_errorStrMap[error]);
        }
    });
}

FreeMusicPlayer::~FreeMusicPlayer()
{
    m_pMusicPlayer->deleteLater();
    this->deleteLater();
}

void FreeMusicPlayer::initPlayList(SongList list)
{
    m_playList.clear();
    m_songInfoList.clear();
    resetCurrentSong(m_currentSong);
    foreach (auto var, list) {
        initPlayList(var);
    }
}

void FreeMusicPlayer::initPlayList(Song song)
{
    m_playList.addMedia(QUrl(song.downLink));
    m_songInfoList.append(song);
}

void FreeMusicPlayer::clearPlayList()
{
    resetCurrentSong(m_currentSong);
    m_playList.clear();
    m_songInfoList.clear();
}

bool FreeMusicPlayer::startPlay(QMediaPlaylist::PlaybackMode mode)
{
    m_playList.setPlaybackMode(mode);
    if(m_playList.isEmpty()){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("播放列表为空")));
        msg.exec();
        return false;
    }
    try{
        m_pMusicPlayer->play();
    }
    catch(...){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("播放异常")));
        msg.exec();
        return false;
    }
    emit playStarted();
    return true;
}

void FreeMusicPlayer::stopPlay()
{
    m_pMusicPlayer->stop();
}

bool FreeMusicPlayer::pausePlay()
{
    m_pMusicPlayer->pause();
    m_currentSong.pausePos = m_currentSong.currentPos;
    return true;
}

bool FreeMusicPlayer::continuePlay()
{
    m_pMusicPlayer->setPosition(m_currentSong.pausePos);
    return this->startPlay(m_playList.playbackMode());
}

void FreeMusicPlayer::next()
{
    resetCurrentSong(m_currentSong);
    if(m_playList.isEmpty()){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("播放列表为空")));
        msg.exec();
        return;
    }
    qDebug() << m_playList.playbackMode();
    if(m_playList.currentIndex() >= m_playList.mediaCount()-1 && \
            m_playList.playbackMode() == QMediaPlaylist::Sequential){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("已经是最后一首了")));
        msg.exec();
        return;
    }
    m_playList.next();
    this->startPlay(m_playList.playbackMode());
}

void FreeMusicPlayer::last()
{
    resetCurrentSong(m_currentSong);
    if(m_playList.isEmpty()){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("播放列表为空")));
        msg.exec();
        return;
    }
    if(m_playList.currentIndex() <= 0 && \
            m_playList.playbackMode() == QMediaPlaylist::Sequential){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("已经是第一首了")));
        msg.exec();
        return;
    }
    m_playList.previous();
    this->startPlay(m_playList.playbackMode());
}

void FreeMusicPlayer::setPlayMode(QMediaPlaylist::PlaybackMode mode)
{
    m_playList.setPlaybackMode(mode);
}

qint64 FreeMusicPlayer::getDuration(QString url)
{
    return 0;//m_pMusicPlayer->newMedia(FileUrl ).duration;
}

bool FreeMusicPlayer::isPause()
{
    return m_isPause;
}

void FreeMusicPlayer::slot_clikedPlay(int currentSong, SongList list)
{
    this->initPlayList(list);
    if(currentSong < m_playList.mediaCount()){
        m_playList.setCurrentIndex(currentSong);
    }
    this->startPlay(m_playList.playbackMode());
}

void FreeMusicPlayer::slot_gotoPersent(int persent)
{
    m_currentSong.pausePos = (double)persent/100.0 * m_currentSong.duration;
    m_pMusicPlayer->setPosition(m_currentSong.pausePos);
}

void FreeMusicPlayer::run()
{
//    m_pMusicPlayer->play();
}
