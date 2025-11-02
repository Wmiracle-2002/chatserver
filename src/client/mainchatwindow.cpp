#include "mainchatwindow.h"
#include "ui_mainchatwindow.h"
#include <ctime>
#include <chrono>
#include <iostream>
#include <QMenu>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include "modifywidget.h"

// 获取系统时间（聊天信息需要添加时间信息）
string getCurrentTime(){
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm *ptm = localtime(&tt);
    char date[60] = {0};
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
    return std::string(date);
}

MainChatWindow::MainChatWindow(ClientCore* clientCore, QWidget *parent)
    : QMainWindow(parent)
    , m_clientCore(clientCore)
    , m_pm(new PictureManager(m_clientCore))
    , ui(new Ui::MainChatWindow)
{
    ui->setupUi(this);
    // 设置上下文菜单策略
    ui->friendWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->groupWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->pictureLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    // 连接自定义上下文菜单请求信号到槽函数
    connect(ui->friendWidget, &QListWidget::customContextMenuRequested, this, &MainChatWindow::onFriendWidgetContextMenuRequested);
    connect(ui->groupWidget, &QListWidget::customContextMenuRequested, this, &MainChatWindow::onGroupWidgetContextMenuRequested);
    connect(ui->pictureLabel, &QLabel::customContextMenuRequested, this, &MainChatWindow::onPictureLabelContextMenuRequested);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainChatWindow::transferText);
    connect(ui->friendWidget, &QListWidget::itemClicked, this, &MainChatWindow::onFriendClicked);
    connect(ui->groupWidget, &QListWidget::itemClicked, this, &MainChatWindow::onGroupClicked);
    connect(clientCore, &ClientCore::recvOfflineMsg, this, &MainChatWindow::onOfflineMsg);
    connect(clientCore, &ClientCore::recvOneChatMsg, this, &MainChatWindow::appendOneChatMsg);
    connect(clientCore, &ClientCore::recvGroupChatMsg, this, &MainChatWindow::appendGroupChatMsg);
    connect(clientCore, &ClientCore::groupDissolved, this, &MainChatWindow::onGroupDissolved);
    connect(clientCore, &ClientCore::friendDeleted, this, &MainChatWindow::onFriendDeleted);
    connect(clientCore, &ClientCore::friendAdded, this, &MainChatWindow::onFriendAdded);
    connect(clientCore, &ClientCore::groupAdded, this, &MainChatWindow::onGroupAdded);
    connect(clientCore, &ClientCore::friendInfo, this, &MainChatWindow::showInfo);
    connect(clientCore, &ClientCore::groupInfo, this, &MainChatWindow::showInfo);
    connect(clientCore, &ClientCore::refreshUserInfo, this, &MainChatWindow::showUserInfo);
    connect(clientCore, &ClientCore::pictureChanged, this, &MainChatWindow::onServerNotifyPictureChanged);

    connect(m_pm, &PictureManager::pictureDownloaded, this, &MainChatWindow::onPictureDownloaded);

    ui->sendButton->setDefault(true);

    QString id = QString::number(m_clientCore->g_currentUser.getId());
    // 删除该用户的头像缓存
    m_pm->deleteCachedPicture(id);
    // 立即重新下载新头像
    m_pm->downloadPicture(id);
    for(auto user : m_clientCore->g_currentUserFriendList){
        QString userid = QString::number(user.getId());
        // 删除该用户的头像缓存
        m_pm->deleteCachedPicture(userid);
        // 立即重新下载新头像
        m_pm->downloadPicture(userid);
    }
}

MainChatWindow::~MainChatWindow()
{
    delete ui;
}

void MainChatWindow::addCurrentFriend()
{
    m_friendList.clear();
    for(User &f : m_clientCore->g_currentUserFriendList)
    {
        QString id = QString::number(f.getId());
        QString name = QString::fromStdString(f.getName());
        m_friendList.insert(id, name);
    }
}

void MainChatWindow::addCurrentGroup()
{
    m_groupList.clear();
    for(Group &g : m_clientCore->g_currentUserGroupList)
    {
        QString groupid = QString::number(g.getId());
        QString groupname = QString::fromStdString(g.getName());
        m_groupList.insert(groupid, groupname);
    }
}

