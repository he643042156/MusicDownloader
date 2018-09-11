#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicCommon.h"
#include "SongListWid.h"
#include "CustomSystemTrayIcon.h"
#include <QLabel>

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

protected:
    void changeEvent(QEvent * event);

private:
    Ui::MainWindow *ui;
    SongListWid *m_listWid;
    QLabel *m_statusLabel;
    CustomSystemTrayIcon *m_systemTray;
};

#endif // MAINWINDOW_H
