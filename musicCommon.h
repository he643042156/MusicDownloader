#ifndef MUSICCOMMON_H
#define MUSICCOMMON_H
#include <QApplication>

//typedef enum{
//    _qqMusic,
//    _ntesCloudMusic,
//    _xiamiMusic,
//    _baiduMusic,
//    _unknow
//}MusicSrc;

//const QMap<MusicSrc, QString> gPlaySrcStrMap = {
//    {_qqMusic, QStringLiteral("qq“Ù¿÷")},
//    {_ntesCloudMusic, QStringLiteral("Õ¯“◊‘∆“Ù¿÷")},
//    {_xiamiMusic, QStringLiteral("œ∫√◊“Ù¿÷")},
//    {_baiduMusic, QStringLiteral("∞Ÿ∂»“Ù¿÷")},
//    {_unknow, QStringLiteral("Œ¥÷™")}
//};

typedef struct {
    QString name;
    QString singer;
    QString time;
    QString downLink;
    QString src;
}Song;

typedef QList<Song> SongList;

Q_DECLARE_METATYPE(SongList)
Q_DECLARE_METATYPE(Song)
//Q_DECLARE_METATYPE(MusicSrc)

#endif // MUSICCOMMON_H
