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
                   QStringLiteral("����"),QStringLiteral("������Ҫ������̲�������"));
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
    // �Ҽ�����
    case QSystemTrayIcon::Context:
        break;
    // ���
    case QSystemTrayIcon::Trigger:
        break;
    // ˫��
    case QSystemTrayIcon::DoubleClick:
        this->parentWidget()->showNormal();
        break;
    // �м����
    case QSystemTrayIcon::MiddleClick:
        this->parentWidget()->hide();
        break;
    default:
        break;
    }
}

void CustomSystemTrayIcon::init() {
    //�ж�ϵͳ�Ƿ�֧��ϵͳ����ͼ��
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
#ifndef QT_NO_DEBUG
            QMessageBox::critical(nullptr,
                   QStringLiteral("����"),QStringLiteral("��ǰϵͳ��֧��ϵͳ����"));
#endif
        return;
    }

    // ���̶���
    trayObject_ = new QSystemTrayIcon(this);
    // ���ذ�ť���˵���
    hideAction_ = new QAction(QStringLiteral("�� ��(&Z)"),this);
    // ��ԭ��ť���˵���
    restoreAction_ = new QAction(QStringLiteral("�� ԭ(&X)"),this);
    // �˳���ť���˵���
    quitAction_ = new QAction(QStringLiteral("�� ��(&C)"),this);
    // ���˵�
    menu_ = new QMenu((QWidget*)QApplication::desktop());

    initMenu();
    initTray();
    initSignal();
}

void CustomSystemTrayIcon::initMenu()
{
    // ���ò˵�ͼ��
//    hideAction_->setIcon(QIcon(":/icon/hide"));
//    restoreAction_->setIcon(QIcon(":/icon/restore"));
//    quitAction_->setIcon(QIcon(":/icon/quit"));

    // ��֯�˵�˳��
    menu_->addAction(hideAction_);
    menu_->addAction(restoreAction_);
    //����һ�������
    menu_->addSeparator();
    menu_->addAction(quitAction_);
}

void CustomSystemTrayIcon::initTray()
{
    trayObject_->setIcon(QIcon(":/icon/logo"));   //����ͼ��ͼƬ
    //setWindowIcon(QIcon(":/ic/logo"));  //��ͼƬ���õ�������
    trayObject_->setToolTip("LuYong Chen");    //����ʱ��������ȥ����ʾ��Ϣ
    trayObject_->showMessage("LuYong Chen","QQ:727057301",QSystemTrayIcon::Information,10000);
    // ���������Ҽ��˵�
    trayObject_->setContextMenu(menu_);
    trayObject_->hide();
}

void CustomSystemTrayIcon::initSignal()
{
    // �˵���ܹ���
    connect(hideAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(hide()));
    connect(restoreAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(showNormal()));
    connect(quitAction_,SIGNAL(triggered()),
                  qApp,SLOT(quit()));

    // ���̼����¼�(�����˫�������ֵ��)
    connect(trayObject_,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayActivatedEvent(QSystemTrayIcon::ActivationReason)));
}
