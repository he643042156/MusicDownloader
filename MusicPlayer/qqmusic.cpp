#include "qqMusic.h"
#include "Python.h"
#include "FreeMusicPlayer.h"

qqMusic::qqMusic(QObject *parent):
    OnlineMusic(_qqMusic, parent)
{
//    m_praser.setPyFile(QString(QStringLiteral("qqDownloader")));
//    m_praser.setPyFile(QString(QStringLiteral("NTESCloudDownloader")));


//    m_praser.initFuncObjToMap(QString("setSearchKeyWord"));
//    m_praser.initFuncObjToMap(QString("getDownloadLink"));
//    m_praser.initFuncObjToMap(QString("getSongnames"));
//    m_praser.initFuncObjToMap(QString("getSingers"));
}

qqMusic::~qqMusic()
{
}

void qqMusic::getSuggestion(QString keyword)
{
    emit getSuggestionListComplete(QStringList());
}

SongList qqMusic::SearchFunc(QString keyword, int page)
{
    SongList list;
    list.clear();
//    m_praser.callInitFunc(QString("setSearchKeyWord"), keyword);
//    QStringList songsStr = m_praser.callInitFunc(QString("getSongnames"));
//    QStringList singersStr = m_praser.callInitFunc(QString("getSingers"));
//    QStringList linksStr = m_praser.callInitFunc(QString("getDownloadLink"));
//    for(int i=0; i<linksStr.size(); i++){
//        Song tmp;
//        tmp.downLink = linksStr.at(i);
//        tmp.name = songsStr.at(i);
//        tmp.time = QString("3:00");
//        tmp.singer = singersStr.at(i);
//        tmp.src = playSrc();
//        list.append(tmp);
//    }
    return list;
}
