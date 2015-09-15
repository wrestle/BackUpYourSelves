﻿#ifndef FUNCTIONAL_H_INCLUDE
#define FUNCTIONAL_H_INCLUDE

#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QQueue>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <QDialog>
#include <QDebug>
static QMutex flag_locker;
struct combine{
    combine(const QString arg1, const QString arg2):fp(arg1), tp(arg2){}
    QString fp;
    QString tp;
};

static QQueue<combine> filesVec;
bool start_cp(const QString & src, const QString & dst);

bool start_bp(const QString localfrompath, const QString localtopath);


class storeThread : public QThread{
    Q_OBJECT
public:

    void set_path(const QString arg1, const QString arg2){
        from_path = arg1;
        to_path = arg2;
    }
    void run_slot(){
        filesVec.clear();
        this->start();
    }

protected:
    void run() override;
private:
    QString from_path;
    QString to_path;
};


class busyThread : public QThread{
    Q_OBJECT
public:
    void run_slot(){
        this->start();
    }
    static void set_flag(bool flag)
    {
        flag_locker.lock(); // 上方定义
        pushThread_finish = flag;
        flag_locker.unlock();
        return;
    }

signals:
    void thread_cp_done(); // 提示界面拷贝线程已经完毕

public slots:
    void told_thread_cp_done(){
        flag_locker.lock();
        if(copyThread_finish == 4)
            emit thread_cp_done();
        flag_locker.unlock();
        //emit thread_cp_done(); // 发送信号
    }

protected:
    void run() Q_DECL_OVERRIDE; // override

private:
    QMutex locker;
    static int copyThread_finish;
    static bool pushThread_finish; //判断 入队线程是否结束
    static unsigned long int newfiles;
};

#endif // FUNCTIONAL

