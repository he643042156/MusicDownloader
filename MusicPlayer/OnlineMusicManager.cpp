#include "OnlineMusicManager.h"
#include "baidumusic.h"
#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include "PyScriptMusic.h"

OnlineMusicManager::OnlineMusicManager(QObject *parent):
    QObject(parent)
{
    m_searchList.clear();
    m_musicSrcList.clear();
    m_finishedCount = 0;
    m_sugCount = 0;

//    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
//    qDebug()<<"current currentPath: "<<QDir::currentPath();
    //检索script目录下的py文件,生成对应音乐源
    auto pyFileList = findPyFileFromPath(QCoreApplication::applicationDirPath()+"/pyScript/", QStringLiteral("*.py"));
    foreach (auto fileName, pyFileList) {
        auto pMusic = new PyScriptMusic(fileName, this);
        connect(pMusic, &OnlineMusic::getSuggestionListComplete, this, &OnlineMusicManager::slot_srcGetSugComplete);
        connect(pMusic, &OnlineMusic::searchFinished, this, &OnlineMusicManager::slot_srcSearchFinished);
        m_musicSrcList.append(pMusic);
    }
}

OnlineMusicManager::~OnlineMusicManager()
{
    foreach (auto src, m_musicSrcList) {
        src->deleteLater();
    }
}

void OnlineMusicManager::startSearch(const QString &keyword, int page)
{
    foreach (auto src, m_musicSrcList) {
        src->startSearch(keyword, page);
    }
    if(m_musicSrcList.isEmpty()){
        QMessageBox msg;
        msg.setText(QStringLiteral("没有在线音乐搜索引擎."));
        msg.exec();
        return;
    }
    emit searchStart();
}

void OnlineMusicManager::getSuggestion(QString keyword)
{
    foreach (auto src, m_musicSrcList) {
        src->getSuggestion(keyword);
    }
}

QList<OnlineMusic *> OnlineMusicManager::musicSource() const
{
    return m_musicSrcList;
}

void OnlineMusicManager::slot_srcSearchFinished(SongList list)
{
    m_finishedCount++;
    m_searchList.append(list);
    if(m_finishedCount == m_musicSrcList.size()){
        emit searchFinished(m_searchList);
        m_searchList.clear();
        m_finishedCount = 0;
    }
}

void OnlineMusicManager::slot_srcGetSugComplete(QStringList suggestion)
{
    m_sugCount++;
    m_sugList.append(suggestion);
    if(m_sugCount == m_musicSrcList.size()){
        emit getSuggestionListComplete(m_sugList);
        m_sugList.clear();
        m_sugCount = 0;
    }
}

QStringList OnlineMusicManager::findPyFileFromPath(const QString &strFilePath, const QString filtersStr)
{
    QStringList retFilelist;//找到的文件存入此队列
    retFilelist.clear();
    if (strFilePath.isEmpty())
    {
        return retFilelist;
    }
    QDir dir;
    QStringList filters;
    filters << filtersStr;
    dir.setPath(strFilePath);
    dir.setNameFilters(filters);//添加过滤器
    QDirIterator iter(dir,QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        iter.next();
        QFileInfo info=iter.fileInfo();
        if (info.isFile())
        {
            retFilelist.append(info.baseName());
        }
    }
    return retFilelist;
}
