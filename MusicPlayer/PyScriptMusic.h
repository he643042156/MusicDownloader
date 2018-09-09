#ifndef PYSCRIPTMUSIC_H
#define PYSCRIPTMUSIC_H

#include "OnlineMusic.h"
#include "PyModule.h"

class PyScriptMusic : public OnlineMusic
{
    Q_OBJECT
public:
    PyScriptMusic(QString fileName, QObject *parent = nullptr);

    void getSuggestion(QString keyword);
protected:
    SongList SearchFunc(QString keyword, int page = 0);
private:
    PyModule *m_pyModule;
};

#endif // PYSCRIPTMUSIC_H
