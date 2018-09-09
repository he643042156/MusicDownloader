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
     * @brief search ��������
     * @param keyword �ؼ���
     * @param page	ҳ��
     */
    Q_INVOKABLE void search(const QString& keyword, int page);

    /**
     * @brief getSuggestion ��ȡ��������
     * @param keyword �ٶ����ָ���id
     */
    Q_INVOKABLE void getSuggestion(QString keyword);

    /**
     * @brief getSongInfo ��ȡ������Ϣ
     * @param songId
     */
    Q_INVOKABLE void getSongInfo(QString songId);

    /**
     * @brief getSongLink ��ȡ�������ӣ������������Ӻ͸�����ӵ�
     * @param songId
     */
    Q_INVOKABLE void getSongLink(QString songId);

    /**
     * @brief getLyric ���ݸ���������ظ��
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

    //��������cookie
    CookieJar cookieJar;
    //ͳһ�������songidת��Ϊsid��songnameת��Ϊsname
    QString unifyResult(QString r);

    //����Suggest json�ַ���
    QStringList praseSuggestJsonStr(QString jsonStr);
    //����search json�ַ���
    QStringList praseSearchJsonStr(QString jsonStr);
private slots:
    void searchReplyFinished();
    void suggestionReplyFinished();
    void songInfoReplyFinished();
    void songLinkReplyFinished();
    void lyricReplyFinished();

    //������߳�����
    void postsRequest(QUrl url, ReplyType type);
signals:
    void postSignal(QUrl, ReplyType);
    /**
     * @brief searchComplete �������
     * @param currentPage ��ǰҳ
     * @param pageCount ��ҳ��
     * @param keyword �ؼ���
     * @param songList �����б�,json����
     * 	[
     * 		{"songItem":
     * 			{
     * 				"sid":877578,
     * 				"author":"Beyond",
     * 				"sname":"�������",
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
     * @brief getSuggestionComplete ��ȡ�����������
     * @param suggestion ��������json����
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
     * @brief getSongLinkComplete ��ȡ�����������
     * @param songLink
     */
    void getSongLinkComplete(QString songLink);

    void getLyricComplete(QString url,QString lyricContent);
public slots:
};
Q_DECLARE_METATYPE(BaiduMusic::ReplyType)

//����BaiduMusic����������
class BaiduMusicAdapter:public OnlineMusic
{
    Q_OBJECT
public:
    BaiduMusicAdapter(QObject *parent = nullptr):
        OnlineMusic(parent),
        m_pMusic(new BaiduMusic(this))
    {
        connect(m_pMusic, &BaiduMusic::searchComplete, this, &BaiduMusicAdapter::getResult);
        //����baiduMusic�źź�onlinMusic������ź�
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

    //����search json�ַ���
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
                tmpSong.src = QStringLiteral("�ٶ�����");
                tmpSong.name = map.value("sname").toString();
                tmpSong.singer = map.value("singer").toString();
                tmpSong.downLink = QStringLiteral("Ŭ���ƽ���...");
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
