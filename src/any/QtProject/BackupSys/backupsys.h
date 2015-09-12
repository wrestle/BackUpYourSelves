#ifndef BACKUPSYS_H
#define BACKUPSYS_H

#if !defined(NOT_DEBUG_AT_ALL)
#include <QDebug>
#endif

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QQueue>
#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <QDialog>

namespace Ui {
class BackupSys;
}

struct combine{
    combine(const QString & path1, const QString & path2):
        curFromPath(path1), curToPath(path2) {}

    QString curFromPath; //拷贝的源路径
    QString curToPath;   //拷贝的目的路径
};

static QQueue<combine> fileVec; // 队列用于拷贝

class BackupSys : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackupSys(QWidget *parent = 0);
    ~BackupSys();
    // nullptr 是为了第一次调用时直接使用成员 fromPath, toPath
    bool backup(const QString localfrompath = nullptr, const QString localtopath = nullptr);


private slots:
    void on_fromButton_clicked();
    void on_toButton_clicked();
    void on_fromEdit_textChanged(const QString &arg1);
    void on_toEdit_textChanged(const QString &arg1);
    void on_startButton_clicked();

private:
    Ui::BackupSys *ui;
    QString fromPath;   //从此处备份
    QString toPath;     //备份存储在此
    int files;       //记录本次增加的文件数目
    int dirs;        //记录本次增加的文件夹数目
};

// 压入拷贝的文件
class storeThread : public QThread{
    Q_OBJECT
public:
    storeThread(BackupSys * pointer): Pointer(pointer){}

protected:
    void run(){
         Pointer->backup();
         return;
    }
public slots:
    void runslot(){
        run();
    }
private:
    BackupSys * Pointer;
};

// 拷贝文件
class copyThread : public QThread{
    Q_OBJECT
public:

public slots:
    void runslot(){
        run();
    }
protected:
    void run();
private:
    QMutex locker;
    static unsigned int newfiles;
};

#endif // BACKUPSYS_H
