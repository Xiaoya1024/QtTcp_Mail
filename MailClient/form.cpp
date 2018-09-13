#include "form.h"
#include "ui_form.h"
#include <synchapi.h>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    timer=new QTimer(this);

    timer->start(10*1000);

}
void Form::fun(){
    qDebug()<<"kkk";
            client_header clientHeader;
            clientHeader.cmdType=0x09;
            clientHeader.length=sizeof(client_header);
            tcpSocket->write((char *)&clientHeader,sizeof(client_header));
}
Form::~Form()
{
    delete ui;
}
void Form::findNew()//循环找新邮件
{

}

void Form::on_Form_destroyed()
{
    emit mysignal();
}
void Form::getSort()//paixv
{

}
void Form::getCount(int x)
{
    ui->count->setText(QString::number(x, 10));
}
void Form::getName(QString name)
{
    ui->labelUserName->setText(name);
     connect(timer,SIGNAL(timeout()),this,SLOT(fun()));
//    while(1)
//    {Sleep(10*1000);
//        qDebug()<<"kkk";
//        client_header clientHeader;
//        clientHeader.cmdType=0x09;
//        clientHeader.length=sizeof(client_header);
//        tcpSocket->write((char *)&clientHeader,sizeof(client_header));



//    }
}
void Form::getMail(mail_package mailPackage1,QStringList str1)
{
    ui->stackedWidget->setCurrentIndex(3);

    QString str;

    str=QString::fromLocal8Bit(mailPackage1.sender);
    ui->lineEditSenderInbox->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.time);
    ui->lineEditTimeInBox->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.title);
    ui->lineEditSubjectInBox->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.context);
    ui->textBrowserInBox->setText(str);

    for(int i=0;i<mailPackage1.appendixCount;i++)
    {   QString s=str1[i];
        ui->comboInBox->addItem(s);
    }

    qDebug()<<mailPackage1.sender<<mailPackage1.time<<mailPackage1.title<<mailPackage1.context;


}
void Form::getMail2(mail_package mailPackage1)
{
    ui->stackedWidget->setCurrentIndex(5);

    QString str;

    str=QString::fromLocal8Bit(mailPackage1.recver);
    ui->lineEditRecSent->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.time);
    ui->lineEditTimeSent->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.title);
    ui->lineEditSubSent->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.context);
    ui->textBrowserSent->setText(str);

    //qDebug()<<mailPackage1.sender<<mailPackage1.time<<mailPackage1.title<<mailPackage1.context;


}
void Form::getMailDraft(mail_package mailPackage1)
{
    ui->stackedWidget->setCurrentIndex(7);

    QString str;

    str=QString::fromLocal8Bit(mailPackage1.recver);
    ui->lineEditRecDraft->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.time);
    ui->lineEditTimeDraft->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.title);
    ui->lineEditSubDraft->setText(str);

    str=QString::fromLocal8Bit(mailPackage1.context);
    ui->textBrowserDraft->setText(str);

    qDebug()<<mailPackage1.sender<<mailPackage1.time<<mailPackage1.title<<mailPackage1.context;
}
void Form::getSimple(simpleMail_package simpleMailPackage1)//填写收件箱

{
    int row = ui->InBoxTable->rowCount();
    qDebug()<<row;
    ui->InBoxTable->setRowCount(row+1);//添加一行
   QString str;
    str=QString::fromLocal8Bit(simpleMailPackage1.sender);
    ui->InBoxTable->setItem(row,0,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.title);
    ui->InBoxTable->setItem(row,1,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.time);
    ui->InBoxTable->setItem(row,2,new QTableWidgetItem(str));

    if(simpleMailPackage1.recvFlag==1)
        ui->InBoxTable->setItem(row,3,new QTableWidgetItem("Yes"));
    else
        ui->InBoxTable->setItem(row,3,new QTableWidgetItem("No"));
    qDebug()<<simpleMailPackage1.recvFlag;


}
void Form::getSimple2(simpleMail_package simpleMailPackage1)//填写收件箱

