#ifndef PLAYMODEFACTORY_H
#define PLAYMODEFACTORY_H

#include <QObject>
#include "musicCommon.h"

//typedef enum{
//    singlePlay,
//    listPlay,
//    randPlay,
//    singleCycle,
//    listCycle,
//    randCycle
//}PlayMode;

//class PlayModeFactory
//{
//public:
//    PlayModeFactory();
//};

//class RulerFactory:public PlayModeFactory
//{
//public:
//    onceModeFactory();
//};


class PlayRuler:public QObject
{
public:
    PlayRuler(){}
    virtual void initPlayList(SongList *restList, SongList *playedList)
    {
        m_restList = restList;
        m_playedList = playedList;
    }
    virtual bool hasNext() = 0;
    virtual Song nextSong() = 0;
protected:
    SongList *m_restList;
    SongList *m_playedList;
};

class singleOncePlayRule:public PlayRuler
{
public:
    virtual bool hasNext()
    {
        return false;
    }
    virtual Song nextSong()
    {
        Song tmp;
        return tmp;
    }
};

class listOncePlayRule:public PlayRuler
{
public:
    listOncePlayRule(){}
    virtual bool hasNext()
    {
        return !m_restList->isEmpty();
    }
    virtual Song nextSong()
    {
        m_restList->removeFirst();
//        m_playedList->append();
        return m_restList->first();
    }
};

class randOncePlayRule:public PlayRuler
{
public:
    randOncePlayRule(){}
    virtual bool hasNext()
    {
        return !m_restList->isEmpty();
    }
    virtual Song nextSong()
    {
        m_restList->removeAt(m_lastIndex);
        int size = m_restList->size();
        m_lastIndex = rand() % size;
        return m_restList->at(m_lastIndex);
    }
private:
    int m_lastIndex = 0;
};

class singleCyclePlayRule:public PlayRuler
{
public:
    singleCyclePlayRule(){}
    virtual bool hasNext()
    {
        return true;
    }
    virtual Song nextSong()
    {
        return m_restList->at(0);
    }
};

class listCyclePlayRule:public PlayRuler
{
public:
    listCyclePlayRule(){}
    virtual bool hasNext()
    {
        return true;
    }
    virtual Song nextSong()
    {
        if(m_currentIndex == m_restList->size() - 1){
            m_currentIndex = 0;
        }
        return m_restList->at(m_currentIndex);
    }
private:
    int m_currentIndex = 0;
};

class randCyclePlayRule:public PlayRuler
{
public:
    randCyclePlayRule(){}
    virtual bool hasNext()
    {
        return true;
    }
    virtual Song nextSong()
    {
        if(m_firtFlag){
            m_list = *m_restList;
        }
        if(m_playCount > m_restList->size()-1){
            m_list = *m_restList;
        }
        m_list.removeAt(m_lastIndex);
        int size = m_list.size();
        m_lastIndex = rand() % size;
        m_playCount++;
        return m_restList->at(m_lastIndex);
    }
private:
    int m_playCount = 0;
    int m_lastIndex = 0;
    bool m_firtFlag = true;
    SongList m_list;
};

#endif // PLAYMODEFACTORY_H
