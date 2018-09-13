#include "mysql.h"
#include<QSqlError>
#include<QSqlQuery>//专门对Sql语句操作的类
#include<QDebug>
#include"protocol.h"
#include<QTcpSocket>

MySql::MySql()
{

}
void MySql::initsql()
{

    //添加Myql数据库
    db=QSqlDatabase::addDatabase("QMYSQL");
    //连接数据库
    db.setHostName("127.0.0.1");
    //数据库用户名
    db.setUserName("root");
    //数据库密码
    db.setPassword("123456");
    //使用的数据库名称,使用哪个数据库
    db.setDatabaseName("MailData");

    //打开数据库
    if(db.open())
        {
            qDebug()<<"Database connected successfully!";
            //createtable();
            query=new QSqlQuery(db);
            return;
        }
    else//数据库打开失败
        {
            qDebug()<<"Database connected failed!";
            return;
        }

}

bool MySql::loguser(QString name, QString passward)//登录
{
    QString str=QString("select * from user where name='%1' and password='%2'").arg(name).arg(passward);
    query->exec(str);//在user数据库中查找用户名为name的人
    query->last();

   // qDebug()<<query->value("name").toString();

    int record=query->at()+1;
    if(record==0)
        return false;
    return true;
}


bool MySql::signup(QString name,QString passward,QString phone)//注册
{

    QString str=QString("select * from user where name='%1'n").arg(name);
    qDebug()<<str;
    query->exec(str);
    int num=query->size();
    if(num==0){

    str=QString("insert into user value('%1','%2','%3')").arg(name).arg(passward).arg(phone);

    qDebug()<<str;

    bool ret=query->exec(str);
    return ret;
    }
    else{
        return false;
    }
}
bool MySql::findPass(QString name,QString phone){
    QString str=QString("select * from user where name='%1' and phone='%2'").arg(name).arg(phone);
    query->exec(str);//在user数据库中查找用户名为name的人
    query->last();

    int record=query->at()+1;
    if(record==0)
        return false;
    return true;
}
bool MySql::checkSignIn(QString name,QString passward)//登录
{
    bool ret=loguser(name,passward);//放到数据库中
    return ret;
}
bool MySql::checkSignUp(QString name, QString passward,QString phone)//注册
{
    qDebug()<<name<<passward<<phone;
    bool ret=signup(name,passward,phone);
    return ret;
}
bool MySql::checkFindPass(QString name,QString phone){
    bool ret=findPass(name,phone);
    return ret;
}
bool MySql::checkResetPass(QString name,QString password){
    qDebug()<<"name:"<<name;
    QString str=QString("update user set password='%1' where name='%2'").arg(password).arg(name);
    return query->exec(str);
}
//senderFlag=1表示发送，=0表示存到草稿箱
bool MySql::checkSendMail(QString sender,QString recipient,QString title,QString content,
                          int attachmentNum,QString time,char senderFlag){

    qDebug()<<"checkSendMail";

    QString str=QString("select * from user where name='%1'").arg(recipient);

    query->exec(str);
    //qDebug()<<str+'\n'+"size:"<<query->size();

    if(query->size()<=0){
        qDebug()<<"the recipient isn't exist!";
        return false;
    }

    QString str2=QString("insert into Message(sender,recipient,title,content,attachmentNum,time,senderFlag) "
                        "values('%1','%2','%3','%4','%5','%6','%7')")
            .arg(sender).arg(recipient).arg(title).arg(content).arg(attachmentNum).arg(time).arg(senderFlag);
    if(query->exec(str2)){
        qDebug()<<"insert successful";
        return true;
    }else{
        qDebug()<<"insert fail";
        return false;
    }
}
int MySql::checkMailNum(QString userName){
    QString str=QString("select * from Message where recipient='%1' and senderFlag=0").arg(userName);
    qDebug()<<"checkMailNum:"<<str;
    query->exec(str);
    return query->size();
}
int MySql::checkSendMailNum(QString userName){
    QString str=QString("select * from Message where sender='%1' and senderFlag=0").arg(userName);
    qDebug()<<"checkMailNum:"<<str;
    query->exec(str);
    return query->size();
}

