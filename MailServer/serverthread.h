#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QObject>
#include <QtNetwork>
#include <QDebug>

#include "mysocket.h"


class MySocket;

class serverThread : public QThread//继承于QThread的类
{
    Q_OBJECT
public:
    serverThread(int socketDes, QObject *parent = 0);
    ~serverThread();
    void run();
public:
    int socketDescriptor;
signals:

    void disconnectTCP(int );

private slots:
    void disconnectToHost();

private:
    MySocket *socket;

};

#endif // SERVERTHREAD_H