void MainChatWindow::on_sendButton_clicked()
{
    transferText();
}

void MainChatWindow::on_addfriendButton_clicked()
{
    QString raw = ui->addfriendEdit->text();
    if (raw.isEmpty()) return;
    addfriend(raw);
    ui->addfriendEdit->clear();
}

void MainChatWindow::on_addgroupButton_clicked()
{
    QString raw = ui->addgroupEdit->text();
    if (raw.isEmpty()) return;
    addgroup(raw);
    ui->addgroupEdit->clear();
}

void MainChatWindow::on_creategroupButton_clicked()
{
    QString groupname = ui->creategroupEdit->text();
    if (groupname.isEmpty()) return;
    // QString groupdesc = str.substr(idx + 1, str.size() - idx);
    QString groupdesc = "";
    creategroup(groupname, groupdesc);
    ui->creategroupEdit->clear();
    fillGroupList();
}

void MainChatWindow::on_tabWidget_currentChanged(int index)
{
    (m_index = index) == 0 ? fillFriendList() : fillGroupList();
}

void MainChatWindow::onServerNotifyPictureChanged(const json &js)
{
    int id = js["id"];
    qDebug() << "收到服务器头像变更通知，用户ID:" << id;
    
    // 删除该用户的头像缓存
    m_pm->deleteCachedPicture(QString::number(id));
    // 立即重新下载新头像
    m_pm->downloadPicture(QString::number(id));
}

void MainChatWindow::onFriendWidgetContextMenuRequested(const QPoint &pos)
{
    // 获取右键点击处的item
    QListWidgetItem *item = ui->friendWidget->itemAt(pos);
    // 如果没有点击在item上，则不显示菜单（可选）
    if (!item) return;

    // 创建菜单
    QMenu menu(this);
    QAction *showFriendInfoAction = menu.addAction("friend information");
    QAction *deleteAction = menu.addAction("delete friend");
    
    // 显示菜单，并获取用户选择的动作
    // 注意需要将控件内的坐标转换为全局坐标
    QAction *selectedAction = menu.exec(ui->friendWidget->mapToGlobal(pos));
    
    // 如果用户选择"查看好友信息"
    if (selectedAction == showFriendInfoAction) {
        QString friendid = item->data(Qt::UserRole).toString();
        QString friendname = item->text();
        
        // 从本地数据中获取好友信息
        QString info = QString("好友ID: %1\n好友名称: %2\n")
                        .arg(friendid)
                        .arg(friendname);
        
        QMessageBox::information(this, "好友信息", info);
    }
    // 如果用户选择了"删除好友"
    else if (selectedAction == deleteAction) {
        // 获取该item存储的好友ID
        QString friendid = item->data(Qt::UserRole).toString();
        // 调用删除好友的函数
        removefriend(friendid);
        // 可选：立即从列表中移除item，避免等待服务器响应
        // 注意：如果服务器删除失败，需要考虑如何回滚
        delete ui->friendWidget->takeItem(ui->friendWidget->row(item));
    }
}

void MainChatWindow::onGroupWidgetContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->groupWidget->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QString userRole = "unknown";

    QString groupid = item->data(Qt::UserRole).toString();
    for(vector<Group>::iterator it = m_clientCore->g_currentUserGroupList.begin(); it != m_clientCore->g_currentUserGroupList.end(); it++){
        if(it->getId() == groupid.toInt()){
            vector<GroupUser> Users = it->getUsers();
            for(vector<GroupUser>::iterator it2 = Users.begin(); it2 != Users.end(); it2++){
                if(it2->getId() == m_clientCore->g_currentUser.getId()){
                    userRole = QString::fromStdString(it2->getRole());
                    break;
                }
            }
            break;
        }
    }

    QAction *deleteAction = (userRole == "creator") ? menu.addAction("dissolve group") : menu.addAction("quit group");
    QAction *showGroupInfoAction = menu.addAction("group infomation");
    QAction *selectedAction = menu.exec(ui->groupWidget->mapToGlobal(pos));
    
    if (selectedAction == showGroupInfoAction) {
        QString groupid = item->data(Qt::UserRole).toString();
        QString groupname = item->text();

        QString info = QString("群组ID: %1\n群组名称: %2\n")
                        .arg(groupid)
                        .arg(groupname);
        
        QMessageBox::information(this, "群组信息", info);
    }
    else if (selectedAction == deleteAction) {
        quitgroup(groupid);
        delete ui->groupWidget->takeItem(ui->groupWidget->row(item));
    }
}

