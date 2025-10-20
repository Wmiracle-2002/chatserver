#ifndef MAINCHATWINDOW_H
#define MAINCHATWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "clientcore.h"

// 消息结构
struct Message {
    QString time;
    QString name;
    QString msg;
};

namespace Ui {
class MainChatWindow;
}

class MainChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainChatWindow(ClientCore* clientCore, QWidget *parent = nullptr);
    ~MainChatWindow();
    void addCurrentFriend();
    void addCurrentGroup();
    void fillFriendList();
    void fillGroupList();
    void loadConversation(QString id);
    void showInfo(const QString& type, const QString& title, const QString& msg);
    void chat(QString time, QString msg);
    void groupchat(QString time, QString msg);
    void addfriend(QString uid);
    void removefriend(QString uid);
    void addgroup(QString gid);
    void quitgroup(QString gid);
    void creategroup(QString groupname, QString groupdesc);
    void logout();
private slots:
    void on_sendButton_clicked();
    void on_addfriendButton_clicked();
    void on_addgroupButton_clicked();
    void on_creategroupButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void onFriendWidgetContextMenuRequested(const QPoint &pos);
    void onGroupWidgetContextMenuRequested(const QPoint &pos);
    void onGroupDissolved(int groupid);
    void onFriendDeleted(int friendid);
    void onFriendAdded(int id, string name);
    void onGroupAdded(int id, string name);
    void transferText();
    void onFriendClicked(QListWidgetItem *item);
    void onGroupClicked(QListWidgetItem *item);
    void onOfflineMsg(QString jsStr);
    void appendOneChatMsg(const json &js);
    void appendGroupChatMsg(const json &js);
private:
    int m_index;    // 区分单聊/群聊页面
    Ui::MainChatWindow *ui;
    ClientCore* m_clientCore; // 添加核心网络对象指针
    QListWidgetItem* m_item;
    QMap<QString, QString> m_friendList;
    QMap<QString, QString> m_groupList;
    QMap<QString, QVector<Message>> m_friendHistory;
    QMap<QString, QVector<Message>> m_groupHistory;
};

#endif // MAINCHATWINDOW_H
