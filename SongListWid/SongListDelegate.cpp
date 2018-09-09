#include "SongListDelegate.h"
#include "FreeMusicPlayer.h"
#include "SongListWid.h"
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

SongListDelegate::SongListDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void SongListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() != SongListWid::listHeaderStr.indexOf(QStringLiteral("操作"))){
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        //button1->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
        button1->text = QStringLiteral("收藏");
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        //button2->rect = option.rect.adjusted(button1->rect.width() + 4, 4, -4, -4);
        button2->text = QStringLiteral("下载");
        button2->state |= QStyle::State_Enabled;
        buttons =new  QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<SongListDelegate *>(this))->m_btns.insert(index, buttons);
    }
    buttons->first->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4) , -4); //
    buttons->second->rect = option.rect.adjusted(buttons->first->rect.width() + 4, 4, -4, -4);
    painter->save();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());

    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->first, painter);
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->second, painter);
}

bool SongListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if (btns->first->rect.contains(e->x(), e->y())) {
                btns->first->state |= QStyle::State_Sunken;
            }
            else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state |= QStyle::State_Sunken;
            }
        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if (btns->first->rect.contains(e->x(), e->y())) {
                btns->first->state &= (~QStyle::State_Sunken);
//                if(btns->first->text.contains(QStringLiteral("收藏"))){
////                    FreeMusicPlayer::getInstance().StartMusic(getSongInfoFromIndex(index));
////                    btns->first->text = QStringLiteral("暂停");
//                }
//                else{
////                    FreeMusicPlayer::getInstance().suspendMusic();
//                    btns->first->text = QStringLiteral("播放");
//                }

//                showMsg(tr("btn1 column %1").arg(index.column()));
            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
//                showMsg(tr("btn2 row %1").arg(index.row()));
            }
        }
    }
    return true;
}

QString SongListDelegate::getDownLinkFromIndex(const QModelIndex &index)
{
    int column = SongListWid::listHeaderStr.indexOf(QStringLiteral("链接"));
    int row = index.row();
    QModelIndex linkIndex = index.model()->index(row, column);
    return index.model()->data(linkIndex,Qt::DisplayRole).toString();
}

Song SongListDelegate::getSongInfoFromIndex(const QModelIndex &index)
{
    Song ret;
    int row = index.row();
    int column = SongListWid::listHeaderStr.indexOf(QStringLiteral("链接"));
    QModelIndex tmpIndex = index.model()->index(row, column);
    ret.downLink = index.model()->data(tmpIndex, Qt::DisplayRole).toString();

    column = SongListWid::listHeaderStr.indexOf(QStringLiteral("歌曲"));
    tmpIndex = index.model()->index(row, column);
    ret.name = index.model()->data(tmpIndex,Qt::DisplayRole).toString();

    column = SongListWid::listHeaderStr.indexOf(QStringLiteral("歌手"));
    tmpIndex = index.model()->index(row, column);
    ret.singer = index.model()->data(tmpIndex,Qt::DisplayRole).toString();

    column = SongListWid::listHeaderStr.indexOf(QStringLiteral("时长"));
    tmpIndex = index.model()->index(row, column);
    ret.time = index.model()->data(tmpIndex,Qt::DisplayRole).toString();

    column = SongListWid::listHeaderStr.indexOf(QStringLiteral("来源"));
    tmpIndex = index.model()->index(row, column);
    ret.src = index.model()->data(tmpIndex,Qt::DisplayRole).value<QString>();

    return ret;
}

void SongListDelegate::showMsg(QString str)
{
    QMessageBox msg;
    msg.setText(str);
    msg.exec();
}


