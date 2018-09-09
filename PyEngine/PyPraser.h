#ifndef PYPRASER_H
#define PYPRASER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "math.h"
#include <Python.h>
#include <QMutex>

class PyPraser
{
public:
    static PyPraser &getInstance(){
        static PyPraser instance;
        return instance;
    }

    PyPraser();
    ~PyPraser();
    int setPyFile(QString fileName);

    //公用参数接口
    PyObject *StringToPyObj(QString str);
    PyObject *IntToPyObj(int val);
    //根据名字获取函数obj
    PyObject *getFuncObjByName(int index, QString funcName);

    //注册函数obj到vec中
    bool initFuncObjToMap(int index, QString funcName);
    int initFuncListToMap(int index, QStringList funcList);

    //调用已注册的函数
    bool callInitFunc(int index, QString name, QString str);
    QStringList callInitFunc(int index, QString name);
    QString callInitFuncRetStr(int index, QString name);

private:
    QList<PyObject *> m_pModuleList;
    typedef QMap<QString, PyObject *> pyobjMap;
    QList<pyobjMap> m_objfuncList;

    bool m_isInitSuccess;
    QMutex m_mutex;/*注意python函数多线程调用的安全问题*/
};

#endif // PYPRASER_H