simpleMail_package* MySql::sendSimpleMail(QString userName){
    QString str=QString("select * from Message where recipient='%1' and senderFlag=0").arg(userName);
    qDebug()<<"sendSimpleMail:"<<str;
    query->exec(str);
    int num=query->size();
    qDebug()<<"num:"<<num;

    simpleMail_package *simpleMailPackage;
    simpleMailPackage=(simpleMail_package *)malloc(sizeof(simpleMail_package)*num);
    int i=0;
    while(query->next()){
       // strncpy(simpleMailPackage[i].sender,query->value("sender"))
        strncpy(simpleMailPackage[i].sender,query->value("sender").toString().toLatin1().data(),20);
       // qDebug()<<"simpleMailPackage[i].sender:"<<simpleMailPackage[i].sender;

        //simpleMailPackage[i].title=query->value("title").toString();
        strncpy(simpleMailPackage[i].title,query->value("title").toString().toLatin1().data(),30);
       // qDebug()<<"simpleMailPackage[i].title:"<<simpleMailPackage[i].title;

        //simpleMailPackage[i].time=query->value("time");
        strncpy(simpleMailPackage[i].time,query->value("time").toString().toLatin1().data(),20);
        //qDebug()<<"simpleMailPackage[i].time:"<<simpleMailPackage[i].time;

        simpleMailPackage[i].appendixCount=query->value("attachmentNum").toInt();
        //qDebug()<<"simpleMailPackage[i].attachmentNum:"<<simpleMailPackage[i].appendixCount;

        simpleMailPackage[i].recvFlag=query->value("recpFlag1").toInt();
        //qDebug()<<"simpleMailPackage[i].recvFlag"<<simpleMailPackage[i].recvFlag;
        //toAscii(1);
        i++;
    }
    return simpleMailPackage;

}
//已发送的邮件简单信息
simpleMail_package* MySql::sendSimpleMail2(QString userName){
    QString str=QString("select * from Message where sender='%1' and senderFlag=0").arg(userName);
    qDebug()<<"sendSimpleMail:"<<str;
    query->exec(str);
    int num=query->size();
    qDebug()<<"num:"<<num;

    simpleMail_package *simpleMailPackage;
    simpleMailPackage=(simpleMail_package *)malloc(sizeof(simpleMail_package)*num);
    int i=0;
    while(query->next()){
       // strncpy(simpleMailPackage[i].sender,query->value("sender"))
        strncpy(simpleMailPackage[i].sender,query->value("sender").toString().toLatin1().data(),20);
      //  qDebug()<<"simpleMailPackage[i].sender:"<<simpleMailPackage[i].sender;

        //simpleMailPackage[i].title=query->value("title").toString();
        strncpy(simpleMailPackage[i].title,query->value("title").toString().toLatin1().data(),30);
      //  qDebug()<<"simpleMailPackage[i].title:"<<simpleMailPackage[i].title;

        //simpleMailPackage[i].time=query->value("time");
        strncpy(simpleMailPackage[i].time,query->value("time").toString().toLatin1().data(),20);
      //  qDebug()<<"simpleMailPackage[i].time:"<<simpleMailPackage[i].time;

        simpleMailPackage[i].appendixCount=query->value("attachmentNum").toInt();
      //  qDebug()<<"simpleMailPackage[i].attachmentNum:"<<simpleMailPackage[i].appendixCount;

        simpleMailPackage[i].recvFlag=query->value("recpFlag1").toInt();
       // qDebug()<<"simpleMailPackage[i].recvFlag"<<simpleMailPackage[i].recvFlag;
        //toAscii(1);
        i++;
    }
    return simpleMailPackage;

}

