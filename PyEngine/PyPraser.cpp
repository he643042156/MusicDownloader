#include "PyPraser.h"
#include <stdarg.h>
#include <qDebug>

PyPraser::PyPraser():
    m_isInitSuccess(false)
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

PyPraser::~PyPraser()
{
    if(m_isInitSuccess){
        Py_Finalize();
    }
}

int PyPraser::setPyFile(QString fileName)
{
    if(!m_isInitSuccess)
        return false;
    int moduleIndex = m_pModuleList.size();
    m_mutex.lock();
    auto pModule = PyImport_ImportModule(fileName.toStdString().c_str());
    m_mutex.unlock();
    if(!pModule)
    {
        qDebug() << QStringLiteral("python模块加载失败");
        return -1;
    }
    m_pModuleList.append(pModule);
    m_objfuncList.append(pyobjMap());//占位
    return moduleIndex;
}

bool PyPraser::initFuncObjToMap(int index, QString funcName)
{
    if(!m_isInitSuccess)
        return false;

    auto pModule = m_pModuleList.at(index);
    if(pModule == NULL || m_objfuncList[index].keys().contains(funcName))
        return false;
    m_mutex.lock();
    PyObject *pFunc = PyObject_GetAttrString(pModule, funcName.toStdString().c_str());
    m_mutex.unlock();
    if(pFunc == NULL)
        return false;
    m_objfuncList[index].insert(funcName, pFunc);
    return true;
}

int PyPraser::initFuncListToMap(int index, QStringList funcList)
{
    if(!m_isInitSuccess || m_pModuleList.size() < index || index < 0)
        return 0;

    int retCount = 0;
    foreach (auto str, funcList) {
        if(initFuncObjToMap(index, str))
            retCount++;
    }
    return retCount;
}

PyObject *PyPraser::getFuncObjByName(int index, QString funcName)
{
    if(!m_isInitSuccess)
        return NULL;

    if(m_objfuncList[index].keys().contains(funcName))
        return m_objfuncList[index][funcName];
    else
        return NULL;
}

bool PyPraser::callInitFunc(int index, QString name, QString str)
{
    if(!m_isInitSuccess)
        return false;
    auto funcPyObjectMap = m_objfuncList[index];
    m_mutex.lock();
    PyObject_CallObject(funcPyObjectMap[name], StringToPyObj(str));
    m_mutex.unlock();
    return true;
}

QStringList PyPraser::callInitFunc(int index, QString name)
{
    QStringList retList;
    retList.clear();
    if(!m_isInitSuccess)
        return retList;

    m_mutex.lock();
    auto list = PyObject_CallObject(m_objfuncList[index][name], NULL);
    if(list == NULL)
        return retList;

    int list_len = PyObject_Size(list);
    PyObject *list_item = NULL;//python类型的列表元素
    for (int i = 0; i < list_len; i++)
    {
        list_item = PyList_GetItem(list, i);//根据下标取出python列表中的元素
        PyObject* bytes = PyUnicode_AsUTF8String(list_item);
        char *file_name = PyBytes_AsString(bytes);
        retList.append(QString(file_name));
    }
    m_mutex.unlock();

    return retList;
}

QString PyPraser::callInitFuncRetStr(int index, QString name)
{
    char retStr[256] = {0};
    m_mutex.lock();
    auto result = PyObject_CallObject(m_objfuncList[index][name], NULL);
    PyArg_Parse(result, "s", retStr);
    m_mutex.unlock();
    QString ret(retStr);
    qDebug() << ret;
    return ret;
}

PyObject *PyPraser::StringToPyObj(QString str)
{
    if(!m_isInitSuccess)
        return NULL;
    auto ret = Py_BuildValue("(s)", str.toStdString().c_str());
    return ret;
}

PyObject *PyPraser::IntToPyObj(int val)
{
    if(!m_isInitSuccess)
        return NULL;
    auto ret = Py_BuildValue("(i)", QString::number(val).toStdString().c_str());
    return ret;
}
