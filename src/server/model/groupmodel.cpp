#include "groupmodel.hpp"

// 创建群组
bool GroupModel::createGroup(ConnectionPool &cp, Group &group){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into AllGroup(groupname, groupdesc) values('%s', '%s')", group.getName().c_str(), group.getDesc().c_str());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->update(sql)){
            group.setId(mysql->getConnId());
            return true;
    }
    return false;
}

// 加入群组
void GroupModel::addGroup(ConnectionPool &cp, int userid, int groupid, string role){
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into GroupUser values(%d, %d, '%s')", groupid, userid, role.c_str());
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    mysql->update(sql);
}

// 查询用户所在群组信息
vector<Group> GroupModel::queryGroups(ConnectionPool &cp, int userid){
    // 先根据userid在groupuser表中查询出该用户所属的群组信息
    // 再根据群组信息，查询属于该群组的所有用户的userid，并且和user表进行多表联合查询，查出用户的详细信息
    char sql[1024] = {0};
    sprintf(sql, "select * from AllGroup a inner join GroupUser b on a.id = b.groupid where b.userid = %d", userid);
    vector<Group> groupVec;
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->query(sql)){
        // 查出userid所有的群组信息
        while(mysql->next()){
            MYSQL_ROW row = mysql->getRow();
            if(row != nullptr){
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);
            }
        }
    }
    // 查询群组的用户信息
    for(Group &group : groupVec){
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from User a inner join GroupUser b on b.userid = a.id where b.groupid = %d", group.getId());
        if(mysql->query(sql)){
            while(mysql->next()){
                MYSQL_ROW row = mysql->getRow();
                if(row != nullptr){
                    GroupUser user;
                    user.setId(atoi(row[0]));
                    user.setName(row[1]);
                    user.setState(row[2]);
                    user.setRole(row[3]);
                    group.getUsers().push_back(user);
                }
            }
        }
    }
    return groupVec;
}

// 根据指定的groupid查询群组用户id列表，除userid自己，主要用于群聊业务给群组其它成员群发消息
vector<int> GroupModel::queryGroupUsers(ConnectionPool &cp, int userid, int groupid){
    char sql[1024] = {0};
    sprintf(sql, "select userid from GroupUser where groupid = %d and userid <> %d", groupid, userid);
    vector<int> idVec;
    shared_ptr<MysqlConn> mysql = cp.getConnection();
    if(mysql->query(sql)){
        while(mysql->next()){
            MYSQL_ROW row = mysql->getRow();
            if(row != nullptr){
                idVec.push_back(atoi(row[0]));
            }
        }
    }
    return idVec;
}