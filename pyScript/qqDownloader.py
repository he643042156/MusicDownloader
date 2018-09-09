import requests
import urllib
import json
import re
import time
import random

def resolve(songmidStr):
    """
    resolve audio url
    :param url: like 'https://y.qq.com/n/yqq/song/000YU69H3N55rZ.html'
    :return:
    """
    url = 'https://y.qq.com/n/yqq/song/%s.html' % (songmidStr)
    songmid = re.search('/(\w+).html$', url).groups()[0]
    filename = 'C400' + songmid + '.m4a'
    guid = int(random.random() * 2147483647) * int(time.time() * 1000) % 10000000000

    d = {
        'format': 'json',
        'cid': 205361747,
        'uin': 0,
        'songmid': songmid,
        'filename': filename,
        'guid': guid,
    }
    requests.packages.urllib3.disable_warnings()
    r = requests.get('https://c.y.qq.com/base/fcgi-bin/fcg_music_express_mobile3.fcg', params=d, verify=False)
    vkey = json.loads(r.content.decode('utf-8'))['data']['items'][0]['vkey']
    if vkey == '':
        return ''
    audio_url = 'http://dl.stream.qqmusic.qq.com/%s?vkey=%s&guid=%s&uin=0&fromtag=66' % (filename, vkey, guid)
    return audio_url

def getSongmidStr(keyWord):
    word = keyWord
    requests.packages.urllib3.disable_warnings()
    res1 = requests.get('https://c.y.qq.com/soso/fcgi-bin/client_search_cp?&t=0&aggr=1&cr=1&catZhida=1&lossless=0&flag_qc=0&p=1&n=20&w='+word)
    jm1 = json.loads(res1.text.strip('callback()[]'))
    jm1 = jm1['data']['song']['list']
    mids = []
    songmids = []
    for j in jm1:
        try:
            mids.append(j['media_mid'])
            songmids.append(j['songmid'])
            api.songnames.append(j['songname'])
            api.singers.append(j['singer'][0]['name'])
        except:
            print('wrong')
    return songmids

class QtDownLoaderInterface:
    def __init__(self):
        self.songnames = []
        self.songIds = []
        self.singers = []
        self.link = []

    def clear(self):
        self.songnames.clear()
        self.songIds.clear()
        self.singers.clear()
        self.link.clear()

    def setSearchKeyWord(self, keyWord):
        self.clear()
        songmids = getSongmidStr(keyWord)
        for n in range(0, len(songmids)):
            self.link.append(resolve(songmids[n]))

    def getDownloadLink(self):
        return self.link

    def getSongnames(self):
        return self.songnames

    def getSingers(self):
        return self.singers


global api;
api = QtDownLoaderInterface()

def setSearchKeyWord(keyWord):
    return api.setSearchKeyWord(keyWord)

def getDownloadLink():
    return api.getDownloadLink()

def getSongnames():
    return api.getSongnames()

def getSingers():
    return api.getSingers()

setSearchKeyWord("天地争霸美猴王")
print(getDownloadLink())