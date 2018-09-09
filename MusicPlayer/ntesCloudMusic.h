#ifndef NTESCLOUDMUSIC_H
#define NTESCLOUDMUSIC_H

#include "OnlineMusic.h"
#include "PyPraser.h"

class ntesCloudMusic : public OnlineMusic
{
public:
    ntesCloudMusic(QObject *parent = nullptr);
    void getSuggestion(QString keyword);

protected:
    SongList SearchFunc(QString keyword, int page = 0);
private:
    PyPraser m_praser;
};

#endif // NTESCLOUDMUSIC_H
