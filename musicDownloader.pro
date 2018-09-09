#-------------------------------------------------
#
# Project created by QtCreator 2018-09-05T15:18:39
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = musicDownloader
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += -I  \
#    C:/Users/hwb/AppData/Local/Programs/Python/Python37/include \
    C:/Users/hwb/AppData/Local/Programs/Python/Python35/include \
    $$PWD/MusicPlayer \
    $$PWD/SongListWid \
    $$PWD/ControlWid \
    $$PWD/PyEngine

#LIBS += -LC:/Users/hwb/AppData/Local/Programs/Python/Python37/libs/ -lpython37
#LIBS += -LC:/Users/hwb/AppData/Local/Programs/Python/Python37/libs/ -lpython37_d

LIBS += -LC:/Users/hwb/AppData/Local/Programs/Python/Python35/libs/ -lpython35
LIBS += -LC:/Users/hwb/AppData/Local/Programs/Python/Python35/libs/ -lpython35_d

FORMS += \
    mainwindow.ui \
    ControlWid/ControlWid.ui

HEADERS += \
    MusicPlayer/FreeMusicPlayer.h \
    MusicPlayer/qqmusic.h \
    SongListWid/QProgressIndicator.h \
    SongListWid/SongListWid.h \
    mainwindow.h \
    musicCommon.h \
    SongListWid/SongListDelegate.h \
    MusicPlayer/PlayModeFactory.h \
    ControlWid/animationprogressbar.h \
    ControlWid/ControlWid.h \
    ControlWid/ringsprogressbar.h \
    ControlWid/MusicProgressbar.h \
    PyEngine/PyPraser.h \
    PyEngine/PyModule.h \
    ControlWid/ScrollLabel.h \
    MusicPlayer/baidumusic.h \
    MusicPlayer/cookiejar.h \
    SearchLineEditor.h \
    MusicPlayer/OnlineMusic.h \
    MusicPlayer/OnlineMusicManager.h \
    MusicPlayer/ntesCloudMusic.h

SOURCES += \
    MusicPlayer/FreeMusicPlayer.cpp \
    MusicPlayer/qqmusic.cpp \
    SongListWid/QProgressIndicator.cpp \
    SongListWid/SongListWid.cpp \
    main.cpp \
    mainwindow.cpp \
    SongListWid/SongListDelegate.cpp \
    MusicPlayer/PlayModeFactory.cpp \
    ControlWid/animationprogressbar.cpp \
    ControlWid/ControlWid.cpp \
    ControlWid/ringsprogressbar.cpp \
    ControlWid/MusicProgressbar.cpp \
    PyEngine/PyPraser.cpp \
    PyEngine/PyModule.cpp \
    ControlWid/ScrollLabel.cpp \
    MusicPlayer/baidumusic.cpp \
    MusicPlayer/cookiejar.cpp \
    SearchLineEditor.cpp \
    MusicPlayer/OnlineMusic.cpp \
    MusicPlayer/OnlineMusicManager.cpp \
    MusicPlayer/ntesCloudMusic.cpp

RESOURCES += \
    image.qrc