void MainChatWindow::onPictureLabelContextMenuRequested(const QPoint &pos)
{
    // 创建菜单
    QMenu menu(this);
    QAction *modifyAction = menu.addAction("modify information");
    
    // 显示菜单，并获取用户选择的动作
    // 注意需要将控件内的坐标转换为全局坐标
    QAction *selectedAction = menu.exec(ui->pictureLabel->mapToGlobal(pos));
    if(selectedAction == modifyAction){
        ModifyWidget* modifyWidget = new ModifyWidget(m_clientCore);
        modifyWidget->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
        modifyWidget->show();
    }
}

void MainChatWindow::onGroupDissolved(int groupid)
{
    QString groupid_str = QString::number(groupid);
    
    // 1. 先检查并处理当前选中项（在清空列表之前）
    bool wasViewingThisGroup = false;
    if (m_item) {
        QVariant data = m_item->data(Qt::UserRole);
        if (data.isValid() && data.toString() == groupid_str) {
            wasViewingThisGroup = true;
            // 先显示提示信息
            ui->textEdit->append("This group has already dissolved.");
        }
        // 无论是否匹配，都要重置 m_item，因为列表即将被清空
        m_item = nullptr;
    }
    
    // 2. 从数据中移除
    m_groupList.remove(groupid_str);
    m_groupHistory.remove(groupid_str);
    
    // 3. 刷新列表（这会清空所有项）
    fillGroupList();
}

void MainChatWindow::onFriendDeleted(int friendid)
{
    QString friendid_str = QString::number(friendid);

    // 1. 先检查并处理当前选中项（在清空列表之前）
    bool wasViewingThisFriend = false;
    if (m_item) {
        QVariant data = m_item->data(Qt::UserRole);
        if (data.isValid() && data.toString() == friendid_str) {
            wasViewingThisFriend = true;
            // 先显示提示信息
            ui->textEdit->append("This friend has already deleted.");
        }
        // 无论是否匹配，都要重置 m_item，因为列表即将被清空
        m_item = nullptr;
    }

    // 2. 从数据中移除
    m_friendList.remove(friendid_str);
    m_friendHistory.remove(friendid_str);
    
    // 3. 刷新列表（这会清空所有项）
    fillFriendList();
}

void MainChatWindow::onFriendAdded(int id, string name)
{
    qDebug() << "有好友修改了信息";
    // 好友信息修改后，这里进行更新（即移除原信息，添加新信息）
    QMetaObject::invokeMethod(this, [this, id, name]() {
        // 从本地好友列表中移除
        auto& friends = m_clientCore->g_currentUserFriendList;
        friends.erase(std::remove_if(friends.begin(), friends.end(), [id](User user) {return user.getId() == id;}), friends.end());
        // 更改后的好友添加到本地好友列表
        User user;
        user.setId(id);
        user.setName(name);
        m_clientCore->g_currentUserFriendList.push_back(user);
    }, Qt::QueuedConnection);
    // 刷新列表
    fillFriendList();
}

void MainChatWindow::onGroupAdded(int id, string name)
{
    // 1. 从数据中添加
    m_groupList.insert(QString::number(id), QString::fromStdString(name));
    // 2. 刷新列表
    fillGroupList();
}

void MainChatWindow::onFriendClicked(QListWidgetItem *item)
{
    m_item = item;
    QString uid = item->data(Qt::UserRole).toString();
    loadConversation(uid);
}

void MainChatWindow::onGroupClicked(QListWidgetItem *item)
{
    m_item = item;
    QString gid = item->data(Qt::UserRole).toString();
    loadConversation(gid);
}

