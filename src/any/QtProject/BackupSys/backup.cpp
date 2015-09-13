#include "backup.h"

BackUp::BackUp()
{

}

bool BackUp::start_bp(const QString localfrompath, const QString localtopath)
{
    QDir sourceDir;
    QDir destinDir;
    if(localfrompath.isNull())
        sourceDir.setPath(fromPath) ;
    else
        sourceDir.setPath(localfrompath);
    if(localtopath.isNull())
        destinDir.setPath(toPath);
    else
        destinDir.setPath(localtopath);
    // 如果目的文件夹不存在，就创建一个
    if(!destinDir.exists())
    {
        ++dirs;
        if(!destinDir.mkdir(destinDir.absolutePath()))
        {
            QMessageBox::critical(this, QStringLiteral("警告"),
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
            if(!backup(fileInfo.filePath(),
                       destinDir.filePath(fileInfo.fileName())))
            {
                QMessageBox::critical(this, QStringLiteral("警告"),
                                      QStringLiteral("递归创建文件夹:") + fileInfo.fileName() + QStringLiteral("失败"));
                return false;
            }
        }
        else //如果不是文件夹
        {
            queEmpty.acquire();
            ++files;
            fileVec.enqueue(combine(fileInfo.filePath(),
                                      destinDir.filePath(fileInfo.fileName())));
            queFull.release(1);
        }
    }
    return true;
}
