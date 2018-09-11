#ifndef SONGLISTWID_H
#define SONGLISTWID_H

#include <QWidget>
#include "musicCommon.h"
#include "QProgressIndicator.h"
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>

class SongListMode:public QStandardItemModel
{
public:
    SongListMode(QObject *parent = Q_NULLPTR):QStandardItemModel(parent){
        m_list.clear();
        m_songIndexMap.clear();
    }
    virtual void appendSong(const Song &add){
        QList<QStandardItem*> rowItems;
        QStandardItem* itemSongName = new QStandardItem(add.name);
        QStandardItem* itemSinger = new QStandardItem(add.singer);
        QStandardItem* itemTime = new QStandardItem(add.time);
        QStandardItem* itemLink = new QStandardItem(add.downLink);
        QStandardItem* itemFrom = new QStandardItem(add.src);
        QStandardItem* itemOper = new QStandardItem();
        QStandardItem* itemProg = new QStandardItem();

        rowItems << itemSongName << itemSinger \
                 << itemTime << itemLink << itemFrom \
                 << itemOper << itemProg;

        QStandardItemModel::appendRow(rowItems);
        m_songIndexMap.insert(m_list.size(), add);
        m_list.append(add);

    }
    virtual void appendSong(const SongList &add){
        foreach (auto song, add) {
            appendSong(song);
        }
    }
    void clear(){
        QStandardItemModel::clear();
        m_list.clear();
        m_songIndexMap.clear();
    }
    SongList list() const{
        return m_list;
    }
    QMap<int, Song> index() const{
        return m_songIndexMap;
    }
private:
    SongList m_list;
    QMap<int, Song> m_songIndexMap;
};

class SongListWid : public QTableView
{
    Q_OBJECT

public:
    explicit SongListWid(QWidget *parent = 0);
    ~SongListWid();

    SongList playList();

    static  const QStringList listHeaderStr;
signals:
    void clickedOneLine(Song);
    void clickedSong(int, SongList);

public slots:
    void appenSongList(SongList list);
    void clearList();

    void startIndicatorAnimation();
    void stopIndicatorAnimation();

private:
    QProgressIndicator *m_pSearchIndicator;
    SongListMode *m_model;

    int getSongIndexFromList(Song song);
};

#endif // SONGLISTWID_H
