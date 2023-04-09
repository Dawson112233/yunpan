#include "widget.h"
#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    ClientWidget client;
    client.show();
//    ClientWidget client2;
//    client2.show();
//    ClientWidget client3;
//    client3.show();
    return a.exec();
}
