#include "OperationWid.h"
#include "ui_OperationWid.h"
#include "FreeMusicPlayer.h"

OperationWid::OperationWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OperationWid)
{
    ui->setupUi(this);
}

OperationWid::~OperationWid()
{
    delete ui;
}

void OperationWid::on_pushButtonPlay_clicked()
{
    if(ui->pushButtonPlay->text().contains(QStringLiteral("²¥·Å"))){
//        FreeMusicPlayer::getInstance().StartMusic("http://dl.stream.qqmusic.qq.com/C400003OUlho2HcRHC.m4a?vkey=EAA6A427B5526785039A787B8380A77EAFBDA044357114C41567545DD74BCF70B11093436C16CCD31C4BDF4D1DCFD64BB3AC653698AE4C0A&guid=209661308&uin=0&fromtag=66");
        ui->pushButtonPlay->setText(QStringLiteral("ÔÝÍ£"));
    }
    else{
        FreeMusicPlayer::getInstance().suspendMusic();
        ui->pushButtonPlay->setText(QStringLiteral("²¥·Å"));
    }
}
