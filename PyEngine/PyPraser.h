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

    //���ò����ӿ�
    PyObject *StringToPyObj(QString str);
    PyObject *IntToPyObj(int val);
    //�������ֻ�ȡ����obj
    PyObject *getFuncObjByName(int index, QString funcName);

    //ע�ắ��obj��vec��
    bool initFuncObjToMap(int index, QString funcName);
    int initFuncListToMap(int index, QStringList funcList);

    //������ע��ĺ���
    bool callInitFunc(int index, QString name, QString str);
    QStringList callInitFunc(int index, QString name);
    QString callInitFuncRetStr(int index, QString name);

private:
    QList<PyObject *> m_pModuleList;
    typedef QMap<QString, PyObject *> pyobjMap;
    QList<pyobjMap> m_objfuncList;

    bool m_isInitSuccess;
    QMutex m_mutex;/*ע��python�������̵߳��õİ�ȫ����*/
};

#endif // PYPRASER_H
