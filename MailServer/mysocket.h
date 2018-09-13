#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>
#include"server.h"

class MySql;

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(int socket, QObject *parent = 0);

signals:
    void signRevData(QByteArray,int);
public slots:
    void slotRevData();
    void sendMsg(char* msg, int len,int id);
private:
    int socketDescriptor;
    QString userName;
    QFile file; //文件对象
    QString fileName; //文件名字
    int fileSize; //文件大小
    int recvSize; //已经接收文件的大小

};

#endif // MYSOCKET_H
