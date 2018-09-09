#ifndef PYPRASER_H
#define PYPRASER_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "math.h"
#include <Python.h>

class PyPraser
{
public:
    PyPraser();
    PyPraser(QString fileName);
    ~PyPraser();
    bool setPyFile(QString fileName);

    //公用参数接口
    //
    PyObject *StringToPyObj(QString str);
    PyObject *IntToPyObj(int val);

    QStringList GetStringFromPyListObject(PyObject *list) const;

    //注册函数obj到vec中
    bool initFuncObjToMap(QString funcName);
    int initFuncListToMap(QStringList funcList);
    //根据名字获取函数obj
    PyObject *getFuncObjByName(QString funcName);
    //调用已注册的函数
//    PyObject *callInitFunc(QString name, ...);

    bool callInitFunc(QString name, QString str);
    QStringList callInitFunc(QString name);
//    PyObject_CallObject(funcSetSearchKeyWord, Py_BuildValue("(s)", QString(QStringLiteral("黄种人")).toStdString().c_str()))

private:
    PyObject *m_pModule;
    QMap<QString, PyObject *> m_funcObjMap;
    bool m_isInitSuccess;
};

#endif // PYPRASER_H