void MainChatWindow::onPictureDownloaded(const QString& userid, const QPixmap& picture)
{
    // 更新当前用户头像
    QString currentUserId = QString::number(m_clientCore->g_currentUser.getId());
    if (userid == currentUserId && !picture.isNull()) {
        m_pm->showPicture(picture, ui->pictureLabel);
    }
    // 更新好友列表头像
    for (int i = 0; i < ui->friendWidget->count(); i++) {
        QListWidgetItem *item = ui->friendWidget->item(i);
        QString itemUserId = item->data(Qt::UserRole).toString();

        if (itemUserId == userid) {
            m_pm->showPicture(picture, item);
            break;
        }
    }
}

void MainChatWindow::showUserInfo()
{
    int id = m_clientCore->g_currentUser.getId();
    string name = m_clientCore->g_currentUser.getName();
    QString showid = QString::fromStdString("账号：") + QString::number(id);
    QString showname = QString::fromStdString("昵称：" + name);
    // 加载原图
    m_pm->downloadPicture(QString::number(id));
    ui->idLabel->setText(showid);
    ui->nameLabel->setText(showname);
}

void MainChatWindow::transferText()
{
    QString raw = ui->lineEdit->text();
    if (raw.isEmpty()) return;
    QString time = QString::fromStdString(getCurrentTime());
    QString username = QString::fromStdString(m_clientCore->g_currentUser.getName());
    QString sendText = ui->lineEdit->text();
    ui->textEdit->append("[" + time + "] " + username + " : " + sendText);
    ui->lineEdit->clear();
    if(m_index == 0){
        chat(time, sendText);
        m_friendHistory[QString::number(m_item->data(Qt::UserRole).toInt())].append({time, username, sendText});
    }
    else{
        groupchat(time, sendText);
        m_groupHistory[QString::number(m_item->data(Qt::UserRole).toInt())].append({time, username, sendText});
    }
}

void MainChatWindow::onOfflineMsg(QString jsStr)
{
    // qDebug() << "[UI] onOfflineMsg triggered with" << jsStr;
    json js = json::parse(jsStr.toStdString());
    int msgid = js["msgid"].get<int>();

    if (msgid == ONE_CHAT_MSG) {          // 私聊
        QString time = QString::fromStdString(js["time"].get<string>());
        QString id = QString::number(js["id"].get<int>());
        QString name = QString::fromStdString(js["name"].get<string>());
        QString msg = QString::fromStdString(js["msg"].get<string>());
        m_friendHistory[id].append({time, name, msg});
    } else {                              // 群聊
        QString time = QString::fromStdString(js["time"].get<string>());
        QString groupid = QString::number(js["groupid"].get<int>());
        QString id = QString::number(js["id"].get<int>());
        QString name = QString::fromStdString(js["name"].get<string>());
        QString msg = QString::fromStdString(js["msg"].get<string>());
        m_groupHistory[id].append({time, name, msg});
    }
}

void MainChatWindow::appendOneChatMsg(const json &js)
{
    QString time = QString::fromStdString(js["time"].get<string>());
    QString id = QString::number(js["id"].get<int>());
    QString name = QString::fromStdString(js["name"].get<string>());
    QString msg = QString::fromStdString(js["msg"].get<string>());
    // ui->textEdit->append(QString("[%1] %2: %3").arg(time, name, msg));
    if (m_item && m_item->data(Qt::UserRole).toString() == id) {
        ui->textEdit->append("[" + time + "] " + name + " : " + msg);
    }
    m_friendHistory[id].append({time, name, msg});
}

void MainChatWindow::appendGroupChatMsg(const json &js)
{
    QString time = QString::fromStdString(js["time"].get<string>());
    QString groupid = QString::number(js["groupid"].get<int>());
    QString id = QString::number(js["id"].get<int>());
    QString name = QString::fromStdString(js["name"].get<string>());
    QString msg = QString::fromStdString(js["msg"].get<string>());
    // ui->textEdit->append(QString("[%1] %2: %3").arg(time, name, msg));
    if (m_item && m_item->data(Qt::UserRole).toString() == groupid) {
        ui->textEdit->append("[" + time + "] " + name + " : " + msg);
    }
    m_groupHistory[id].append({time, name, msg});
}

