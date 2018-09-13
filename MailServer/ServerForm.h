#ifndef SERVERFORM_H
#define SERVERFORM_H

#include <QWidget>
#include <QtNetwork>
#include "server.h"
#include"protocol.h"

class Server;
namespace Ui {
class ServerForm;
}

class ServerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServerForm(QWidget *parent = 0);
    ~ServerForm();

   // QString dataInfo;




   // void acceptConnection();
   // void receiveData();
    //void clearMsg();
    //void SendMessageToClientForSignUp(QString dataInfo);
   // void SendMessageToClientForSignIn(QString dataInfo);
  //  void sendMsg(char* data,int len);
public slots:
   // void revData(QByteArray data,int socketDescriptor);
signals:
   // void sendData(QByteArray data,int len, int id);
private slots:
   // void showConnection();



private:
    Ui::ServerForm *ui;
//    QTcpSocket *tcpSocket;
    //QTcpServer *tcpServer;
    int count;
    Server *server;
};

#endif // SERVERFORM_H
