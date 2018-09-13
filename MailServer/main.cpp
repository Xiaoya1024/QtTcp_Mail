//#include "ServerForm.h"
#include <QApplication>
#include "mysql.h"
#include"server.h"
#include"mysocket.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server s;
    s.listen(QHostAddress::Any,8000);

    qDebug()<<"listen";
    //ServerForm w;
//    w.setWindowTitle("服务器控制台");
//    w.show();


    return a.exec();
}
