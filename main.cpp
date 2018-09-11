#include "mainwindow.h"
#include "musicCommon.h"
#include <QApplication>
#include <qDebug>
#include <QString>
#include <QFile>
#include "CustomSystemTrayIcon.h"


int main(int argc, char *argv[])
{
    qRegisterMetaType<SongList>();
    qRegisterMetaType<Song>();
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QString qss;
    QFile qssFile(":/image/skin/qss/DarkStyle.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()){
        qss = qssFile.readAll();
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    return a.exec();
}
