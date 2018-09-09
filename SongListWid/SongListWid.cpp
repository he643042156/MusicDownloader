#include "SongListWid.h"
#include "OnlineMusicManager.h"
#include "SongListDelegate.h"

const QStringList SongListWid::listHeaderStr = {
    {QStringLiteral("歌曲")},
    {QStringLiteral("歌手")},
    {QStringLiteral("时长")},
    {QStringLiteral("链接")},
    {QStringLiteral("来源")},
    {QStringLiteral("操作")}
};

SongListWid::SongListWid(QWidget *parent):
    QTableView(parent),
    m_model(new SongListMode(this))
{
    m_model->setHorizontalHeaderLabels(listHeaderStr);
    this->setModel(m_model);
    this->setItemDelegate(new SongListDelegate(this));

    connect(this, &QTableView::clicked, this, [&](const QModelIndex &index){
        if(index.column() != listHeaderStr.indexOf(QStringLiteral("操作"))){
            auto song = m_model->list().at(index.row());
            emit clickedOneLine(song);
            emit clickedSong(index.row(), this->playList());
        }
    });

    m_pSearchIndicator = new QProgressIndicator(this);
    connect(&OnlineMusicManager::getInstance(), &OnlineMusicManager::searchStart, m_pSearchIndicator, &QProgressIndicator::startAnimation);
    connect(&OnlineMusicManager::getInstance(), &OnlineMusicManager::searchFinished, [&](SongList){
            m_pSearchIndicator->stopAnimation();
    });
}

SongListWid::~SongListWid()
{
    m_pSearchIndicator->deleteLater();
}

SongList SongListWid::playList()
{
    return m_model->list();
}

void SongListWid::appenSongList(SongList list)
{
    m_model->appendSong(list);
}

void SongListWid::clearList()
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels(listHeaderStr);
}

void SongListWid::startIndicatorAnimation()
{
    m_pSearchIndicator->startAnimation();
}

void SongListWid::stopIndicatorAnimation()
{
    m_pSearchIndicator->stopAnimation();
}

int SongListWid::getSongIndexFromList(Song song)
{
//    return m_model->index().key(song);
    return 0;
}
