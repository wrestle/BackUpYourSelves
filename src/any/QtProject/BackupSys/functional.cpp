#include  <QDebug>

#include "functional.h"
static QSemaphore queEmptys(20);
static QSemaphore queFulls(20);
static QQueue<combine> filesVec;
static QMutex flag_locker;

static std::ofstream writing; // 写入被本次复制的文件
static std::ofstream err_log; // 写入本次失败复制的文件

/**
 * @brief start_bp
 * @return false if Create Dir Fail
 * @version v2.1
 * To Create Dir and put the file path into queue which is used to copyThreads.
 * Global Function
 */
bool start_bp(const QString localfrompath, const QString localtopath)
{
    QDir sourceDir(localfrompath);
    QDir destinDir(localtopath);
    // 如果目的文件夹不存在，就创建一个
    if(!destinDir.exists())
    {
        if(!destinDir.mkdir(destinDir.absolutePath()))
        {
           // QMessageBox::critical(nullptr, QStringLiteral("警告"),
           //                       QStringLiteral("创建文件夹失败!"));
            QMessageBox bad_thing;
            bad_thing.setWindowTitle(QStringLiteral("警告"));
            bad_thing.setText(QStringLiteral("创建文件夹失败!"));
            bad_thing.exec();
            return false;
        }

    }
    //开始将本目录下，所有文件入队，所有文件夹创建在目的路径
    QFileInfoList alltarget = sourceDir.entryInfoList();
    foreach (auto fileInfo, alltarget)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()) // 如果是文件夹
        {
            if(!start_bp(fileInfo.filePath(), destinDir.filePath(fileInfo.fileName())))
                return false;
        }
        else //如果不是文件夹
        {
            queEmptys.acquire(1);
            flag_locker.lock();
            filesVec.enqueue(combine(fileInfo.filePath(),
                                      destinDir.filePath(fileInfo.fileName())));
            flag_locker.unlock();
            queFulls.release(1);
        }
    }
    return true;
}

/**
 * @brief start_cp
 * @return false if exist without changed.
 * @version v2.2
 * Global function
 */
bool start_cp(const QString & src, const QString & dst)
{
    QFileInfo fPath(src);
    QFileInfo tPath(dst);

    if(tPath.exists()) // 如果待复制的文件已经存在
    {

        if(fPath.lastModified() == tPath.lastModified()) // 判断是否被改变
        {
#if !defined(NOT_DEBUG_AT_ALL)
            qDebug() << QStringLiteral("文件相同，无需复制");
#endif
            return false;
        }
    }
    else
    {
        if(!QFile::copy(src, dst))
            return false;
            //QMessageBox::critical(NULL, QStringLiteral("警告"),
            //                      QStringLiteral("备份文件:") + src + QStringLiteral("失败"));
    }
    return true;
}

void storeThread::run_slot(){
    filesVec.clear();
    this->start();
}

/**
 * @brief busyThread::run override
 * @version v2.2
 * Copy the File until the queue is EMPTY and pushThread is DONE.
 */
void busyThread::run()
{
#if !defined(NOT_DEBUG_AT_ALL)
    qDebug() << this->currentThread() << "start";
#endif
    while(true)
    {
        if(!queFulls.tryAcquire(1, 5000)) // 尝试请求资源
        {// 如果没有成功，即超时或者资源少于1
            locker.lock();
            if(pushThread_finish && filesVec.isEmpty())
            { // 如果没有待复制文件，并且入队的线程已经结束，就结束线程
                locker.unlock();
                break;
            }
            locker.unlock();
            continue;
        }
        locker.lock();
        combine local(filesVec.dequeue());
        locker.unlock();
        if(start_cp(local.fp, local.tp))
        {
            locker.lock();
            ++newfiles; //记录新增或修改的文件数量
            writing << local.fp.toStdString() << std::endl;
            locker.unlock();
        }
        else
        {
            locker.lock();
            err_log << local.fp.toStdString() << " <<<<<<COPY FAIL!"<< std::endl;
            locker.unlock();
        }
        queEmptys.release(); // 使用资源完毕
    }
    flag_locker.lock();
    ++copyThread_finish; // 已经结束的线程
    flag_locker.unlock();
#if !defined(NOT_DEBUG_AT_ALL)
    qDebug() << this->currentThread() << "end";
#endif
    return;
}

/**
 * @brief busyThread::told_thread_cp_done
 * @version v2.2
 * To tell MainWindow that copy file thread is done.
 */
void busyThread::told_thread_cp_done(){
    flag_locker.lock();
    if(copyThread_finish == 4)
    {
        writing.close();
        err_log.close();
        emit thread_cp_done(newfiles);
        newfiles = 0;              //等待下一次的复制
        copyThread_finish = 0;
        pushThread_finish = false;
    }
    flag_locker.unlock();
}

/**
 * @brief busyThread::set_flag
 * @version v2.1
 * To Tell busyThread that pushThread is end.
 */
void busyThread::set_flag(bool flag)
{
    flag_locker.lock(); // 上方定义
    pushThread_finish = flag;
    flag_locker.unlock();
    return;
}

bool busyThread::pushThread_finish = 0;
int busyThread::copyThread_finish = 0;
int busyThread::newfiles = 0;

/**
 * @brief storeThread::run override
 * @version v2.2
 */
void storeThread::run(){
    time_t time_now = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z",localtime(&time_now) );
    std::string time(tmp);
    int avail = queFulls.available(); // 因为一开始的队列中为空
    queFulls.acquire(avail);
    err_log.open("CopyErrorLog", std::ios_base::app);
    err_log << time << std::endl;
    writing.open("CopySuccessLog", std::ios_base::out | std::ios_base::trunc);
    writing << time << std::endl;
    start_bp(from_path, to_path); //开始将文件路径入队
    return;
}
