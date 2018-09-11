#include <QApplication>
#include <QtGui>
#include <QMenu>
#include <QAction>

#ifndef QT_NO_DEBUG
#include <QMessageBox>
#endif

#include "CustomSystemTrayIcon.h"


CustomSystemTrayIcon* CustomSystemTrayIcon::self_ = nullptr;


CustomSystemTrayIcon::CustomSystemTrayIcon(QWidget *parent)
    : QWidget(parent) {
    init();
}

void CustomSystemTrayIcon::setTray(const QString &_name,
                                   const QString &_icon,
                                   const QString &_describe) {
    setTray(_name,_icon);
    trayDescribe_ = _describe;
}

void CustomSystemTrayIcon::setTray(const QString &_name,
                                   const QString &_icon) {
    setTray(_name);
    trayObject_->setIcon(QIcon(_icon));
}

void CustomSystemTrayIcon::setTray(const QString &_name) {
    trayObject_->setToolTip(_name);
}

void CustomSystemTrayIcon::showTray()
{
    trayObject_->show();
    trayObject_->showMessage(trayObject_->toolTip(),trayDescribe_,
                            QSystemTrayIcon::Information, 5000);
}

void CustomSystemTrayIcon::hideTray()
{
    trayObject_->hide();
}

CustomSystemTrayIcon* CustomSystemTrayIcon::instance(QWidget *_this)
{
    if (self_ == nullptr) {
        if (_this != nullptr) {
            self_ = new CustomSystemTrayIcon(_this);
        }
        else {
#ifndef QT_NO_DEBUG
            QMessageBox::critical(nullptr,
                   QStringLiteral("警告"),QStringLiteral("不符合要求的托盘操作代码"));
#endif
            return nullptr;
        }
    }
    return self_;
}

void CustomSystemTrayIcon::trayActivatedEvent(
        QSystemTrayIcon::ActivationReason _action) {
    switch(_action)
    {
    case QSystemTrayIcon::Unknown:
        trayObject_->showMessage(trayObject_->toolTip(),trayDescribe_,
                                QSystemTrayIcon::Information, 5000);
        break;
    // 右键单击
    case QSystemTrayIcon::Context:
        break;
    // 点击
    case QSystemTrayIcon::Trigger:
        break;
    // 双击
    case QSystemTrayIcon::DoubleClick:
        this->parentWidget()->showNormal();
        break;
    // 中键点击
    case QSystemTrayIcon::MiddleClick:
        this->parentWidget()->hide();
        break;
    default:
        break;
    }
}

void CustomSystemTrayIcon::init() {
    //判断系统是否支持系统托盘图标
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
#ifndef QT_NO_DEBUG
            QMessageBox::critical(nullptr,
                   QStringLiteral("警告"),QStringLiteral("当前系统不支持系统托盘"));
#endif
        return;
    }

    // 托盘对象
    trayObject_ = new QSystemTrayIcon(this);
    // 隐藏按钮（菜单）
    hideAction_ = new QAction(QStringLiteral("隐 藏(&Z)"),this);
    // 还原按钮（菜单）
    restoreAction_ = new QAction(QStringLiteral("还 原(&X)"),this);
    // 退出按钮（菜单）
    quitAction_ = new QAction(QStringLiteral("退 出(&C)"),this);
    // 根菜单
    menu_ = new QMenu((QWidget*)QApplication::desktop());

    initMenu();
    initTray();
    initSignal();
}

void CustomSystemTrayIcon::initMenu()
{
    // 设置菜单图标
//    hideAction_->setIcon(QIcon(":/icon/hide"));
//    restoreAction_->setIcon(QIcon(":/icon/restore"));
//    quitAction_->setIcon(QIcon(":/icon/quit"));

    // 组织菜单顺序
    menu_->addAction(hideAction_);
    menu_->addAction(restoreAction_);
    //加入一个分离符
    menu_->addSeparator();
    menu_->addAction(quitAction_);
}

void CustomSystemTrayIcon::initTray()
{
    trayObject_->setIcon(QIcon(":/icon/logo"));   //设置图标图片
    //setWindowIcon(QIcon(":/ic/logo"));  //把图片设置到窗口上
    trayObject_->setToolTip("LuYong Chen");    //托盘时，鼠标放上去的提示信息
    trayObject_->showMessage("LuYong Chen","QQ:727057301",QSystemTrayIcon::Information,10000);
    // 设置托盘右键菜单
    trayObject_->setContextMenu(menu_);
    trayObject_->hide();
}

void CustomSystemTrayIcon::initSignal()
{
    // 菜单项功能关联
    connect(hideAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(hide()));
    connect(restoreAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(showNormal()));
    connect(quitAction_,SIGNAL(triggered()),
                  qApp,SLOT(quit()));

    // 托盘激活事件(点击、双击、滚轮点击)
    connect(trayObject_,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayActivatedEvent(QSystemTrayIcon::ActivationReason)));
}
