#include "ntesCloudMusic.h"

ntesCloudMusic::ntesCloudMusic(QObject *parent):
    OnlineMusic(_ntesCloudMusic, parent)
{

}

void ntesCloudMusic::getSuggestion(QString keyword)
{

}

SongList ntesCloudMusic::SearchFunc(QString keyword, int page)
{
    return SongList();
}
