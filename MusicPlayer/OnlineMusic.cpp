#include "OnlineMusic.h"

OnlineMusic::OnlineMusic(QObject *parent) :
    QThread(parent),
    m_keyWord(QString("")),
    m_isSearch(false),
    m_src(QStringLiteral("δ֪"))
{

}

OnlineMusic::~OnlineMusic()
{
    if(this->isRunning()){
        this->terminate();
        this->wait(1000);
    }
}

QString OnlineMusic::playSrcStr() const
{
    //    return gPlaySrcStrMap[m_playerSrc];
    return m_src;
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

void OnlineMusic::setPlaySrc(QString src)
{
    m_src = src;
}
