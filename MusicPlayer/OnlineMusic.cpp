#include "OnlineMusic.h"

OnlineMusic::OnlineMusic(MusicSrc src, QObject *parent) :
    QThread(parent),
    m_playerSrc(src),
    m_keyWord(QString("")),
    m_isSearch(false)
{

}

MusicSrc OnlineMusic::playSrc() const
{
    return m_playerSrc;
}

QString OnlineMusic::playSrcStr() const
{
    return gPlaySrcStrMap[m_playerSrc];
}

void OnlineMusic::run()
{
    m_isSearch = true;
    emit searchStart();
    auto list = SearchFunc(m_keyWord, m_searchPage);
    emit searchFinished(list);
    m_isSearch = false;
}

void OnlineMusic::startSearch(const QString &keyword, int page)
{
    if(keyword == "" || m_isSearch || page < 0)
        return;
    m_keyWord = keyword;
    m_searchPage = page;
    this->start();
}
