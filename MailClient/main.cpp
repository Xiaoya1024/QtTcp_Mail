#include "widget.h"
#include <QApplication>
#include "form.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Widget w;
    //Form w;
    w.show();


    return a.exec();

}
