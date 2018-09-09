#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "OnlineMusicManager.h"
#include "FreeMusicPlayer.h"
#include <QSlider>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1000,500);

    m_listWid = dynamic_cast<SongListWid *>(ui->widSongList);
    connect(m_listWid, &SongListWid::clickedSong, &FreeMusicPlayer::getInstance(), &FreeMusicPlayer::slot_clikedPlay);
    connect(&OnlineMusicManager::getInstance(), &OnlineMusicManager::searchFinished, this, &MainWindow::on_onLineMusicSearchFinished);
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
