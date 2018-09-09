#include "SearchLineEditor.h"
#include "OnlineMusicManager.h"
#include <qDebug>


SearchLineEditor::SearchLineEditor(QWidget *parent):
    QLineEdit(parent),
    m_completer(new QCompleter(this)),
    m_model(new QStringListModel(this))
{
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->setCompleter(m_completer);
    m_completer->setModel(m_model);
    m_completer->setFilterMode(Qt::MatchContains);

    connect(this, &QLineEdit::textEdited, this, &SearchLineEditor::getOnlineSuggestion);
    connect(&OnlineMusicManager::getInstance(), &OnlineMusicManager::getSuggestionListComplete, this, &SearchLineEditor::setCompleteList);
}

void SearchLineEditor::editComplete()
{
    QStringList m_list;
    m_list.clear();
    QString text = this->text();
    if(QString::compare(text, QString("")) != 0)
    {
        bool is_contains = m_list.contains(text, Qt::CaseInsensitive);
        if(!is_contains)
        {
            m_list << text;
            m_model->setStringList(m_list);
        }
    }
}

void SearchLineEditor::setCompleteList(QStringList str)
{
    m_model->setStringList(str);

//    m_completer->popup.boundingRect(string).width();
    m_completer->complete(QRect(0,0,150,0));
}

void SearchLineEditor::getOnlineSuggestion(const QString &text)
{
    OnlineMusicManager::getInstance().getSuggestion(text);
}

void SearchLineEditor::mousePressEvent(QMouseEvent *ev)
{
    QLineEdit::mousePressEvent(ev);
//    this->getOnlineSuggestion(this->text());
}
