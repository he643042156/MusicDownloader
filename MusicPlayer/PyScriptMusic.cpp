#include "PyScriptMusic.h"

PyScriptMusic::PyScriptMusic(QString fileName, QObject *parent):
    m_pyModule(new PyModule(parent)),
    OnlineMusic(parent)
{
    m_pyModule->setPyFile(fileName);
}

void PyScriptMusic::getSuggestion(QString keyword)
{
    emit getSuggestionListComplete(QStringList());
}

SongList PyScriptMusic::SearchFunc(QString keyword, int page)
{
    Q_UNUSED(page)
    SongList list;
    list.clear();
    m_pyModule->setSearchKeyWord(keyword);
    QStringList songsStr = m_pyModule->getSongnames();
    QStringList singersStr = m_pyModule->getSingers();
    QStringList linksStr = m_pyModule->getDownloadLink();
    for(int i=0; i<linksStr.size(); i++){
        Song tmp;
        tmp.downLink = linksStr.at(i);
        tmp.name = songsStr.at(i);
        tmp.time = QString("3:00");
        tmp.singer = singersStr.at(i);
        tmp.src = m_pyModule->getName();
        list.append(tmp);
    }
    return list;
}
