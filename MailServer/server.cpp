#include "server.h"
#include<QDebug>
#include "mysql.h"
#include<QAbstractSocket>
#include<QMessageBox>
#include<protocol.h>
#include<QByteArray>

MySql mysql=fun();
MySql fun(){
    MySql mysql;
    mysql.initsql();
    return mysql;
}

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
    /* get current serverForm object */
   // serverForm = dynamic_cast<ServerForm *>(parent);


}
Server::~Server()
{

}
void Server::incomingConnection(qintptr socketDescriptor)
{
    //每当一个新的客户端连接时，通过标识码socketDescriptor，实现与对应的客户端通信
    socketList.append(socketDescriptor);

    qDebug()<<"link";

    serverThread *thread = new serverThread(socketDescriptor, 0);

    connect(thread, SIGNAL(started()), this, SLOT(showConnection()));//开启一个线程
    connect(thread, SIGNAL(disconnectTCP(int)), this, SLOT(showDisConnection(int)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

}
void Server::showConnection(){
    count++;
    qDebug()<<"count:"<<count;
}
void Server::showDisConnection(int socketDescriptor){
    count--;
    qDebug()<<"count:"<<count;
    socketList.removeAll(socketDescriptor);
    for(int i=0;i<socketUserList.size();i++){
        if(socketUserList.at(i).socketDescriptor==socketDescriptor){
            socketUserList.removeAt(i);
        }
    }

}