mail_package MySql::sendCheckMail(QString sender,QString time){
    QString str=QString("select * from Message where sender='%1' and time='%2'").arg(sender).arg(time);
    query->exec(str);
    qDebug()<<"str:"<<str<<"\nsize:"<<query->size();

    mail_package mailPackage;
    query->next();

    strncpy(mailPackage.sender,query->value("sender").toString().toLatin1().data(),20);
    //qDebug()<<"mailPackage.sender:"<<mailPackage.sender;

    strncpy(mailPackage.recver,query->value("recipient").toString().toLatin1().data(),20);
   // qDebug()<<"mailPackage.recver:"<<mailPackage.recver;

    strncpy(mailPackage.title,query->value("title").toString().toLatin1().data(),30);
  //  qDebug()<<"mailPackage.title:"<<mailPackage.title;

    strncpy(mailPackage.time,query->value("time").toString().toLatin1().data(),20);
   // qDebug()<<"mailPackage.time:"<<mailPackage.time;

    strncpy(mailPackage.context,query->value("content").toString().toLatin1().data(),100);
   // qDebug()<<"mailPackage.context:"<<mailPackage.context;

    mailPackage.appendixCount=query->value("attachmentNum").toInt();
   // qDebug()<<"mailPackage.appendixCount:"<<mailPackage.appendixCount;
    if(query->value("recpFlag1").toInt()!=1){
        QString str2=QString("update Message set recpFlag1=1 where sender='%1' and time='%2'").arg(sender).arg(time);
        query->exec(str2);
    }

    return mailPackage;

}
int MySql::checkNoReadMail(QString userName){
    QString str=QString("select * from Message where recipient='%1' and recpFlag1=0").arg(userName);
    query->exec(str);
    int num=query->size();
    return num;
}

simpleMail_package* MySql::simpleStoreMail(QString userName){
    QString str=QString("select * from Message where sender='%1' and senderFlag=1").arg(userName);
    qDebug()<<"simpleStoreMail:"<<str;
    query->exec(str);
    int num=query->size();
    qDebug()<<"num:"<<num;
    simpleMail_package *simpleMailPackage;
    simpleMailPackage=(simpleMail_package *)malloc(sizeof(simpleMail_package)*num);
    int i=0;
    while(query->next()){
       // strncpy(simpleMailPackage[i].sender,query->value("sender"))
        strncpy(simpleMailPackage[i].sender,query->value("recipient").toString().toLatin1().data(),20);
      //  qDebug()<<"simpleMailPackage[i].sender:"<<simpleMailPackage[i].sender;

        //simpleMailPackage[i].title=query->value("title").toString();
        strncpy(simpleMailPackage[i].title,query->value("title").toString().toLatin1().data(),30);
      //  qDebug()<<"simpleMailPackage[i].title:"<<simpleMailPackage[i].title;

        //simpleMailPackage[i].time=query->value("time");
        strncpy(simpleMailPackage[i].time,query->value("time").toString().toLatin1().data(),20);
      //  qDebug()<<"simpleMailPackage[i].time:"<<simpleMailPackage[i].time;

        simpleMailPackage[i].appendixCount=query->value("attachmentNum").toInt();
      //  qDebug()<<"simpleMailPackage[i].attachmentNum:"<<simpleMailPackage[i].appendixCount;

        simpleMailPackage[i].recvFlag=query->value("recpFlag1").toInt();
       // qDebug()<<"simpleMailPackage[i].recvFlag"<<simpleMailPackage[i].recvFlag;
        //toAscii(1);
        i++;
    }
    return simpleMailPackage;

}
int MySql::storeMailNum(QString userName){
    QString str=QString("select * from Message where sender='%1' and senderFlag=1").arg(userName);
    query->exec(str);
    int num=query->size();
    return num;
}

void MySql::filePath(QString sender,QString time,QString filePath){
    QString str=QString("update Message set path='%1' where sender='%2' and time='%3'").arg(filePath).arg(sender).arg(time);
    qDebug()<<"filePath-str:"<<str;
    query->exec(str);
    int num=query->numRowsAffected();
    qDebug()<<"filePath-num:"<<num;

}
QStringList MySql::finPath(QString sender,QString time){
    QString str=QString("select path from Message where sender='%1' and time='%2'").arg(sender).arg(time);
    qDebug()<<"finPath-str:"<<str;
    query->exec(str);
    query->next();
    QString filePath=query->value("path").toString();
    QStringList pathList;
    pathList=filePath.split(QRegExp("[#$]"));
    qDebug()<<"pathList:"<<pathList;
    return pathList;
}
