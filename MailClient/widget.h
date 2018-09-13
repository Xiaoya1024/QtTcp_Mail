#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include "port.h"
#include <QString>
#include <QByteArray>
#include "form.h"
#include <QStringList>


class Form;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void init();
    void connectServer();

private slots:
    void on_buttonForgotg_clicked();

    void on_buttonRequest_clicked();

    void on_buttonRegister_clicked();

    void on_buttonSureNewPass_clicked();

    void on_buttonSure_clicked();

    void displayError(QAbstractSocket::SocketError);

    void readMessages();

    void on_buttonLogin_clicked();

    void on_buttonRegister_2_clicked();

    void on_buttonSure_2_clicked();

    void on_buttonSureNewPass_2_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    Form *myform;
    QString name;
};

#endif // WIDGET_H
