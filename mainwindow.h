#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicCommon.h"
#include "SongListWid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonClear_clicked();

    void on_onLineMusicSearchFinished(SongList list);

private:
    Ui::MainWindow *ui;
    SongListWid *m_listWid;
};

#endif // MAINWINDOW_H
