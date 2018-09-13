#include "externv.h"
#include<QSqlDatabase>
int ccc;

        QSqlDatabase::addDatabase("QMYSQL");
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
        createtable();
        return;
    }
else//数据库打开失败
    {
        qDebug()<<"Database connected failed!";
        return;
    }
