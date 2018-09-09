#ifndef PYMODULE_H
#define PYMODULE_H

#include <QObject>

class PyModule : public QObject
{
    Q_OBJECT
public:
    explicit PyModule(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PYMODULE_H