{
    int row = ui->SentTable->rowCount();
    qDebug()<<row;
    ui->SentTable->setRowCount(row+1);//添加一行
   QString str;
    str=QString::fromLocal8Bit(simpleMailPackage1.sender);
    ui->SentTable->setItem(row,0,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.title);
    ui->SentTable->setItem(row,1,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.time);
    ui->SentTable->setItem(row,2,new QTableWidgetItem(str));




}

void Form::getSimpleDtraft(simpleMail_package simpleMailPackage1)
{
    int row = ui->DraftTable->rowCount();
    qDebug()<<row;
    ui->DraftTable->setRowCount(row+1);//添加一行
   QString str;
    str=QString::fromLocal8Bit(simpleMailPackage1.sender);
    ui->DraftTable->setItem(row,0,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.title);
    ui->DraftTable->setItem(row,1,new QTableWidgetItem(str));

    str=QString::fromLocal8Bit(simpleMailPackage1.time);
    ui->DraftTable->setItem(row,2,new QTableWidgetItem(str));
}
void Form::sendInfo()//发送完整邮件需要的信息
{
     infmForMail_package infmForMailPackage;
     QString str;
     char*  ch;
     QByteArray ba;
     str=ui->InBoxTable->item(roww,0)->text();
      ba= str.toLatin1();
     ch=ba.data();
     strncpy(infmForMailPackage.sender,ch,20);

     str=ui->InBoxTable->item(roww,2)->text();
      ba= str.toLatin1();
     ch=ba.data();
     strncpy(infmForMailPackage.time,ch,20);
     qDebug()<<infmForMailPackage.sender<<infmForMailPackage.time;

     qint64 x=tcpSocket->write((char*)&infmForMailPackage,sizeof(infmForMail_package));
      // qint64 x= tcpSocket->write("hello",5);
     qDebug()<<x;
}

void Form::sendInfo3()//发送完整邮件需要的信息
{
     infmForMail_package infmForMailPackage;
     QString str;
     char*  ch;
     QByteArray ba;
     str=ui->labelUserName->text();
      ba= str.toLatin1();
     ch=ba.data();
     strncpy(infmForMailPackage.sender,ch,20);
    qDebug()<<infmForMailPackage.sender;
     str=ui->SentTable->item(row3,2)->text();
      ba= str.toLatin1();
     ch=ba.data();
     strncpy(infmForMailPackage.time,ch,20);
     qDebug()<<infmForMailPackage.time;
     qDebug()<<infmForMailPackage.sender<<infmForMailPackage.time;

     qint64 x=tcpSocket->write((char*)&infmForMailPackage,sizeof(infmForMail_package));
      // qint64 x= tcpSocket->write("hello",5);
     qDebug()<<x;
}
void Form::sendInfoDraft()
{
    infmForMail_package infmForMailPackage;
    QString str;
    char*  ch;
    QByteArray ba;
    str=ui->labelUserName->text();
     ba= str.toLatin1();
    ch=ba.data();
    strncpy(infmForMailPackage.sender,ch,20);

   qDebug()<<infmForMailPackage.sender;
    str=ui->SentTable->item(rowDraft,2)->text();
     ba= str.toLatin1();
    ch=ba.data();
    strncpy(infmForMailPackage.time,ch,20);

    qDebug()<<infmForMailPackage.sender<<infmForMailPackage.time;

    qint64 x=tcpSocket->write((char*)&infmForMailPackage,sizeof(infmForMail_package));
     // qint64 x= tcpSocket->write("hello",5);
    qDebug()<<x;
}

void Form::on_listWidget_itemClicked(QListWidgetItem *item)
{

}