void MainChatWindow::fillFriendList()
{
    ui->friendWidget->clear();
    addCurrentFriend();
    for (QMap<QString, QString>::iterator it = m_friendList.begin(); it != m_friendList.end(); ++it) {
        // 1. 创建 item
        QListWidgetItem *item = new QListWidgetItem(ui->friendWidget);
    
        // 2. 设置信息
        item->setText(it.value());
        item->setData(Qt::UserRole, it.key());   // 后面点 item 能取到 uid
        item->setSizeHint(QSize(0, 48));
        for(User user : m_clientCore->g_currentUserFriendList){ // 获取好友头像
            if(QString::number(user.getId()) == it.key()){
                m_pm->downloadPicture(it.key());
                break;
            }
        }
    }
}

void MainChatWindow::fillGroupList()
{
    ui->groupWidget->clear();
    addCurrentGroup();
    for (QMap<QString, QString>::iterator it = m_groupList.begin(); it != m_groupList.end(); ++it) {
        // 1. 创建 item
        QListWidgetItem *item = new QListWidgetItem(ui->groupWidget);
        item->setText(it.value());
        item->setData(Qt::UserRole, it.key());   // 后面点 item 能取到 uid
        // 2. 大小
        item->setSizeHint(QSize(0, 48));      // 固定 48 px 高
    }
}

void MainChatWindow::loadConversation(QString id)
{
    if(m_index == 0)
    {
        // 1. 从数据库/缓存/map 里拿到这个好友的完整数据
        QString uid = id;
        QString name = m_friendList[uid];
        QVector<Message> msgs = m_friendHistory[uid]; // 消息记录

        // 2. 把聊天历史刷到右边的 QTextEdit/QListWidget
        ui->textEdit->clear();
        for (const Message &m : msgs)
            ui->textEdit->append("[" + m.time + "] " + m.name + " : " + m.msg);
    }
    else
    {
        QString gid = id;
        QString gname = m_groupList[gid];
        QVector<Message> msgs = m_groupHistory[gid]; // 消息记录

        ui->textEdit->clear();
        for (const Message &m : msgs)
            ui->textEdit->append("[" + m.time + "] " + m.name + " : " + m.msg);
    }
}

void MainChatWindow::showInfo(const QString& type, const QString& title, const QString& msg)
{
    if(type == "warning"){
        QMessageBox::warning(this, title, msg);
    }
    else{
        QMessageBox::information(this, title, msg);
    }
}

// 一对一聊天命令
void MainChatWindow::chat(QString time, QString msg){
    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["name"] = m_clientCore->g_currentUser.getName();
    js["toid"] = m_item->data(Qt::UserRole).toInt();
    js["msg"] = msg.toStdString();
    js["time"] = time.toStdString();
    m_clientCore->sendJson(js);
}

// 添加好友命令
void MainChatWindow::addfriend(QString uid){
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["friendid"] = uid.toInt();
    m_clientCore->sendJson(js);
}

// 删除好友命令
void MainChatWindow::removefriend(QString uid){
    json js;
    js["msgid"] = REMOVE_FRIEND_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["friendid"] = uid.toInt();
    m_clientCore->sendJson(js);
}

// 创建群聊命令
void MainChatWindow::creategroup(QString groupname, QString groupdesc){
    json js;
    js["msgid"] = CREATE_GROUP_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["groupname"] = groupname.toStdString();
    js["groupdesc"] = groupdesc.toStdString();
    // js["groupdesc"] = groupdesc;
    m_clientCore->sendJson(js);
}

// 加入群聊命令
void MainChatWindow::addgroup(QString gid){
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["groupid"] = gid.toInt();
    m_clientCore->sendJson(js);
}

// 退出群聊命令
void MainChatWindow::quitgroup(QString gid){
    json js;
    js["msgid"] = QUIT_GROUP_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["groupid"] = gid.toInt();
    m_clientCore->sendJson(js);
}

// 群组聊天命令
void MainChatWindow::groupchat(QString time, QString msg){
    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    js["name"] = m_clientCore->g_currentUser.getName();
    js["groupid"] = m_item->data(Qt::UserRole).toInt();
    js["msg"] = msg.toStdString();
    js["time"] = time.toStdString();
    m_clientCore->sendJson(js);
}

// 注销命令
void MainChatWindow::logout(){
    json js;
    js["msgid"] = LOGOUT_MSG;
    js["id"] = m_clientCore->g_currentUser.getId();
    m_clientCore->sendJson(js);
    m_clientCore->isMainMenuRunning = false;
}
