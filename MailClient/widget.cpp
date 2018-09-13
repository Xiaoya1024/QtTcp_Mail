#include "widget.h"
#include "ui_widget.h"
#include<QStackedWidget>
#include<QMessageBox>
#include<QDebug>

#define ip "192.168.43.13"
#define port 8000


//0.login 1.register 2.forgot 3.newPass
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEditPasswordLog->setEchoMode(QLineEdit::Password);
    ui->lineEditPasswordReg->setEchoMode(QLineEdit::Password);
    ui->lineEditNewPassword->setEchoMode(QLineEdit::Password);

    tcpSocket=new QTcpSocket(this);

    tcpSocket->abort();   //取消已有的连接
    tcpSocket->connectToHost(ip,port);

    connect(tcpSocket,&QTcpSocket::connected,
            [=](){
        qDebug()<<"Link successful";
    });

    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));   //发生错误时执行displayError函数

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessages()));//连接好之后准备接收服务器发来下的数据
    //myform关闭后 这个窗口也关掉
    //connect(myform,SIGNAL(mysignal()),this,SLOT(close()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();   //输出出错信息
}

void Widget::readMessages(){   
    response_header responseHeader;
    mail_package mailPackage;
    infmForMail_package infmForMailPackage;

    simpleMail_package simpleMailPackage;
    qint64 x=tcpSocket->read((char *)&responseHeader,sizeof(response_header));
    qDebug()<<x;


    switch (responseHeader.statu) {
    case 0x01://注册成功
        qDebug()<<"yes";
        QMessageBox::information(this,"message","Success!",QMessageBox::Ok);
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 0x00://注册失败
        qDebug()<<"no";
        QMessageBox::information(this,"warning","Fail!Same name has been existed!",QMessageBox::Ok);
        break;
    case 0x0C://登陆成功
        QMessageBox::information(this,"message","Sucess！",QMessageBox::Ok);
        this->hide();
        //找到用户名
        myform=new Form();

        myform->tcpSocket=tcpSocket;
        myform->getName(name);
        myform->show();
        break;
    case 0x0D://登录失败
        QMessageBox::information(this,"warning","Fail!!",QMessageBox::Ok);
        break;
    case 0x0E://找回密码成功
        ui->stackedWidget->setCurrentIndex(3);
        break;
    case 0x0F://找回密码失败
        QMessageBox::information(this,"warning","Fail!!",QMessageBox::Ok);
        break;
    case 0x10://重置成功
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 0x11://重置失败
        QMessageBox::information(this,"warning","Fail!!",QMessageBox::Ok);
        break;
    case 0x03://传邮件成功
        qDebug()<<"sss";
        break;
    case 0x12://传邮件失败
        qDebug()<<"nnn";
        break;
    case 0x05://存草稿成功
        break;
    case 0x13://存草稿失败
        break;
    case 0x02://接收简单邮件
        qDebug()<<"收到0x02";
        if(responseHeader.length==0)
        {
            //no
            qDebug()<<"my";
        }
        else
        {
            int i;
            for(i=0;i<responseHeader.length;i++)
            {
                qint64 x=tcpSocket->read((char*)&simpleMailPackage,sizeof(simpleMail_package));
                //qDebug()<<x;
                myform->getSimple(simpleMailPackage);
               // qDebug()<<simpleMailPackage.sender<<simpleMailPackage.time<<simpleMailPackage.title;
            }
        }
        break;

    case 0x14://接收完整邮件请求响应,发送需要的信息
    {qDebug()<<"shoudao0x14";

        qint64 x=tcpSocket->read((char*)&mailPackage,sizeof(mail_package));
        qDebug()<<x;
        qDebug()<<mailPackage.appendixCount;
        QStringList strpath;
        for(int i=0;i<mailPackage.appendixCount;i++)
        {
            char buf[20]={0};
            tcpSocket->read(buf,sizeof(buf));
            qDebug()<<buf;
            strpath<<QString::fromLocal8Bit(buf);

        }



        myform->getMail(mailPackage,strpath);

    }
        break;
    case 0x15:
    {
        qint64 x=tcpSocket->read((char*)&mailPackage,sizeof(mail_package));
        qDebug()<<x;
        qDebug()<<mailPackage.sender;
        myform->getMail2(mailPackage);
    }

        break;
    case 0x0A:
        //qDebug()<<"ddd";

            //  QMessageBox::information(this,"message",QString("you have '%1'not read!").arg(responseHeader.length),QMessageBox::Ok);
            //qDebug()<<"lala";
            myform->getCount(responseHeader.length);
         break;
     case 0x07:
        if(responseHeader.length==0)
        {
            //no
            qDebug()<<"my111";
        }
        else
        {
            int i;
            for(i=0;i<responseHeader.length;i++)
            {
                qint64 x=tcpSocket->read((char*)&simpleMailPackage,sizeof(simpleMail_package));
                //qDebug()<<x;
                myform->getSimple2(simpleMailPackage);
                qDebug()<<simpleMailPackage.sender<<simpleMailPackage.time<<simpleMailPackage.title;
            }
        }
        break;
     case 0x16: //草稿箱
        qDebug()<<"收到0x16";
        if(responseHeader.length==0)
        {
            //no
            qDebug()<<"my16";
        }
        else
        {   qDebug()<<"length"<<responseHeader.length;
            int i;
            for(i=0;i<responseHeader.length;i++)
            {
                qint64 x=tcpSocket->read((char*)&simpleMailPackage,sizeof(simpleMail_package));
                qDebug()<<x;
                myform->getSimpleDtraft(simpleMailPackage);
               qDebug()<<simpleMailPackage.sender<<simpleMailPackage.time<<simpleMailPackage.title;
            }
        }

        break;
    case 0x17://草稿箱完整信息
    {
        qDebug()<<"收到0x17";
        tcpSocket->read((char*)&mailPackage,sizeof(mail_package));

        qDebug()<<mailPackage.sender<<mailPackage.time;
        myform->getMailDraft(mailPackage);
    }
    break;

    default:
        break;
    }


}

