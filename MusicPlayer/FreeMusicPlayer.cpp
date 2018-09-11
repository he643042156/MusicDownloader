#include "FreeMusicPlayer.h"
#include <QUrl>

const QMap<QMediaPlayer::Error, QString> m_errorStrMap = {
    {QMediaPlayer::NoError, QStringLiteral("û�д���.")},
    {QMediaPlayer::ResourceError, QStringLiteral("����Դ�޷�����.")},
    {QMediaPlayer::FormatError, QStringLiteral("��Դ��ʽ����֧��.")},
    {QMediaPlayer::NetworkError, QStringLiteral("�������.")},
    {QMediaPlayer::AccessDeniedError, QStringLiteral("û���ʵ���Ȩ��������ý����Դ.")},
    {QMediaPlayer::ServiceMissingError, QStringLiteral("û���ҵ���Ч�Ļطŷ���,�طŲ��ܼ���..")},
};

const QMap<QMediaPlayer::MediaStatus, QString> m_statusStrMap = {
    {QMediaPlayer::UnknownMediaStatus, QStringLiteral("δ֪��״̬")},
    {QMediaPlayer::NoMedia, QStringLiteral("��ǰû��ý���ļ�")},
    {QMediaPlayer::LoadingMedia, QStringLiteral("��ǰý���ļ����ڼ���")},
    {QMediaPlayer::LoadedMedia, QStringLiteral("��ǰý���ļ��������")},
    {QMediaPlayer::StalledMedia, QStringLiteral("���ڻ��岻�������һЩ��ʱ�жϣ���ǰý���ļ��Ļط��Ѿ�ֹͣ")},
    {QMediaPlayer::BufferingMedia, QStringLiteral("�ò��������ڻ�������,�����㹻�����ݻ���,�Ա��ڲ��õĽ�����������")},
    {QMediaPlayer::BufferedMedia, QStringLiteral("�������Ѿ���ȫ�����˵�ǰ��ý���ļ�")},
    {QMediaPlayer::EndOfMedia, QStringLiteral("�ط��Ѿ������˵�ǰý���ļ��Ľ�β")},
    {QMediaPlayer::InvalidMedia, QStringLiteral("��Ч��ý���ļ�")},
};

FreeMusicPlayer::FreeMusicPlayer(QObject *parent):
    QThread(parent),
    m_pMusicPlayer(new QMediaPlayer(this))
{
    m_pMusicPlayer->setPlaylist(&m_playList);
    resetCurrentSong(m_currentSong);

    //���µ�ǰ����ʱ��
    connect(m_pMusicPlayer, &QMediaPlayer::durationChanged,
            this, [&](qint64 duration){
        emit FreeMusicPlayer::getInstance().currentDuration(duration);
        m_currentSong.duration = duration;
    });

    //���µ�ǰ��������
    connect(m_pMusicPlayer, &QMediaPlayer::positionChanged,
            this, [&](qint64 position){
        emit FreeMusicPlayer::getInstance().currentPos(position);
        if(position >= m_currentSong.duration){
            emit FreeMusicPlayer::getInstance().playComplete();
        }
        m_currentSong.currentPos = position;
    });

    //������״̬�ı�
    connect(m_pMusicPlayer, &QMediaPlayer::mediaStatusChanged,
    this, [&](QMediaPlayer::MediaStatus status){
        qDebug() << m_statusStrMap[status];
        emit playStatus(m_statusStrMap[status]);
    });

    //��ǰ���Ÿ��������ı�
    connect(&m_playList, &QMediaPlaylist::currentIndexChanged,
            this, [&](int position){
        if(position == -1){
            m_pMusicPlayer->stop();
            m_playList.clear();
            emit playListEnd();
            emit playStatus(QStringLiteral("��ǰ�ѵ��б�ĩβ"));
        }
        else if(position < m_songInfoList.size())
            emit currentSongChanged(m_songInfoList.at(position));
    });

    //�����쳣
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
        msg.setText(QString(QStringLiteral("�����б�Ϊ��")));
        msg.exec();
        return false;
    }
    try{
        m_pMusicPlayer->play();
    }
    catch(...){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("�����쳣")));
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
        msg.setText(QString(QStringLiteral("�����б�Ϊ��")));
        msg.exec();
        return;
    }
    qDebug() << m_playList.playbackMode();
    if(m_playList.currentIndex() >= m_playList.mediaCount()-1 && \
            m_playList.playbackMode() == QMediaPlaylist::Sequential){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("�Ѿ������һ����")));
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
        msg.setText(QString(QStringLiteral("�����б�Ϊ��")));
        msg.exec();
        return;
    }
    if(m_playList.currentIndex() <= 0 && \
            m_playList.playbackMode() == QMediaPlaylist::Sequential){
        QMessageBox msg;
        msg.setText(QString(QStringLiteral("�Ѿ��ǵ�һ����")));
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
