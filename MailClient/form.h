#ifndef FORM_H
#define FORM_H

#include <QDateTime>
#include <QWidget>
#include <QListWidget>
#include "port.h"
#include "widget.h"
#include <QMessageBox>
#include<QTimer>
#include <QDesktopServices>
#include <QFileDialog>
#include <QStringList>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void findNew();
    void getName(QString name);
    void getMail(mail_package mailPackage1,QStringList str);
    void getMail2(mail_package mailPackage1);
    void getMailDraft(mail_package mailPackage1);

    void getSimple(simpleMail_package simpleMailPackage1);
    void getSimple2(simpleMail_package simpleMailPackage1);
    void getSimpleDtraft(simpleMail_package simpleMailPackage1);

    void getSort();
    void sendInfo();
    void sendInfo3();
    void sendInfoDraft();

void getCount(int x);
    Ui::Form *ui;
    QTcpSocket *tcpSocket;
    mail_package mailPackage;
    simpleMail_package simpleMailPackage;
    int roww;
    int row3;//已发送
    int rowDraft;

    QFile file; //文件对象
    QString fileName; //文件名字
    int fileSize; //文件大小
    int sendSize; //已经发送文件的大小

    QStringList filepaths;


private slots:
    void on_Form_destroyed();
    void on_listWidget_itemClicked(QListWidgetItem *item);


    void on_listWidget_clicked(const QModelIndex &index);

    void on_buttonSendCompose_clicked();

    void on_buttonSaveCompose_clicked();

    void on_InBoxTable_cellClicked(int row, int column);

    void on_buttonReturnInBox_clicked();
    void fun();


    void on_SentTable_cellClicked(int row, int column);

    void on_pushButton_7_clicked();

    void on_DraftTable_cellClicked(int row, int column);

    void on_pushButton_9_clicked();

    void on_buttonAttachFilerCompose_clicked();

    void on_buttonDownloadAttachFile_clicked();

signals:
    void mysignal();

private:
    QTimer *timer;

};

#endif // FORM_H