void Form::on_listWidget_clicked(const QModelIndex &index)
{

        client_header clientHeader;

        int row=ui->listWidget->currentRow();
        switch (row) {
        case 0://写信

            ui->stackedWidget->setCurrentIndex(1);
            break;
        case 1://转到收信箱
            {
             ui->stackedWidget->setCurrentIndex(2);
             ui->InBoxTable->clear();
             ui->InBoxTable->setRowCount(0);
             QStringList headers;
              headers<<"sender"<<"title"<<"time"<<"Read before";
              ui->InBoxTable ->setColumnCount(4);//设置4列
              ui->InBoxTable->setHorizontalHeaderLabels(headers);
              ui->InBoxTable->horizontalHeader()->setStretchLastSection(true);
               ui->InBoxTable->verticalHeader()->setVisible(true);


            //发送收邮件请求
             clientHeader.cmdType=0x01;
             clientHeader.length=0;
             qint64 x=tcpSocket->write((char *)&clientHeader,sizeof(client_header));



            }
           break;
        case 2://转到发件箱
        {ui->stackedWidget->setCurrentIndex(4);
            ui->SentTable->clear();
            ui->SentTable->setRowCount(0);
            QStringList headers;
             headers<<"Receiver"<<"title"<<"time";
             ui->SentTable ->setColumnCount(3);//设置4列
             ui->SentTable->setHorizontalHeaderLabels(headers);
             ui->SentTable->horizontalHeader()->setStretchLastSection(true);
              ui->SentTable->verticalHeader()->setVisible(true);

              clientHeader.cmdType=0x06;
              clientHeader.length=0;
              tcpSocket->write((char *)&clientHeader,sizeof(client_header));

        }
            break;
        case 3://转到草稿箱
       { ui->stackedWidget->setCurrentIndex(6);
        ui->DraftTable->clear();
        ui->DraftTable->setRowCount(0);
        QStringList headers;
         headers<<"Receiver"<<"title"<<"time";
         ui->DraftTable ->setColumnCount(3);//设置4列
         ui->DraftTable->setHorizontalHeaderLabels(headers);
         ui->DraftTable->horizontalHeader()->setStretchLastSection(true);
          ui->DraftTable->verticalHeader()->setVisible(true);

          clientHeader.cmdType=0x10;
          clientHeader.length=0;
          tcpSocket->write((char *)&clientHeader,sizeof(client_header));
        }
            break;
        case 4://个人中心


            ui->stackedWidget->setCurrentIndex(8);
            break;

        default:
            break;
        }
}


void Form::on_buttonSendCompose_clicked() //发送邮件
{   QString str0=ui->labelUserName->text();
    QString str1=ui->lineEditToUserCompose->text();//收信人
    ui->lineEditToUserCompose->clear();
    QString str2=ui->lineEditSubjectCompose->text();//标题
    ui->lineEditSubjectCompose->clear();
    QString str3=ui->textEditContentCompose->toPlainText();//文本
    ui->textEditContentCompose->clear();
    int appendixCount=ui->comboBoxCompose->count();

    QString str4=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
        requestforregister.cmdType=0x02;
        requestforregister.length=sizeof(register_package);
         tcpSocket->write((char *)&requestforregister, sizeof(client_header));

        char*  ch;

        mail_package mailPackage;
        QByteArray ba = str2.toLatin1();
        ch=ba.data();
        strncpy(mailPackage.title,ch,30);

         ba = str0.toLatin1();
        ch=ba.data();
        strncpy(mailPackage.sender,ch,20);
        qDebug()<<mailPackage.sender;
         ba = str1.toLatin1();
        ch=ba.data();
        strncpy(mailPackage.recver,ch,20);

        ba = str4.toLatin1();
       ch=ba.data();
        strncpy(mailPackage.time,ch,20);

        ba = str3.toLatin1();
       ch=ba.data();
        strncpy(mailPackage.context,ch,100);

        mailPackage.appendixCount=appendixCount;



        //传邮件协议包
         tcpSocket->write((char *)&mailPackage, sizeof(mail_package));

         //协议包
        appendix_package appendixPackage;


         for(int i=0;i<appendixCount;i++)
         {
             QFileInfo info(filepaths[i]);
             qDebug()<<filepaths[i];
             fileName = info.fileName(); //获取文件名字
            fileSize = info.size(); //获取文件大小
            qDebug()<<fileSize;


             QByteArray ba = fileName.toLatin1();
             char *ch=ba.data();
            strncpy(appendixPackage.appendixName,ch,30);
            appendixPackage.appendixLen=fileSize;

            tcpSocket->write((char*)&appendixPackage,sizeof(appendix_package));

            int len=0;
            file.setFileName(filepaths[i]);
             bool ok=file.open(QIODevice::ReadOnly);
             if(ok) qDebug()<<"yes";

            while(1)
            {
                char buf[25]={0};
                len=0;
                len=file.read(buf,sizeof(buf));
                qDebug()<<"len:"<<len;
                qDebug()<<buf;
                if(len==0) break;

                len=tcpSocket->write(buf,len);
                qDebug()<<"len:"<<len;
            }
            file.close();

         }
        filepaths.clear();
        ui->comboBoxCompose->clear();
    }

}

