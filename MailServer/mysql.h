#ifndef MYSQL_H
#define MYSQL_H

#include<QSqlError>
#include<QSqlQuery>//专门对Sql语句操作的类
#include<QDebug>
#include"protocol.h"

class MySql
{
public:
    MySql();
    void initsql();
   // void createtable();
    bool loguser(QString name,QString passward);
    bool signup(QString name,QString passward,QString phone);
    bool findPass(QString name,QString phone);
    bool checkSignIn(QString name,QString passward);//login
    bool checkSignUp(QString name,QString passward,QString phone);//
    bool checkFindPass(QString name,QString phone);
    bool checkResetPass(QString name,QString password);
    bool checkSendMail(QString sender,QString recipient,QString title,QString content,
                       int attachmentNum,QString time,char senderFlag);
    int checkMailNum(QString userName);
    simpleMail_package* sendSimpleMail(QString userName);
    mail_package sendCheckMail(QString sender,QString time);
    int checkNoReadMail(QString userName);
    int checkSendMailNum(QString userName);
    simpleMail_package* sendSimpleMail2(QString userName);
    int storeMailNum(QString userName);
    simpleMail_package* simpleStoreMail(QString userName);
    void filePath(QString sender,QString time,QString filePath);
    QStringList finPath(QString sender,QString time);
private:
    QSqlQuery *query;
    QSqlDatabase db;
};

#endif // MYSQL_H
