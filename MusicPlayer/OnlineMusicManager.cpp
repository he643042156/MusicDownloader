#include "OnlineMusicManager.h"
#include "qqMusic.h"
#include "baidumusic.h"

OnlineMusicManager::OnlineMusicManager(QObject *parent):
    QObject(parent)
{
    m_searchList.clear();
    m_musicSrcList.clear();
    m_finishedCount = 0;
    m_sugCount = 0;
    OnlineMusic *pQQMusic = new qqMusic(this);
    OnlineMusic *pBAIDUMusic = new BaiduMusicAdapter(this);

    m_musicSrcList << pQQMusic;
//                   << pBAIDUMusic;

    foreach (auto src, m_musicSrcList) {
        connect(src, &OnlineMusic::getSuggestionListComplete, this, &OnlineMusicManager::slot_srcGetSugComplete);
//        connect(src, &OnlineMusic::searchStart, this, &OnlineMusicManager::searchStart);
        connect(src, &OnlineMusic::searchFinished, this, &OnlineMusicManager::slot_srcSearchFinished);
//        connect(src, &OnlineMusic::singleSearchFinished, this, &OnlineMusicManager::singleSearchFinished);
    }
}

OnlineMusicManager::~OnlineMusicManager()
{
    foreach (auto src, m_musicSrcList) {
        src->deleteLater();
    }
}

void OnlineMusicManager::startSearch(const QString &keyword, int page)
{
    foreach (auto src, m_musicSrcList) {
        src->startSearch(keyword, page);
    }
    emit searchStart();
}

void OnlineMusicManager::getSuggestion(QString keyword)
{
    foreach (auto src, m_musicSrcList) {
        src->getSuggestion(keyword);
    }
}

QList<OnlineMusic *> OnlineMusicManager::musicSource() const
{
    return m_musicSrcList;
}

void OnlineMusicManager::slot_srcSearchFinished(SongList list)
{
    m_finishedCount++;
    m_searchList.append(list);
    if(m_finishedCount == m_musicSrcList.size()){
        emit searchFinished(m_searchList);
        m_searchList.clear();
        m_finishedCount = 0;
    }
}

void OnlineMusicManager::slot_srcGetSugComplete(QStringList suggestion)
{
    m_sugCount++;
    m_sugList.append(suggestion);
    if(m_sugCount == m_musicSrcList.size()){
        emit getSuggestionListComplete(m_sugList);
        m_sugList.clear();
        m_sugCount = 0;
    }
}
