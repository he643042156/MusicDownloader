#include "SongListDelegate.h"
#include "FreeMusicPlayer.h"
#include "SongListWid.h"
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPixmap>

SongListDelegate::SongListDelegate(QObject *parent):
    QStyledItemDelegate(parent),
    m_progressBarOption(NULL),
    m_fileButton(NULL)
{
    m_openFilePixmap = QPixmap(":/image/folder_open.png");
    m_openFileDisablePixmap = QPixmap(":/image/folder_open_disabled.png");

    m_button = new QPushButton;
    m_button->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(:/image/folder_open.png);} \
                            QPushButton:hover {image:url(:/image/folder_open_disabled.png);} \
                            QPushButton:pressed {image:url(:/image/folder_open_disabled.png);}"
                            );
}

void SongListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //    paintButton(painter, option, index);
    //    paintProgress(painter, option, index);
    if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("操作"))){
        QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
        if (!buttons) {
            QStyleOptionButton* button1 = new QStyleOptionButton();
            button1->text = QStringLiteral("收藏");
            button1->state |= QStyle::State_Enabled;

            QStyleOptionButton* button2 = new QStyleOptionButton();
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
    else if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("进度"))) {
        if(m_progressBarOption == NULL){
            int value = index.model()->data(index).toInt();
            (const_cast<SongListDelegate *>(this))->m_progressBarOption = new QStyleOptionProgressBarV2;
            m_progressBarOption->minimum = 0;
            m_progressBarOption->maximum = 100;
            m_progressBarOption->textAlignment = Qt::AlignRight;
            m_progressBarOption->textVisible = true;
            m_progressBarOption->progress = value;
            m_progressBarOption->text = QString("%1%").arg(m_progressBarOption->progress);
        }
        if(m_fileButton == NULL){
            (const_cast<SongListDelegate *>(this))->m_fileButton = new QStyleOptionButton();
            m_fileButton->state |= QStyle::State_Enabled;
//            m_fileButton->icon = QIcon(m_openFilePixmap);
//            m_fileButton->iconSize = m_openFilePixmap.size();
        }
        m_fileButton->rect = option.rect.adjusted(m_progressBarOption->rect.width() + 4, 4, -4, -4);
        m_progressBarOption->rect = option.rect.adjusted(4, 4, -(option.rect.width()/8 + 4) , -4);
        painter->save();
//        if (option.state & QStyle::State_Selected) {
//            painter->fillRect(option.rect, option.palette.highlight());
//            painter->setBrush(option.palette.highlightedText());
//        }
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
        }
        painter->restore();
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, m_progressBarOption, painter);
        QApplication::style()->drawControl(QStyle::CE_PushButton, m_fileButton, painter, m_button);

        //        int width= m_openFilePixmap.width();
        //        int height= m_openFilePixmap.height();
        //        QRect rect= option.rect;
        //        int x=rect.x()+rect.width()/2-width/2;
        //        int y=rect.y()+rect.height()/2-height/2;

        //        painter->drawPixmap(x,y,m_openFileDisablePixmap);
    }
    else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

bool SongListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("进度"))){
            if (m_fileButton->rect.contains(e->x(), e->y())) {
                m_fileButton->state |= QStyle::State_Sunken;
//                m_fileButton->icon = QIcon(m_openFileDisablePixmap);
//                m_fileButton->iconSize = m_fileButton->rect.size();
            }
        }

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

        if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("进度"))){
            if (m_fileButton->rect.contains(e->x(), e->y())) {
                m_fileButton->state &= (~QStyle::State_Sunken);
            }
        }


        if (m_btns.contains(index)) {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if (btns->first->rect.contains(e->x(), e->y())) {
                btns->first->state &= (~QStyle::State_Sunken);

            } else if(btns->second->rect.contains(e->x(), e->y())) {
                btns->second->state &= (~QStyle::State_Sunken);
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

void SongListDelegate::paintProgress(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("进度"))) {
        int value = index.model()->data(index).toInt();
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.rect = option.rect.adjusted(4, 4, -4, -4);
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignRight;
        progressBarOption.textVisible = true;
        progressBarOption.progress = value;
        progressBarOption.text = tr("%1%").arg(progressBarOption.progress);

        painter->save();
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setBrush(option.palette.highlightedText());
        }
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

        painter->restore();

    }
}

void SongListDelegate::paintButton(QPainter *painter,  const QStyleOptionViewItem &option,  const QModelIndex &index)
{
    if(index.column() == SongListWid::listHeaderStr.indexOf(QStringLiteral("操作"))){
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
}

void SongListDelegate::showMsg(QString str)
{
    QMessageBox msg;
    msg.setText(str);
    msg.exec();
}


