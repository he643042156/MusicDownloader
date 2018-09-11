#ifndef SEARCHLINEEDITOR_H
#define SEARCHLINEEDITOR_H

#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QListView>

class SearchLineEditor : public QLineEdit
{
    Q_OBJECT
public:
    SearchLineEditor(QWidget *parent = Q_NULLPTR);


private slots:
    void editComplete();
    void setCompleteList(QStringList str);

    void getOnlineSuggestion(const QString &text);
private:
    QCompleter *m_completer;
    QStringListModel *m_model;
//    QStringList m_list;

protected:
    void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);
};

class PopListView:public QListView
{
    PopListView();

};

#endif // SEARCHLINEEDITOR_H
