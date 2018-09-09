#ifndef ONLINEMUSICMANAGER_H
#define ONLINEMUSICMANAGER_H

#include <QObject>
#include <QSemaphore>
#include "OnlineMusic.h"

class OnlineMusicManager : public QObject
{
    Q_OBJECT
public:
    static OnlineMusicManager &getInstance(){
        static OnlineMusicManager instance;
        return instance;
    }
    OnlineMusicManager(QObject *parent=Q_NULLPTR);
    ~OnlineMusicManager();

    void startSearch(const QString& keyword, int page = 1);

    void getSuggestion(QString keyword);

    QList<OnlineMusic *> musicSource() const;
signals:
    //匹配关键字的提示信号
    void getSuggestionListComplete(QStringList suggestion);
    //开始搜索的信号
    void searchStart();
    //列表搜索完成的信号
    void searchFinished(SongList);
    //单首歌曲搜索完成
    void singleSearchFinished(Song);

private slots:
    void slot_srcSearchFinished(SongList list);
    void slot_srcGetSugComplete(QStringList suggestion);
private:
    QList<OnlineMusic *> m_musicSrcList;
    QSemaphore m_searchSem;
    SongList m_searchList;
    QStringList m_sugList;
    int m_finishedCount;
    int m_sugCount;

    QStringList findPyFileFromPath(const QString &strFilePath, const QString filtersStr);
};

#endif // ONLINEMUSICMANAGER_H
