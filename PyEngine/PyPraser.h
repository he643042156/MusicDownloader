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

    //���ò����ӿ�
    //
    PyObject *StringToPyObj(QString str);
    PyObject *IntToPyObj(int val);

    QStringList GetStringFromPyListObject(PyObject *list) const;

    //ע�ắ��obj��vec��
    bool initFuncObjToMap(QString funcName);
    int initFuncListToMap(QStringList funcList);
    //�������ֻ�ȡ����obj
    PyObject *getFuncObjByName(QString funcName);
    //������ע��ĺ���
//    PyObject *callInitFunc(QString name, ...);

    bool callInitFunc(QString name, QString str);
    QStringList callInitFunc(QString name);
//    PyObject_CallObject(funcSetSearchKeyWord, Py_BuildValue("(s)", QString(QStringLiteral("������")).toStdString().c_str()))

private:
    PyObject *m_pModule;
    QMap<QString, PyObject *> m_funcObjMap;
    bool m_isInitSuccess;
};

#endif // PYPRASER_H
