#include "maindesktop.h"
#include "functional.h"
#include <QApplication>

storeThread * pushThread;
busyThread * copyThreads[4];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pushThread = new storeThread;
    for(int i = 0; i < 4; ++i)
        copyThreads[i] = new busyThread;
    MainDesktop w;
    QObject::connect(&w, &MainDesktop::send_path_toback, //将界面的路径发送给 线程对象
                     pushThread, &storeThread::set_path);
    QObject::connect(pushThread, &storeThread::finished,
                     [=](){
        pushThread->deleteLater();
        busyThread::set_flag(true);
    });
/*
    QObject::connect(pushThread, &storeThread::finished, // 销毁
                     pushThread, &QObject::deleteLater);
*/
    QObject::connect(&w, &MainDesktop::thread_bp_start,  //开启入队线程
                     pushThread, &storeThread::run_slot);
    for(int i = 0;i < 4;++i)
    {
        QObject::connect(copyThreads[i], &busyThread::finished, [=](){
           copyThreads[i]->deleteLater();
           copyThreads[i]->told_thread_cp_done();
        });
        /*
        QObject::connect(copyThreads[i], &busyThread::finished,
                         copyThreads[i], &QObject::deleteLater); //让线程结束后自动销毁
        QObject::connect(copyThreads[i], &busyThread::finished, // 告诉界面，线程已经完毕
                         copyThreads[i], &busyThread::told_thread_cp_done);
        */
        QObject::connect(copyThreads[i], &busyThread::thread_cp_done,
                         &w, &MainDesktop::receive_thread_cp_done);
        QObject::connect(&w, &MainDesktop::thread_cp_start,
                         copyThreads[i], &busyThread::run_slot); //开启线程(出队复制)
    }

    w.show();

    return a.exec();
}
