#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "OnlineMusicManager.h"
#include "FreeMusicPlayer.h"
#include <QSlider>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_statusLabel(new QLabel(this))
{
    ui->setupUi(this);
    this->resize(1000,520);
    this->setWindowTitle(QStringLiteral("Free Music"));
    statusBar()->addPermanentWidget(m_statusLabel); //现实永久信息

    m_systemTray = CustomSystemTrayIcon::instance(this);
    m_systemTray->setTray(QStringLiteral("Free Music"),QString(":/image/freeMusic.png"),
                          QStringLiteral("这是一个调用python的可扩展在线音乐播放器."));

    m_listWid = dynamic_cast<SongListWid *>(ui->widSongList);
    connect(m_listWid, &SongListWid::clickedSong, &FreeMusicPlayer::getInstance(), &FreeMusicPlayer::slot_clikedPlay);
    connect(&OnlineMusicManager::getInstance(), &OnlineMusicManager::searchFinished, this, &MainWindow::on_onLineMusicSearchFinished);
    connect(&FreeMusicPlayer::getInstance() ,&FreeMusicPlayer::playStatus, this, [&](QString staStr){m_statusLabel->setText(staStr);});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSearch_clicked()
{
    QString keyWord = ui->lineEditSearch->text();
    OnlineMusicManager::getInstance().startSearch(keyWord);
}

void MainWindow::on_pushButtonClear_clicked()
{
    m_listWid->clearList();
}

void MainWindow::on_onLineMusicSearchFinished(SongList list)
{
    m_listWid->clearList();
    m_listWid->appenSongList(list);
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange)
        if(windowState() & Qt::WindowMinimized){
           hide();
           m_systemTray->showTray();
        }
    QMainWindow::changeEvent(event);
}