void Widget::on_buttonForgotg_clicked()//点击忘记密码，跳转到2页
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_buttonRequest_clicked()//点击申请新用户按钮，跳转到1页
{
    ui->stackedWidget->setCurrentIndex(1);


}


void Widget::on_buttonRegister_clicked()//点击注册按钮，如果成功跳转到0页
{


    QString str1=ui->lineEditUserNameReg->text();
    ui->lineEditUserNameReg->clear();
    QString str2=ui->lineEditPasswordReg->text();
    ui->lineEditPasswordReg->clear();
    QString str3=ui->lineEditPhoneNumberReg->text();
    ui->lineEditPhoneNumberReg->clear();
//    QByteArray cpath = str1.toLocal8Bit();
//    char* name = cpath.data();
    //判断填写内容是否为空
    if(str1.isEmpty()||str2.isEmpty()||str3.isEmpty())
    {
        qDebug()<<"Register :Can't be empty!";
        QMessageBox::information(this,"Warning","Can't be empty!",QMessageBox::Ok);

    }
    else
    {
        //首先发送请求报文
        client_header requestforregister;
        requestforregister.cmdType=0x0B;
        requestforregister.length=sizeof(register_package);
        tcpSocket->write((char *)&requestforregister, sizeof(client_header));
         char*  ch;

        register_package registerPackage;
        QByteArray ba = str1.toLatin1();
        ch=ba.data();

        strncpy(registerPackage.username,ch,20);
         ba = str2.toLatin1();
        ch=ba.data();

        strncpy(registerPackage.password,ch,20);
         ba = str3.toLatin1();
        ch=ba.data();

        strncpy(registerPackage.phone,ch,12);
        //qDebug()<<registerPackage.username<<registerPackage.password<<registerPackage.phone;

        //传注册协议包
         tcpSocket->write((char *)&registerPackage, sizeof(register_package));


    }

}

void Widget::on_buttonSureNewPass_clicked()//改完新密码，跳转到登录界面
{
    QString str1=ui->lineEditNewPassword->text();
    ui->lineEditNewPassword->clear();

//    QByteArray cpath = str1.toLocal8Bit();
//    char* name = cpath.data();
    //判断填写内容是否为空
    if(str1.isEmpty())
    {
        qDebug()<<"Register :Can't be empty!";
        QMessageBox::information(this,"Warning","Can't be empty!",QMessageBox::Ok);

    }
    else
    {
        //首先发送请求报文
        client_header requestforregister;
        requestforregister.cmdType=0x0D;
        requestforregister.length=sizeof(register_package);
        tcpSocket->write((char *)&requestforregister, sizeof(client_header));
         char*  ch;

        newPass_package newPassPackage;
        QByteArray ba = str1.toLatin1();
        ch=ba.data();

        strncpy(newPassPackage.password,ch,20);


        //传注册协议包
         tcpSocket->write((char *)&newPassPackage, sizeof(newPass_package));


    }

}

void Widget::on_buttonSure_clicked()//找回密码
{
    QString str1=ui->lineEditUserNameFid->text();
    ui->lineEditUserNameFid->clear();
    QString str2=ui->lineEditPhoneNumberFid->text();
    ui->lineEditPhoneNumberFid->clear();
    if(str1.isEmpty()||str2.isEmpty())
    {
        qDebug()<<"Register :Can't be empty!";
        QMessageBox::information(this,"Warning","Can't be empty!",QMessageBox::Ok);
    }
    else
    {
        //首先发送请求报文
        client_header requestforregister;
        requestforregister.cmdType=0x0C;
        requestforregister.length=sizeof(register_package);
        tcpSocket->write((char *)&requestforregister, sizeof(client_header));
         char*  ch;

        findPass_package findPassPackage;
        QByteArray ba = str1.toLatin1();
        ch=ba.data();
        strncpy(findPassPackage.username,ch,20);

         ba = str2.toLatin1();
        ch=ba.data();
        strncpy(findPassPackage.phone,ch,12);

        //传登陆协议包
         tcpSocket->write((char *)&findPassPackage, sizeof(findPass_package));


    }


}

void Widget::on_buttonLogin_clicked()//登陆
{
    //登陆请求
    QString str1=ui->lineEditUserNameLog->text();
    name=str1;
    ui->lineEditUserNameLog->clear();
    QString str2=ui->lineEditPasswordLog->text();
    ui->lineEditPasswordLog->clear();
    if(str1.isEmpty()||str2.isEmpty())
    {
        qDebug()<<"Register :Can't be empty!";
        QMessageBox::information(this,"Warning","Can't be empty!",QMessageBox::Ok);
    }
    else
    {
        //首先发送请求报文
        client_header requestforregister;
        requestforregister.cmdType=0x00;
        requestforregister.length=sizeof(register_package);
        tcpSocket->write((char *)&requestforregister, sizeof(client_header));
         char*  ch;

        login_package loginPackage;
        QByteArray ba = str1.toLatin1();
        ch=ba.data();
        strncpy(loginPackage.username,ch,20);

         ba = str2.toLatin1();
        ch=ba.data();
        strncpy(loginPackage.password,ch,20);

        //传登陆协议包
         tcpSocket->write((char *)&loginPackage, sizeof(login_package));


    }



}

void Widget::on_buttonRegister_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_buttonSure_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_buttonSureNewPass_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
