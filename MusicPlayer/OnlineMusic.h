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

    //��ʼ����,����������źŷ���
    virtual void startSearch(const QString& keyword, int page = 1);
    //��ȡ�ؼ��ֹ���
    virtual void getSuggestion(QString keyword) = 0;
    //��ǰ��������ʶ
    MusicSrc playSrc() const;
    //��ǰ��ʶ�ַ���
    QString  playSrcStr() const;
signals:
    //ƥ��ؼ��ֵ���ʾ�ź�
    void getSuggestionListComplete(QStringList suggestion);
    //��ʼ�������ź�
    void searchStart();
    //�б�������ɵ��ź�
    void searchFinished(SongList);
    //���׸����������
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
