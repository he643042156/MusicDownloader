#ifndef ONLINEMUSIC_H
#define ONLINEMUSIC_H

#include <QObject>
#include <QThread>
#include "MusicCommon.h"

class SearchThread;

class OnlineMusic : public QThread
{
    Q_OBJECT
public:
    explicit OnlineMusic(MusicSrc src, QObject *parent = nullptr);

    //开始搜索,搜索完毕以信号发出
    virtual void startSearch(const QString& keyword, int page = 1);
    //获取关键字关联
    virtual void getSuggestion(QString keyword) = 0;
    //当前播放器标识
    MusicSrc playSrc() const;
    //当前标识字符串
    QString  playSrcStr() const;
signals:
    //匹配关键字的提示信号
    void getSuggestionListComplete(QStringList suggestion);
    //开始搜索的信号
    void searchStart();
    //列表搜索完成的信号
    void searchFinished(SongList);
    //单首歌曲搜索完成
    void singleSearchFinished(Song);

public slots:
protected:
    virtual SongList SearchFunc(QString keyword, int page = 0)=0;
    void run();
private:
    QString m_keyWord;
    int m_searchPage;
    bool m_isSearch;
    MusicSrc m_playerSrc;
};

#endif // ONLINEMUSIC_H
