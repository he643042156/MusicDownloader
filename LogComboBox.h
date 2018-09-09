#ifndef LOGCOMBOBOX_H
#define LOGCOMBOBOX_H

#include <QComboBox>
#include <QCompleter>
#include <QSettings>

class LogComboBox : public QComboBox
{
public:
    LogComboBox(QWidget *parent = Q_NULLPTR);
    ~LogComboBox();

    void setCompleterStr(QString list);

public slots:
    void addLogItem(QString str);

private:
    const QString m_fileName = QStringLiteral("arinc429tool.ini");
    QSettings *m_settingFile;
    QStringList m_ipList;
    QCompleter *m_pCompleter;

    const quint8 m_logCount = 10;
};

#endif // LOGCOMBOBOX_H
