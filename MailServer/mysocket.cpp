#include "mysocket.h"
#include"protocol.h"
#include<QDebug>
#include"mysql.h"
#include"server.h"
#include<QDir>

//Socket完成主机与客户端之间的通信,存在于线程中
MySocket::MySocket(int socketDescriptor, QObject *parent)
    : QTcpSocket(parent), socketDescriptor(socketDescriptor)
{
    //当Client向主机发来消息时，触发readyRead()信号，触发slotRevData()槽函数，
   // connect(this, SIGNAL(readyRead()), this, SLOT(slotRevData()));



}

void MySocket::sendMsg(char* msg,int len, int id)//服务器将信息发送给编号为id的客户端
{
    qDebug()<<"Client ID:"<<id;
    if (socketDescriptor==id )
    {
       qDebug()<<"Client ID:"<<id;
        write(msg,len);
    }
}

void MySocket::slotRevData()//这里可以将用户发送的信息存储起来
{

    client_header clientHeader;
    response_header responseHeader;
    findPass_package findPassPackage;
    login_package loginPackage;
    register_package registerPackage;
    newPass_package newPassPackage;
    mail_package mailPackage;
    simpleMail_package *simpleMailPackage;
    infmForMail_package infmForMailPackage;
    appendix_package appendixPackage;

    read((char*)&clientHeader,sizeof(client_header));
    qDebug()<<clientHeader.cmdType<<clientHeader.length;
    qDebug()<<"socketDescriptor:"<<socketDescriptor;
    switch (clientHeader.cmdType) {
    case 0x0B://register
        {

           // qDebug()<<sizeof(register_package);
            read((char*)&registerPackage,sizeof(register_package));
            bool ok= mysql.checkSignUp(registerPackage.username,registerPackage.password,registerPackage.phone);
            //write("1234");
            if(ok){//注册成功

                responseHeader.statu=0x01;//
                responseHeader.length=0;
                qDebug()<<responseHeader.statu;
                qint64 num=write((char*)&responseHeader,sizeof(response_header));
                qDebug()<<num;
                qDebug()<<"successful";
            }
            else{//失败,用户名已存在
                //response_header responseHeader;
                responseHeader.statu=0x00;//
                responseHeader.length=0;
                qint64 num=write((char*)&responseHeader,sizeof(response_header));
                qDebug()<<num;
                qDebug()<<"fail";
            }
        }
        break;
    case 0x00://登录请求
    {

        read((char*)&loginPackage,sizeof(login_package));
        bool ok= mysql.checkSignIn(loginPackage.username,loginPackage.password);
        if(ok){//Login successful
            responseHeader.statu=0x0C;
            responseHeader.length=0;
            write((char*)&responseHeader,sizeof(response_header));
            qDebug()<<"successful";
            userName=loginPackage.username;

        }else{//Login fail
            responseHeader.statu=0x0D;//
            responseHeader.length=0;
            qint64 num=write((char*)&responseHeader,sizeof(response_header));
        }

    }
        break;
    case 0x0C://find password
    {

        read((char*)&findPassPackage,sizeof(findPass_package));
        bool ok=mysql.checkFindPass(findPassPackage.username,findPassPackage.phone);
        if(ok){
            responseHeader.statu=0x0E;
            responseHeader.length=0;
            write((char*)&responseHeader,sizeof(response_header));
        }
        else{
            responseHeader.statu=0x0F;//
            responseHeader.length=0;
            write((char*)&responseHeader,sizeof(response_header));
        }
     }
        break;
    case 0x0D:{//ResetPassword

        read((char*)&newPassPackage,sizeof(newPass_package));
        bool ok=mysql.checkResetPass(findPassPackage.username,newPassPackage.password);
        qDebug()<<"findPassPackage.username:"<<findPassPackage.username;
        if(ok){
            responseHeader.statu=0x10;
            responseHeader.length=0;
            write((char*)&responseHeader,sizeof(response_header));
        }
        else{
            responseHeader.statu=0x11;//
            responseHeader.length=0;
            write((char*)&responseHeader,sizeof(response_header));
        }
    }
        break;
    case 0x02:{//send
            char sendFlag='0';//send
            read((char*)&mailPackage,sizeof(mail_package));
            bool ok;
            qDebug()<<mailPackage.sender<<mailPackage.recver<<mailPackage.title<<mailPackage.context
                   <<mailPackage.appendixCount<<mailPackage.time<<sendFlag;
            ok=mysql.checkSendMail(
                        mailPackage.sender,
                        mailPackage.recver,
                        mailPackage.title,
                        mailPackage.context,
                        mailPackage.appendixCount,
                        mailPackage.time,
                        sendFlag

                        );

            //附件个数为0
            if(mailPackage.appendixCount==0){
                if(ok){
                    responseHeader.statu=0x03;//successful
                    responseHeader.length=0;
                    write((char*)&responseHeader,sizeof(response_header));
                }
                else{
                    responseHeader.statu=0x12;//fail
                    responseHeader.length=0;
                    write((char*)&responseHeader,sizeof(response_header));
                }
            }
            else{

                QString fileTotalPath;
                for(int i=0;i<mailPackage.appendixCount;i++){
                    int count = read((char*)&appendixPackage,sizeof(appendix_package));


                    QString::fromLocal8Bit(mailPackage.sender);
                    QString::fromLocal8Bit(appendixPackage.appendixName);
                    QString::fromLocal8Bit(mailPackage.time);

                    QString filePath=QString("/home/xiaoya/Desktop/MailAppendix/")+QString::fromLocal8Bit(mailPackage.sender)+
                            "#"+QString::fromLocal8Bit(mailPackage.time)+
                            "#"+QString::fromLocal8Bit(appendixPackage.appendixName);
                    qDebug()<<"filePath:"<<filePath;

                    fileTotalPath+=filePath+"$";

                    file.setFileName(filePath);

                    bool isOk=file.open(QIODevice::WriteOnly);
                    recvSize=0;
                    if(isOk){
                        while(recvSize<appendixPackage.appendixLen){
                            char buf[25]={0};
                            int len=read(buf,sizeof(buf));
                            recvSize+=len;
                            int haveRead=file.write(buf);
                            qDebug()<<"buf:"<<buf;
                            qDebug()<<"haveRead:"<<haveRead;
                            qDebug()<<"recvSize:"<<recvSize;
                        }
                        file.close();//这个要加，不然收不到具体内容
                    }

                }
                qDebug()<<"fileTotalPath:"<<fileTotalPath;
                mysql.filePath(mailPackage.sender,mailPackage.time,fileTotalPath);
            }
    }
        break;
    case 0x04:{//store
        char sendFlag='1';//store
        read((char*)&mailPackage,sizeof(mail_package));
        bool ok;
        qDebug()<<mailPackage.sender<<mailPackage.recver<<mailPackage.title<<mailPackage.context
               <<mailPackage.appendixCount<<mailPackage.time<<sendFlag;
        ok=mysql.checkSendMail(
                    mailPackage.sender,
                    mailPackage.recver,
                    mailPackage.title,
                    mailPackage.context,
                    mailPackage.appendixCount,
                    mailPackage.time,
                    sendFlag
                    );
        //附件个数为0
        if(mailPackage.appendixCount==0){
            if(ok){
                responseHeader.statu=0x05;//successful
                responseHeader.length=0;
                write((char*)&responseHeader,sizeof(response_header));
            }
            else{
                responseHeader.statu=0x13;//fail
                responseHeader.length=0;
                write((char*)&responseHeader,sizeof(response_header));
            }
        }
        else{

        }
    }
        break;
    case 0x10:{
        responseHeader.statu=0x16;
        responseHeader.length=mysql.storeMailNum(userName);
        write((char*)&responseHeader,sizeof(response_header));

        if(responseHeader.length>0){
            simpleMailPackage=mysql.simpleStoreMail(userName);
            for(int i=0;i<responseHeader.length;i++){
                write((char*)&simpleMailPackage[i],sizeof(simpleMail_package));
                qDebug()<<"0x10---"<<simpleMailPackage[i].sender<<simpleMailPackage[i].time;
            }
        }

    }
        break;
    case 0x01:{//客户端发来是否有邮件请求
        responseHeader.statu=0x02;
        int num=mysql.checkMailNum(userName);
        responseHeader.length=num;

        write((char*)&responseHeader,sizeof(response_header));
        //发给客户端简单的邮件信息
        if(num>0){
            simpleMailPackage=mysql.sendSimpleMail(userName);
            for(int i=0;i<num;i++){
                write((char*)&simpleMailPackage[i],sizeof(simpleMail_package));
            }
        }
    }
        break;
    case 0x0E:{//客户端请求接收完整邮件信息

        read((char*)&infmForMailPackage,sizeof(infmForMail_package));

        //向客户端发送完整的包
        mailPackage=mysql.sendCheckMail(infmForMailPackage.sender,infmForMailPackage.time);

         responseHeader.statu=0x14;
         responseHeader.length=sizeof(mail_package);
         write((char*)&responseHeader,sizeof(response_header));

         write((char*)&mailPackage,sizeof(mail_package));

         QStringList pathstr=mysql.finPath(mailPackage.sender,mailPackage.time);



         for(int i=0;i<mailPackage.appendixCount;i++){
             char buf[20]={0};
             QString str=pathstr[3*i+2];
             strncpy(buf,str.toLatin1().data(),20);
             qDebug()<<"buf:"<<buf;
             write(buf,sizeof(buf));
         }

    }
        break;
    case 0x09:{//客户端每过几秒就来询问未读邮件
        responseHeader.statu=0x0A;
        responseHeader.length=mysql.checkNoReadMail(userName);
        qDebug()<<"responseHeader.length="<<responseHeader.length;
        write((char*)&responseHeader,sizeof(response_header));

    }
        break;
    case 0x06:{//发送的信息
        responseHeader.statu=0x07;
        int num=mysql.checkSendMailNum(userName);
        responseHeader.length=num;

        write((char*)&responseHeader,sizeof(response_header));
        qDebug()<<"0x06-num:"<<num;
        if(num>0){
            simpleMailPackage=mysql.sendSimpleMail2(userName);
            for(int i=0;i<num;i++){
                write((char*)&simpleMailPackage[i],sizeof(simpleMail_package));
            }
        }

    }
        break;
    case 0x0F:{//客户端接收完整的已发送邮件信息
        qDebug()<<"0x0F";
        read((char*)&infmForMailPackage,sizeof(infmForMail_package));
        qDebug()<<"infmForMailPackage.sender:"<<infmForMailPackage.sender;



        //向客户端发送完整的包
        mailPackage=mysql.sendCheckMail(infmForMailPackage.sender,infmForMailPackage.time);

         responseHeader.statu=0x15;
         responseHeader.length=sizeof(mail_package);
         write((char*)&responseHeader,sizeof(response_header));

         write((char*)&mailPackage,sizeof(mail_package));



//         for(int i=0;i<mailPackage.appendixCount;i++){
//             appendixPackage.appendixName=
//         }
    }
       break;
    case 0x11:{//客户端接收完整的草稿箱信息
        read((char*)&infmForMailPackage,sizeof(infmForMail_package));
        qDebug()<<"infmForMailPackage.sender:"<<infmForMailPackage.sender;
        //向客户端发送完整的包
        mailPackage=mysql.sendCheckMail(infmForMailPackage.sender,infmForMailPackage.time);

        responseHeader.statu=0x17;//回应草稿箱完整信息
        responseHeader.length=0;
        write((char*)&responseHeader,sizeof(response_header));

        write((char*)&mailPackage,sizeof(mail_package));

    }
        break;
    default:
        break;
    }

}
