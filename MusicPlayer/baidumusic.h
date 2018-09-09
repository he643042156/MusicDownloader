#ifndef BAIDUMUSIC_H
#define BAIDUMUSIC_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include "MusicCommon.h"
#include "OnlineMusic.h"
#include "cookiejar.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSemaphore>

class BaiduMusic : public QObject
{
    Q_OBJECT
public:
    explicit BaiduMusic(QObject *parent = 0);
    ~BaiduMusic();
    /**
     * @brief search 搜索歌曲
     * @param keyword 关键字
     * @param page	页数
     */
    Q_INVOKABLE void search(const QString& keyword, int page);

    /**
     * @brief getSuggestion 获取搜索建议
     * @param keyword 百度音乐歌曲id
     */
    Q_INVOKABLE void getSuggestion(QString keyword);

    /**
     * @brief getSongInfo 获取歌曲信息
     * @param songId
     */
    Q_INVOKABLE void getSongInfo(QString songId);

    /**
     * @brief getSongLink 获取歌曲链接，包括下载链接和歌词连接等
     * @param songId
     */
    Q_INVOKABLE void getSongLink(QString songId);

    /**
     * @brief getLyric 根据歌词链接下载歌词
     * @param url
     */
    Q_INVOKABLE void getLyric(QString url);

    typedef enum{
        _search,
        _suggestion,
        _songInfo,
        _songLink,
        _lyric
    }ReplyType;
private:
    QNetworkAccessManager *manager;
    QNetworkReply* searchReply;
    QNetworkReply* suggestionReply;
    QNetworkReply* songInfoReply;
    QNetworkReply* songLinkReply;
    QNetworkReply* lyricReply;

    //保存所有cookie
    CookieJar cookieJar;
    //统一结果，如songid转换为sid，songname转换为sname
    QString unifyResult(QString r);

    //解析Suggest json字符串
    QStringList praseSuggestJsonStr(QString jsonStr);
    //解析search json字符串
    QStringList praseSearchJsonStr(QString jsonStr);
private slots:
    void searchReplyFinished();
    void suggestionReplyFinished();
    void songInfoReplyFinished();
    void songLinkReplyFinished();
    void lyricReplyFinished();

    //解决跨线程问题
    void postsRequest(QUrl url, ReplyType type);
signals:
    void postSignal(QUrl, ReplyType);
    /**
     * @brief searchComplete 搜索完毕
     * @param currentPage 当前页
     * @param pageCount 总页数
     * @param keyword 关键字
     * @param songList 歌曲列表,json数据
     * 	[
     * 		{"songItem":
     * 			{
     * 				"sid":877578,
     * 				"author":"Beyond",
     * 				"sname":"海阔天空",
     * 				"oid":877578,
     * 				"pay_type":"2"
     * 			}
     * 		},
     * 		{"songItem":
     * 			...
     * 		},
     * 		...
     * ]
     */
    void searchComplete(int currentPage,int pageCount,QString keyword, QString songList);

    /**
     * @brief getSuggestionComplete 获取搜索建议完毕
     * @param suggestion 搜索建议json数据
     * {
     *    "data": {
     *           "song": [{
     *               "songid": "877578",
     *               "songname": "\u6d77\u9614\u5929\u7a7a",
     *               "encrypted_songid": "",
     *               "has_mv": "1",
     *               "yyr_artist": "0",
     *               "artistname": "Beyond"
     *           },
     *           ...
     *          ],
     *           "artist": [{
     *               "artistid": "2345733",
     *               "artistname": "\u6d77\u9614\u5929\u7a7a",
     *               "artistpic": "http:\/\/a.hiphotos.baidu.com\/ting\/pic\/item\/6d81800a19d8bc3eb42695cc808ba61ea8d3458d.jpg",
     *               "yyr_artist": "0"
     *           },
     *           ...
     *          ],
     *           "album": [{
     *               "albumid": "197864",
     *               "albumname": "\u6d77\u9614\u5929\u7a7a",
     *               "artistname": "Beyond",
     *               "artistpic": "http:\/\/a.hiphotos.baidu.com\/ting\/pic\/item\/6c224f4a20a4462314dd8c409a22720e0cf3d7f8.jpg"
     *           },
     *           ...
     *           ]
     *       },
     *       "Pro": ["artist", "song", "album"]
     *   }
     *
     */

    void getSuggestionComplete(QString suggestion);

    void getSuggestionListComplete(QStringList suggestion);

    void getSongInfoComplete(QString songInfo);

    /**
     * @brief getSongLinkComplete 获取歌曲连接完毕
     * @param songLink
     */
    void getSongLinkComplete(QString songLink);

    void getLyricComplete(QString url,QString lyricContent);
public slots:
};
Q_DECLARE_METATYPE(BaiduMusic::ReplyType)

//创建BaiduMusic对象适配器
class BaiduMusicAdapter:public OnlineMusic
{
    Q_OBJECT
public:
    BaiduMusicAdapter(QObject *parent = nullptr):
        OnlineMusic(parent),
        m_pMusic(new BaiduMusic(this))
    {
        connect(m_pMusic, &BaiduMusic::searchComplete, this, &BaiduMusicAdapter::getResult);
        //连接baiduMusic信号和onlinMusic虚基类信号
        connect(m_pMusic, &BaiduMusic::getSuggestionListComplete, this, &OnlineMusic::getSuggestionListComplete);
    }

    void getSuggestion(QString keyword){
        m_pMusic->getSuggestion(keyword);
    }

protected:
    SongList SearchFunc(QString keyword, int page = 1){
        m_pMusic->search(keyword, page);
        m_usedSpace.acquire();
        return m_songList;
    }
private slots:
    void getResult(int currentPage,int pageCount,QString keyword, QString songList){
        praseSearchJsonStr(songList);
        m_usedSpace.release();
    }
private:
    SongList m_songList;
    BaiduMusic *m_pMusic;
    QSemaphore m_usedSpace;

    //解析search json字符串
    void praseSearchJsonStr(QString jsonStr){
        QJsonParseError jsonError;
        QByteArray buf = jsonStr.toUtf8();
        QJsonDocument jd = QJsonDocument::fromJson(buf, &jsonError);
        if(jsonError.error == QJsonParseError::NoError)
        {
            m_songList.clear();
            auto songItemList = jd.toVariant();
            foreach (auto item, songItemList.toList()) {
                auto map = item.toMap()["songItem"].toMap();

                Song tmpSong;
                tmpSong.src = QStringLiteral("百度音乐");
                tmpSong.name = map.value("sname").toString();
                tmpSong.singer = map.value("singer").toString();
                tmpSong.downLink = QStringLiteral("努力破解中...");
                tmpSong.time = QString("3:00");
//                auto oid = map.value("oid").toString();
//                auto sid = map.value("sid").toString();
                m_songList.append(tmpSong);
            }
        }
        else{
            qDebug() << "json error " << jsonError.error;
        }
    }
};

#endif // BAIDUMUSIC_H
