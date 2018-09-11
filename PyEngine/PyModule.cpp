#include "PyModule.h"
#include "PyPraser.h"

PyModule::PyModule(QObject *parent) :
    QObject(parent),
    m_moduleIndex(-1)
{
}

bool PyModule::setPyFile(QString fileName)
{
    m_moduleIndex = PyPraser::getInstance().setPyFile(fileName);
    if(m_moduleIndex < 0){
        return false;
    }
    PyPraser::getInstance().initFuncObjToMap(m_moduleIndex, QString("setSearchKeyWord"));
    PyPraser::getInstance().initFuncObjToMap(m_moduleIndex, QString("getDownloadLink"));
    PyPraser::getInstance().initFuncObjToMap(m_moduleIndex, QString("getSongnames"));
    PyPraser::getInstance().initFuncObjToMap(m_moduleIndex, QString("getSingers"));
//    PyPraser::getInstance().initFuncObjToMap(m_moduleIndex, QString("getName"));
    return true;
}

void PyModule::setSearchKeyWord(QString keyWord)
{
    if(0 > m_moduleIndex)
        return;
    PyPraser::getInstance().callInitFunc(m_moduleIndex, QString("setSearchKeyWord"), keyWord);
}

QStringList PyModule::getSongnames()
{
    return PyPraser::getInstance().callInitFunc(m_moduleIndex, QString("getSongnames"));
}

QStringList PyModule::getSingers()
{
    return PyPraser::getInstance().callInitFunc(m_moduleIndex, QString("getSingers"));
}

QStringList PyModule::getDownloadLink()
{
    return PyPraser::getInstance().callInitFunc(m_moduleIndex, QString("getDownloadLink"));
}

QString PyModule::getName()
{
    return QStringLiteral("δ֪");
//    return PyPraser::getInstance().callInitFuncRetStr(m_moduleIndex, QString("getName"));
}
