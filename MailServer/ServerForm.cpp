#include "ServerForm.h"
#include "ui_ServerForm.h"
#include "mysql.h"
#include<QDebug>
#include<QAbstractSocket>
#include<QMessageBox>
#include<protocol.h>
#include<QByteArray>

ServerForm::ServerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerForm), count(0)
{
    ui->setupUi(this);
    ui->num->setText(QString("%1").arg(count));
    server = new Server(this);

   // server->listen(QHostAddress::Any, 8000);
    qDebug()<<"listen";

}

ServerForm::~ServerForm()
{
    delete ui;

}


//void ServerForm::showConnection()
//{
//    count++;

//    qDebug()<<"count:"<<count;

//    /* add socket object that join in */
//    //增加客户端
//    ui->objectBox->addItem(QString("%1").arg(server->socketList.last()));

//    /* change connect number while connection is connecting */
//    //改变连接的数量
//    ui->num->setText(QString("%1").arg(count));
//}
//void ServerForm::showDisconnection(int socketDescriptor)
//{
//    count--;

//    /* remove disconnect socketdescriptor from list */
//    server->socketList.removeAll(socketDescriptor);
//    for(int i=0;i<socketUserList.size();i++){
//        if(socketUserList.at(i).socketDescriptor==socketDescriptor){
//            socketUserList.removeAt(i);
//        }
//    }

//    /* reload combobox */
//    ui->objectBox->clear();
//    //后期可省略
//    for (int i = 0; i < server->socketList.size(); i++) {
//         ui->objectBox->addItem(QString("%1").arg(server->socketList.at(i)));
//    }

//    //change connect number while connection is disconnecting
//    ui->num->setText(QString("%1").arg(count));
//}





//void ServerForm::SendMessageToClientForSignUp(QString dataInfo){//a
//    QStringList list=dataInfo.split("#");
//    QString sendData="a";
//    if(checkSignUp(list[1],list[2])){
//       sendData+="#true";
//    }
//    else{
//        sendData+="#false";
//    }

//}
//void ServerForm::SendMessageToClientForSignIn(QString dataInfo){

//}
//void ServerForm::clearMsg()
//{
//    ui->msg->clear();
//}
