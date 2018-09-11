#ifndef CUSTOMSYSTEMTRAYICON_H
#define CUSTOMSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QWidget>

/*
* �Զ���ϵͳ������
* ���ߣ���³��
* ���䣺727057301@qq.com
* ����ʱ�䣺2017��2��10��16:26:48
* QT�汾��5.0.2
* CSDN��http://blog.csdn.net/csnd_ayo
* **************************************
* ˵����
*   ��һ��ʹ��instanceʱ���봫�������ڵ�ָ��
*   �Ա�֤��������������,ʹ��ǰ��ȷ����QT.pro�м��� C++11 ��֧��
*
* ʾ�����룺
    icon_ = CustomSystemTrayIcon::instance(this);
    icon_->setTray("��������","����ͼ����Դ��ַ",
                   "��������");
    // ��ʾ����
    icon_->showTray();
    // ��������
    //icon_->hideTray();
*/

class QAction;
class QMenu;

class CustomSystemTrayIcon : public QWidget
{
    Q_OBJECT

public:

    // �õ�ϵͳ����ʵ��
    static CustomSystemTrayIcon* instance(QWidget* _this = nullptr);

    /*
     * ��������
     * ���� _name�����̱���
     * ���� _icon������ͼ��
     * ���� _describe����������
     *
         ͼ�꽨���С 20~40
    */
    void setTray(const QString& _name,
                 const QString& _icon,
                 const QString& _describe);
    void setTray(const QString& _name,
                 const QString& _icon);
    void setTray(const QString& _name);

    // ��ʾ����
    void showTray(void);
    // ��������
    void hideTray(void);

private:

    explicit CustomSystemTrayIcon(QWidget *parent = 0);
    // ��ʼ��
    void init(void);

    // ��ʼ���Ҽ��˵�
    void initMenu(void);

    // ��ʼ������
    void initTray(void);

    // ��ʼ���ź�
    void initSignal(void);

private slots:

    /*
     * ���̵���¼�
     * ���� _action��������¼���Ϊ��
    */
    void trayActivatedEvent(QSystemTrayIcon::ActivationReason _action);


protected:

    // ������������
    QString trayDescribe_;

private:

    // ���ذ�ť(�˵�)
    QAction* hideAction_ = nullptr;
    // ��ԭ��ť(�˵�)
    QAction* restoreAction_ = nullptr;
    // �˳���ť(�˵�)
    QAction* quitAction_ = nullptr;
    // ���˵�
    QMenu* menu_ = nullptr;
    // ����ʵ��
    QSystemTrayIcon* trayObject_ = nullptr;
    // ʵ��
    static CustomSystemTrayIcon* self_;

};

#endif // CUSTOMSYSTEMTRAYICON_H
