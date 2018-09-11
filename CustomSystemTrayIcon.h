#ifndef CUSTOMSYSTEMTRAYICON_H
#define CUSTOMSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QWidget>

/*
* 自定义系统托盘类
* 作者：陈鲁勇
* 邮箱：727057301@qq.com
* 创建时间：2017年2月10日16:26:48
* QT版本：5.0.2
* CSDN：http://blog.csdn.net/csnd_ayo
* **************************************
* 说明：
*   第一次使用instance时必须传入主窗口的指针
*   以保证托盘与程序相关联,使用前请确保在QT.pro中加入 C++11 的支持
*
* 示例代码：
    icon_ = CustomSystemTrayIcon::instance(this);
    icon_->setTray("托盘名字","托盘图标资源地址",
                   "托盘描述");
    // 显示托盘
    icon_->showTray();
    // 隐藏托盘
    //icon_->hideTray();
*/

class QAction;
class QMenu;

class CustomSystemTrayIcon : public QWidget
{
    Q_OBJECT

public:

    // 得到系统托盘实例
    static CustomSystemTrayIcon* instance(QWidget* _this = nullptr);

    /*
     * 设置托盘
     * 参数 _name：托盘标题
     * 参数 _icon：托盘图标
     * 参数 _describe：托盘描述
     *
         图标建议大小 20~40
    */
    void setTray(const QString& _name,
                 const QString& _icon,
                 const QString& _describe);
    void setTray(const QString& _name,
                 const QString& _icon);
    void setTray(const QString& _name);

    // 显示托盘
    void showTray(void);
    // 隐藏托盘
    void hideTray(void);

private:

    explicit CustomSystemTrayIcon(QWidget *parent = 0);
    // 初始化
    void init(void);

    // 初始化右键菜单
    void initMenu(void);

    // 初始化托盘
    void initTray(void);

    // 初始化信号
    void initSignal(void);

private slots:

    /*
     * 托盘点击事件
     * 参数 _action：具体的事件行为宏
    */
    void trayActivatedEvent(QSystemTrayIcon::ActivationReason _action);


protected:

    // 托盘描述内容
    QString trayDescribe_;

private:

    // 隐藏按钮(菜单)
    QAction* hideAction_ = nullptr;
    // 还原按钮(菜单)
    QAction* restoreAction_ = nullptr;
    // 退出按钮(菜单)
    QAction* quitAction_ = nullptr;
    // 根菜单
    QMenu* menu_ = nullptr;
    // 托盘实例
    QSystemTrayIcon* trayObject_ = nullptr;
    // 实例
    static CustomSystemTrayIcon* self_;

};

#endif // CUSTOMSYSTEMTRAYICON_H
