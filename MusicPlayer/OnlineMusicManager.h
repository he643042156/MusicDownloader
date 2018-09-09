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
    //ƥ��ؼ��ֵ���ʾ�ź�
    void getSuggestionListComplete(QStringList suggestion);
    //��ʼ�������ź�
    void searchStart();
    //�б�������ɵ��ź�
    void searchFinished(SongList);
    //���׸����������
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
