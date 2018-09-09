#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QByteArray>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <qDebug>


#include "baidumusic.h"

//页面大小
const int PAGESIZE = 20;

//音乐搜索API
//参数%1：搜索关键字
//参数%2: 起始位置，为(page-1)*20
//const QString ApiOfSearch = "http://music.baidu.com/search?key=%1&start=%2&size=20&s=1";

const QString ApiOfSearch = "http://music.taihe.com/search?key=%1&start=%2&size=20&s=1";

//搜索建议API
//参数%1：歌曲id
const QString ApiOfSuggestion = "http://sug.music.baidu.com/info/suggestion?format=json&word=%1&version=2&from=0";

//歌曲信息API
//参数%1：歌曲id
const QString ApiOfSongInfo = "http://play.baidu.com/data/music/songinfo?songIds=%1";

//歌曲链接API
//参数%1：歌曲id
const QString ApiOfSongLink = "http://play.baidu.com/data/music/songlink?songIds=%1&type=m4a,mp3";

///http://zhangmenshiting.qianqian.com/data2/music/348ece64e60b08ed2764de23296dfe56/598430891/598430891.mp3?xcode=d11e70bcc179cd5423407a79428c3f0f
///http://zhangmenshiting.qianqian.com/data2/music/9a33dd747734f833719726cdd35e3cb3/599476383/599476383.mp3?xcode=2db982c4591f94ab58a651629ec11350

BaiduMusic::BaiduMusic(QObject *parent) :
    QObject(parent),
    manager(new QNetworkAccessManager(parent))
{
    searchReply = 0;
    suggestionReply = 0;
    songInfoReply = 0;
    songLinkReply = 0;
    lyricReply = 0;
    manager->setCookieJar(&cookieJar);
    connect(this, &BaiduMusic::postSignal, this, &BaiduMusic::postsRequest, Qt::QueuedConnection);
}

BaiduMusic::~BaiduMusic()
{

}

//搜索关键字
void BaiduMusic::search(const QString &keyword, int page)
{
    //删除原来的响应
    if(searchReply){
        searchReply->deleteLater();
    }
    //起始位置
    int start = (page-1)*PAGESIZE;
    //构造请求链接url
    QUrl url = QUrl(ApiOfSearch.arg(keyword).arg(start));
    emit postSignal(url, _search);
}

void BaiduMusic::getSuggestion(QString keyword)
{
    if(suggestionReply){
        suggestionReply->deleteLater();
    }
    QUrl url = QUrl(ApiOfSuggestion.arg(keyword));
    emit postSignal(url, _suggestion);
}

void BaiduMusic::getSongInfo(QString songId)
{
    if(songInfoReply){
        songInfoReply->deleteLater();
    }
    QUrl url = QUrl(ApiOfSongInfo.arg(songId));
    emit postSignal(url, _songInfo);
}

void BaiduMusic::getSongLink(QString songId)
{
    if(songLinkReply){
        songLinkReply->deleteLater();
    }
    QUrl url = QUrl(ApiOfSongLink.arg(songId));
    emit postSignal(url, _songLink);
}

void BaiduMusic::getLyric(QString url)
{
    if(lyricReply){
        lyricReply->deleteLater();
    }
    emit postSignal(QUrl(url), _lyric);
}

QString BaiduMusic::unifyResult(QString r)
{
    r.replace(QRegularExpression("songid|songId"),"sid")
            .replace(QRegularExpression("author|artistname"),"singer")
            .replace(QRegularExpression("songname|songName"),"sname");
    emit getSuggestionListComplete(praseSuggestJsonStr(r));
    return r;
}

QStringList BaiduMusic::praseSuggestJsonStr(QString jsonStr)
{
    QByteArray buf = jsonStr.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf);
    if(jd.isObject())
    {
        QVariantMap map = jd.toVariant().toMap();
        auto songMap = map["data"].toMap();
        auto list = songMap["song"].toList();

        QStringList ret;
        foreach (auto tmp, list) {
            QString singer = tmp.toMap()["singer"].toString();
            QString sName = tmp.toMap()["sname"].toString();
            ret.append(sName + " - " +singer);
        }
        return ret;
    }
    return QStringList();
}

QStringList BaiduMusic::praseSearchJsonStr(QString jsonStr)
{
    QJsonParseError jsonError;
    QByteArray buf = jsonStr.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf, &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
//        QVariantMap map = jd.toVariant().toMap();
        auto songItemList = jd.toVariant();
        foreach (auto item, songItemList.toList()) {
            auto map = item.toMap()["songItem"].toMap();
            auto sname = map.value("sname").toString();
            auto singer = map.value("singer").toString();
            auto oid = map.value("oid").toString();
            auto sid = map.value("sid").toString();
//            qDebug() <<sname << " "<<singer << " "<<oid << " "<<sid << " ";
        }
        QStringList ret;
        return ret;
    }
    else{
        qDebug() << "json error " << jsonError.error;
    }
    return QStringList();
}