void Form::on_buttonSaveCompose_clicked()//存放到草稿箱
{
    QString str0=ui->labelUserName->text();
        QString str1=ui->lineEditToUserCompose->text();//收信人
        ui->lineEditToUserCompose->clear();
        QString str2=ui->lineEditSubjectCompose->text();//标题
        ui->lineEditSubjectCompose->clear();
        QString str3=ui->textEditContentCompose->toPlainText();//文本
        ui->textEditContentCompose->clear();
        int appendixCount=ui->comboBoxCompose->count();
        QString str4=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
            requestforregister.cmdType=0x04;
            requestforregister.length=sizeof(register_package);
             tcpSocket->write((char *)&requestforregister, sizeof(client_header));

            char*  ch;

            mail_package mailPackage;
            QByteArray ba = str2.toLatin1();
            ch=ba.data();
            strncpy(mailPackage.title,ch,30);

             ba = str0.toLatin1();
            ch=ba.data();
            strncpy(mailPackage.sender,ch,20);
            qDebug()<<mailPackage.sender;
             ba = str1.toLatin1();
            ch=ba.data();
            strncpy(mailPackage.recver,ch,20);

            ba = str4.toLatin1();
           ch=ba.data();
            strncpy(mailPackage.time,ch,20);

            ba = str3.toLatin1();
           ch=ba.data();
            strncpy(mailPackage.context,ch,100);
            mailPackage.appendixCount=appendixCount;



            //传邮件协议包
             tcpSocket->write((char *)&mailPackage, sizeof(mail_package));


        }
}

void Form::on_InBoxTable_cellClicked(int row, int column)//查询收件箱具体信息
{
    roww=row;
    client_header clientHeader;

    //发送完整邮件的请求
    clientHeader.cmdType=0x0E;
    clientHeader.length=sizeof(infmForMail_package);
    tcpSocket->write((char *)&clientHeader,sizeof(client_header));
    sendInfo();

}

void Form::on_buttonReturnInBox_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Form::on_SentTable_cellClicked(int row, int column)
{
    qDebug()<<"333";
    row3=row;
    client_header clientHeader;

    //发送完整邮件的请求
    clientHeader.cmdType=0x0F;
    clientHeader.length=sizeof(infmForMail_package);
    qint64 x=tcpSocket->write((char *)&clientHeader,sizeof(client_header));
    qDebug()<<x;
    sendInfo3();
}

void Form::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Form::on_DraftTable_cellClicked(int row, int column)
{
    qDebug()<<"draft11";
    rowDraft=row;
    client_header clientHeader;

    //发送完整邮件的请求
    clientHeader.cmdType=0x11;
    clientHeader.length=sizeof(infmForMail_package);
    qint64 x=tcpSocket->write((char *)&clientHeader,sizeof(client_header));
    //qDebug()<<x;
    sendInfoDraft();
}

void Form::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void Form::on_buttonAttachFilerCompose_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
    filepaths<<filePath;
    qDebug()<<filepaths;
    if(false == filePath.isEmpty()) //如果选择文件路径有效
    {
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字

        ui->comboBoxCompose->addItem(fileName);
        sendSize = 0; //发送文件的大小

        //只读方式打开文件
        //指定文件的名字
        //file.setFileName(filePath);

        //打开文件
       // bool isOk = file.open(QIODevice::ReadOnly);
//        if(false == isOk)
//        {
//            qDebug() << "只读方式打开文件失败 106";

//        }

        //提示打开文件的路径
        //ui->textEdit->append(filePath);

//        ui->buttonFile->setEnabled(false);
//        ui->buttonSend->setEnabled(true);

    }
    else
    {
        qDebug() << "选择文件路径出错 118";
    }

}

void Form::on_buttonDownloadAttachFile_clicked()
{
//    QString str =ui->comboInBox->currentText();
//    //发送 一个包
//    QString path = QDir::currentPath();
//    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), path, tr("Image Files (*.bmp)"));

}
