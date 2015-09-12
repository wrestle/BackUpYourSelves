#include "backupsys.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BackupSys w;
    w.show();

    return a.exec();
}
