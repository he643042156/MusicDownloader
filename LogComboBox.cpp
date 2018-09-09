#include "LogComboBox.h"

LogComboBox::LogComboBox(QWidget *parent):
    QComboBox(parent),
    m_settingFile(new QSettings(m_fileName, QSettings::IniFormat))
{
    int i = 0;
    QString str;
    m_ipList.clear();
    do{
        str = m_settingFile->value(QString("Remote/ip%1").arg(i++)).toString();
        if(!str.isEmpty()){
            this->addItem(str);
            m_ipList << str;
        }
    }
    while(!str.isEmpty());

    m_pCompleter = new QCompleter(m_ipList, this);
    this->setCompleter(m_pCompleter);
    this->setEditable(true);
}

LogComboBox::~LogComboBox()
{
    delete m_settingFile;
}

void LogComboBox::setCompleterStr(QString list)
{
//    m_pCompleter->set
}

void LogComboBox::addLogItem(QString str)
{
    if(!m_ipList.contains(str)){
        if(m_ipList.size() < m_logCount){
            m_settingFile->setValue(QString("Remote/ip%1").arg(m_ipList.size()), str);
            m_ipList << str;
            this->addItem(str);
        }
        else{
            m_settingFile->setValue(QString("Remote/ip0"), str);
            m_ipList[0] = str;
            this->removeItem(0);
            this->addItem(str);
        }
    }
}
