#ifndef PYMODULE_H
#define PYMODULE_H

#include <QObject>

/*针对单个py接口文件*/
class PyModule : public QObject
{
    Q_OBJECT
public:
    explicit PyModule(QObject *parent = nullptr);

    bool setPyFile(QString fileName);
    void setSearchKeyWord(QString keyWord);

    QStringList getSongnames();
    QStringList getSingers();
    QStringList getDownloadLink();
    QString getName();

signals:

public slots:

private:
    int m_moduleIndex;
};

#endif // PYMODULE_H
