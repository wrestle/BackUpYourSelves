#include "maindesktop.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDesktop w;
    w.show();

    return a.exec();
}
