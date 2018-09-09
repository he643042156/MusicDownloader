#include "PyPraser.h"
#include <stdarg.h>
#include <qDebug>

PyPraser::PyPraser():m_isInitSuccess(false)
{
    Py_Initialize();
    //如果初始化失败，返回
    if(!Py_IsInitialized()){
        qDebug()<< QStringLiteral("python库初始化失败");
        Py_Finalize();
        m_isInitSuccess = false;
        return;
    }
    m_isInitSuccess = true;
}

PyPraser::PyPraser(QString fileName):m_isInitSuccess(false)
{
    Py_Initialize();
    //如果初始化失败，返回
    if(!Py_IsInitialized()){
        qDebug()<< QStringLiteral("python库初始化失败");
        Py_Finalize();
        m_isInitSuccess = false;
        return;
    }
    m_pModule = PyImport_ImportModule(fileName.toStdString().c_str());
    m_isInitSuccess = true;
}

PyPraser::~PyPraser()
{
    if(m_isInitSuccess)
        Py_Finalize();
}

bool PyPraser::setPyFile(QString fileName)
{
    if(!m_isInitSuccess)
        return false;

    m_pModule = PyImport_ImportModule(fileName.toStdString().c_str());
    if(!m_pModule)
    {
        qDebug() << QStringLiteral("python模块加载失败");
        return false;
    }
    return true;
}

bool PyPraser::initFuncObjToMap(QString funcName)
{
    if(!m_isInitSuccess)
        return false;

    if(m_pModule == NULL || m_funcObjMap.keys().contains(funcName))
        return false;
    PyObject *pFunc = PyObject_GetAttrString(m_pModule, funcName.toStdString().c_str());
    if(pFunc == NULL)
        return false;
    m_funcObjMap.insert(funcName, pFunc);
    return true;
}

int PyPraser::initFuncListToMap(QStringList funcList)
{
    if(!m_isInitSuccess)
        return 0;

    int retCount = 0;
    foreach (auto str, funcList) {
        if(initFuncObjToMap(str))
            retCount++;
    }
    return retCount;
}

PyObject *PyPraser::getFuncObjByName(QString funcName)
{
    if(!m_isInitSuccess)
        return NULL;

    if(m_funcObjMap.keys().contains(funcName))
        return m_funcObjMap[funcName];
    else
        return NULL;
}

//PyObject *PyPraser::callInitFunc(QString name, ...)
//{
////    va_list ap;
////    va_start(ap, count);
////    int sum = 0;
////    for(int i = 0; i < count; i++)
////        sum += va_arg(ap, int);

//////    PyObject_CallObject(m_funcObjMap[name], Py_BuildValue("(s)", QString(QStringLiteral("黄种人")).toStdString().c_str()))

////    va_end(ap);
//    return NULL;
//}

bool PyPraser::callInitFunc(QString name, QString str)
{
    if(!m_isInitSuccess)
        return false;

    PyObject_CallObject(m_funcObjMap[name], StringToPyObj(str));
    return true;
}

QStringList PyPraser::callInitFunc(QString name)
{
    if(!m_isInitSuccess)
        return QStringList();

    auto pRet = PyObject_CallObject(m_funcObjMap[name], NULL);
    return GetStringFromPyListObject(pRet);
}

PyObject *PyPraser::StringToPyObj(QString str)
{
    if(!m_isInitSuccess)
        return NULL;

    return Py_BuildValue("(s)", str.toStdString().c_str());
}

PyObject *PyPraser::IntToPyObj(int val)
{
    if(!m_isInitSuccess)
        return NULL;

    return Py_BuildValue("(i)", QString::number(val).toStdString().c_str());
}

QStringList PyPraser::GetStringFromPyListObject(PyObject *list) const
{
    if(!m_isInitSuccess)
        return QStringList();

    QStringList retList;
    retList.clear();
    if(list == NULL)
        retList.clear();

    int list_len = PyObject_Size(list);//列表长度40
    PyObject *list_item = NULL;//python类型的列表元素
    for (int i = 0; i < list_len; i++)
    {
        list_item = PyList_GetItem(list, i);//根据下标取出python列表中的元素
        PyObject* bytes = PyUnicode_AsUTF8String(list_item);
        char *file_name = PyBytes_AsString(bytes);
        retList.append(QString(file_name));
    }
    return retList;
}