void BaiduMusic::searchReplyFinished()
{
    QString url = searchReply->request().url().toString();

    int keywordBegin = url.indexOf("key=") + 4;
    int keywordEnd = url.indexOf("&start=");

    int pageBeginPos = url.indexOf("start=") + 6;
    int pageEndPos = url.indexOf("&size=");

    //当前页
    int currentPage = url.mid(pageBeginPos,pageEndPos-pageBeginPos).toInt()/PAGESIZE + 1;
    //关键字
    QString keyword = url.mid(keywordBegin,keywordEnd-keywordBegin);
    if(searchReply->error()){

        //如果出错，pageCount为-1;
        emit searchComplete(currentPage,1,keyword,"{error:"+searchReply->errorString()+"}");
        return;
    }

    //TODO:未搜索到内容的判断

    QString html = searchReply->readAll();
    QStringList songList;
    QRegularExpression re("<li data-songitem = '(.+?)'");
    QRegularExpressionMatchIterator i = re.globalMatch(html);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString songData = match.captured(1);
        //&quot; 替换为 " ;删除<em>和</em>
        songData = songData.replace("&quot;","\"").replace("&lt;em&gt;","").replace("&lt;\\/em&gt;","");
        songList << songData;
    }

    //构造json数组
    QString songArray = "[" + songList.join(",") + "]";
    QString result = unifyResult(songArray);
    //匹配总页数
    QRegularExpression pageCountRe("\">(\\d+)</a>\\s*<a class=\"page-navigator-next\"");
    QRegularExpressionMatch match = pageCountRe.match(html);

    //页面总数
    int pageCount = match.captured(1).toInt();

    //如果没有 pageCount，则 pageCount 设为 1;
    pageCount = pageCount>0 ? pageCount : 1;

//    praseSearchJsonStr(result);
    emit searchComplete(currentPage,pageCount,keyword,result);
}

void BaiduMusic::suggestionReplyFinished()
{
    if(suggestionReply->error()){
        emit getSuggestionComplete("{error:"+suggestionReply->errorString()+"}");
        return;
    }
    QString sug = suggestionReply->readAll();
    emit getSuggestionComplete(unifyResult(sug));
}

void BaiduMusic::songInfoReplyFinished()
{
    if(songInfoReply->error()){
        emit getSongInfoComplete("{error:" + songInfoReply->errorString() + "}");
        return;
    }
    
    QString songinfo = songInfoReply->readAll();
    emit getSongInfoComplete(songinfo);
}

void BaiduMusic::songLinkReplyFinished()
{
    if(songLinkReply->error()){
        emit getSongLinkComplete("{error:" + songLinkReply->errorString() + "}");
        return;
    }   
    QString songlink = songLinkReply->readAll();
    emit getSongLinkComplete(unifyResult(songlink));
}

void BaiduMusic::lyricReplyFinished()
{
    QString url = lyricReply->url().toString();
    if(lyricReply->error()){
        emit getLyricComplete(url, "");
        return;
    }
    qDebug()<<lyricReply->rawHeaderList();
    emit getLyricComplete(url, lyricReply->readAll());
}

void BaiduMusic::postsRequest(QUrl url, BaiduMusic::ReplyType type)
{
    switch (type) {
    case _search:
        searchReply = manager->get(QNetworkRequest(url));
        connect(searchReply,SIGNAL(finished()),this,SLOT(searchReplyFinished()));
        break;
    case _suggestion:
        suggestionReply = manager->get(QNetworkRequest(url));
        connect(suggestionReply,SIGNAL(finished()),this,SLOT(suggestionReplyFinished()));
        break;
    case _songInfo:
        songInfoReply = manager->get(QNetworkRequest(url));
        connect(songInfoReply,SIGNAL(finished()),this,SLOT(songInfoReplyFinished()));
        break;
    case _songLink:
        songLinkReply = manager->get(QNetworkRequest(url));
        connect(songLinkReply,SIGNAL(finished()),this,SLOT(songLinkReplyFinished()));
        break;
    case _lyric:
        lyricReply = manager->get(QNetworkRequest(QUrl(url)));
        connect(lyricReply,SIGNAL(finished()),this,SLOT(lyricReplyFinished()));
        break;
    default:
        break;
    }
}
