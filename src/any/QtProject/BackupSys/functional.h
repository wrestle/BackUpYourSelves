/**
 * functional.h/cpp 为编写线程类的文件
*/
#ifndef FUNCTIONAL_H_INCLUDE
#define FUNCTIONAL_H_INCLUDE
#ifndef NOT_DEBUG_AL_ALL
#define NOT_DEBUG_AL_ALL
#endif
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QQueue>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <QDialog>

#if !defined(NOT_DEBUG_AL_ALL)
#include <QDebug>
#endif

struct combine;

bool start_cp(const QString & src, const QString & dst);
bool start_bp(const QString localfrompath, const QString localtopath);

/**
 * @brief The storeThread class
 * 该类的作用为将待复制的文件路径压入队列中
 */
class storeThread : public QThread{
    Q_OBJECT
public:
    void set_path(const QString arg1, const QString arg2){
        from_path = arg1;
        to_path = arg2;
    }
    void run_slot(); // 执行 run()

protected:
    void run() override;
private:
    QString from_path;
    QString to_path;
};

/**
 * @brief The busyThread class
 * 该类作用为，将队列中的文件复制。
 */
class busyThread : public QThread{
    Q_OBJECT
public:
    void run_slot(){
        this->start();
    }
    static void set_flag(bool flag);

signals:
    void thread_cp_done(int numbers); // 提示界面拷贝线程已经完毕

public slots:
    void told_thread_cp_done(); // 让 copyThreads 与 MainWindow 交互

protected:
    void run() Q_DECL_OVERRIDE; // override

private:
    QMutex locker;
    static int copyThread_finish;  //判断 复制线程是否结束
    static bool pushThread_finish; //判断 入队线程是否结束
    static int newfiles;  // 实际执行复制操作的文件数量
};

/**
 * @brief The combine struct
 * 将备份路径和存储路径放在一个单元内
 */
struct combine{
    combine(const QString arg1, const QString arg2):fp(arg1), tp(arg2){}
    QString fp;
    QString tp;
};
#endif // FUNCTIONAL

