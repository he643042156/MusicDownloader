#ifndef SONGLISTDELEGATE_H
#define SONGLISTDELEGATE_H

#include <QStyledItemDelegate>
#include "musicCommon.h"

class SongListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SongListDelegate(QObject *parent);

public slots:
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;
//    void setEditorData(QWidget *editor,const QModelIndex &index) const;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem *option,
//                              const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    typedef QMap<QModelIndex, QPair<QStyleOptionButton*, QStyleOptionButton*>* >  collButtons;
    collButtons m_btns;

    QString getDownLinkFromIndex(const QModelIndex &index);
    Song getSongInfoFromIndex(const QModelIndex &index);

    void showMsg(QString str);
};

#endif // SONGLISTDELEGATE_H
