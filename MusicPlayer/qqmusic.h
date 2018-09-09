#ifndef QQMUSIC_H
#define QQMUSIC_H

#include "MusicCommon.h"
#include "PyPraser.h"
#include "QProgressIndicator.h"
#include <QObject>
#include <QThread>
#include "OnlineMusic.h"

class qqMusic:public OnlineMusic
{
    Q_OBJECT
public:
    qqMusic(QObject *parent=Q_NULLPTR);
    ~qqMusic();

    void getSuggestion(QString keyword);
protected:
    SongList SearchFunc(QString keyword, int page = 0);
private:
    PyPraser m_praser;
};

#endif // QQMUSIC_H
