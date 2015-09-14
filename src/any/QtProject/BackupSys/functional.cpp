#include "functional.h"
QSemaphore queEmptys(20);
QSemaphore queFulls(20);

bool start_bp(const QString localfrompath, const QString localtopath)
{
    QDir sourceDir(localfrompath);
    QDir destinDir(localtopath);

    // 如果目的文件夹不存在，就创建一个
    if(!destinDir.exists())
    {
        //++dirs;
        if(!destinDir.mkdir(destinDir.absolutePath()))
        {
            QMessageBox::critical(nullptr, QStringLiteral("警告"),
                                  QStringLiteral("创建文件夹失败!"));
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
            if(!start_bp(fileInfo.filePath(),
                       destinDir.filePath(fileInfo.fileName())))
            {
                QMessageBox::critical(nullptr, QStringLiteral("警告"),
                                      QStringLiteral("递归创建文件夹:") + fileInfo.fileName() + QStringLiteral("失败"));
                return false;
            }
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

bool start_cp(const QString & src, const QString & dst)
{
    QFileInfo fPath(src);
    QFileInfo tPath(dst);

    if(tPath.exists())
    {
#if !defined(NOT_DEBUG_AT_ALL)
        if(fPath.lastModified() == tPath.lastModified())
        {
            qDebug() << QStringLiteral("文件相同，无需复制");
        }
#else
#endif
    }
    else
    {
        if(!QFile::copy(src, dst))
            QMessageBox::critical(NULL, QStringLiteral("警告"),
                                  QStringLiteral("备份文件:") + src + QStringLiteral("失败"));
    }
    return true;
}

void busyThread::run()
{
    while(1)
    {
        locker.lock();
        if(pushThread_finish && filesVec.isEmpty())
        {
            locker.unlock();
            break;
        }
        locker.unlock();
        queFulls.acquire(); // 请求是否资源
        locker.lock();
        combine local(filesVec.dequeue());
        locker.unlock();
        start_cp(local.fp, local.tp);
        queEmptys.release(); // 使用资源完毕
    }
    flag_locker.lock();
    ++copyThread_finish;
    flag_locker.unlock();
    //emit add_finish(); // 发送信号给自身，判断是finish是否到达要求
    return;
}
bool busyThread::pushThread_finish = 0;
int busyThread::copyThread_finish = 0;

void storeThread::run(){
    int avail = queFulls.available();
    queFulls.acquire(avail);
    start_bp(from_path, to_path);
    return;
}
