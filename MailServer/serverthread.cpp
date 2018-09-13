#include "serverthread.h"

serverThread::serverThread(int socketDescriptor, QObject *parent) :
    QThread(parent), socketDescriptor(socketDescriptor)
{

}

serverThread::~serverThread()
{
    socket->close();
}

void serverThread::run()
{
    socket = new MySocket(socketDescriptor, 0);


    //重写incomingConnection (qintptr socketDescriptor) 函数 ,
    //再有连接时这个函数会被系统调用，socketDescriptor  为套接字描述符，
    //用这个描述符创建一个socket就可以了，即setSocketDescriptor  这个函数，
    //描述符传给这个函数参数即可
    if (!socket->setSocketDescriptor(socketDescriptor))
        //如果传入的套接字描述符不合法，退出run函数
        return ;
    connect(socket,SIGNAL(readyRead()),socket,SLOT(slotRevData()));


    //当客户端断开连接时，引发disconnecttoHost的槽函数，这个槽函数里会发送一个信号，最后被主机知道
    connect(socket, &MySocket::disconnected, this, &serverThread::disconnectToHost);
    //要启动事件循环，必须在run()内调用exec()
    exec();
}


void serverThread::disconnectToHost()
{
    emit disconnectTCP(socketDescriptor);
    socket->disconnectFromHost();
}
