#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "serverthread.h"
#include "ServerForm.h"
#include"protocol.h"
#include <QtNetwork>
#include"mysql.h"

class MySql;
class ServerForm;

extern MySql mysql;
MySql fun();


class Server: public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    QList<ClientInfo> socketUserList;
public:
    QList<int> socketList;//建立一个list存连接服务器的客户端id
    int count=0;



protected:
    void incomingConnection(qintptr socketDescriptor);//重写此虚函数，每次有客户端来连接时触发该虚函数


private slots:
    void showConnection();
    void showDisConnection(int socketDescriptor);

private:
    ServerForm *serverForm;
};

#endif // SERVER_H
