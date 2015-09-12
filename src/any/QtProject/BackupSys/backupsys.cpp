#include "backupsys.h"

#include "ui_backupsys.h"

QSemaphore queEmpty(20);
QSemaphore queFull(20);
storeThread *pushThread;
copyThread  *copyArr[4];

unsigned int copyThread::newfiles = 0;

BackupSys::BackupSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BackupSys),
    files(0), dirs(0)
{
    ui->setupUi(this);
    queFull.acquire(20);
    pushThread = new storeThread(this);  // 创建
    for(int i = 0;i < 4;++i)
        copyArr[i] = new copyThread;
    connect(pushThread, &storeThread::finished, pushThread, &QObject::deleteLater); //销毁
    for(int i = 0;i < 4;++i)
        connect(copyArr[i], &copyThread::finished, copyArr[i], &QObject::deleteLater);
}

BackupSys::~BackupSys()
{
    delete ui;
}

bool BackupSys::backup(const QString localfrompath, const QString localtopath)
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

//**************************************************************************************
//******************************** Thread 实现 ******************************************
void copyThread::run()
{
    while(1)
    {
        if(pushThread->isFinished() && fileVec.isEmpty())
            break;
        queFull.acquire();
        locker.lock();
        combine local(fileVec.dequeue());
        ++newfiles;
        locker.unlock();

        QFileInfo fPath(local.curFromPath);
        QFileInfo tPath(local.curToPath);

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
            if(!QFile::copy(local.curFromPath, local.curToPath))
                QMessageBox::critical(NULL, QStringLiteral("警告"),
                                      QStringLiteral("备份文件:") + local.curFromPath + QStringLiteral("失败"));
            queEmpty.release();
        }
    }// while(1)

}// run()

//**************************************************************************************
//********************************* SLOT 实现 *******************************************
void BackupSys::on_fromButton_clicked()
{
    QString fromDirName = QFileDialog::getExistingDirectory(this, tr("选择备份路径"),
                                                        "/", QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);
    if(fromDirName.isEmpty())
        QMessageBox::critical(this, tr("选择有误"),
                              tr("路径无效"));
    else
    {
        this->ui->fromEdit->setText(fromDirName);
    }
}

void BackupSys::on_toButton_clicked()
{
    // 使用文件夹选择对话框获取路径
    QString toDirName = QFileDialog::getExistingDirectory(this, tr("选择备份存储路径"),
                                                        "/", QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);
    if(toDirName.isEmpty())
        QMessageBox::critical(this, tr("选择有误"), tr("路径无效"));
    else
    {
        this->ui->toEdit->setText(toDirName);
    }
}

void BackupSys::on_fromEdit_textChanged(const QString &arg1)
{
#if !defined(NOT_DEBUG_AT_ALL)
    QMessageBox::about(this, QStringLiteral("你好"),
                       QStringLiteral("fromPath有值变了"));
#endif
    fromPath = arg1;
}

void BackupSys::on_toEdit_textChanged(const QString &arg1)
{
#if !defined(NOT_DEBUG_AT_ALL)
    QMessageBox::about(this, QStringLiteral("你好"),
                       QStringLiteral("toPath有值变了"));
#endif
    toPath = arg1;
}

void BackupSys::on_startButton_clicked()
{
    if(this->ui->toEdit->text().isEmpty() ||
            this->ui->fromEdit->text().isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("路径为空！"),
                              QStringLiteral("请确认填充路径！"));
        return;
    }
    // 建立新对话框
    QDialog *copyWindows = new QDialog(this);
    QHBoxLayout * layout = new QHBoxLayout;
    copyWindows->setAttribute(Qt::WA_DeleteOnClose);
    copyWindows->setWindowTitle(QStringLiteral("拷贝文件"));
    QPushButton *starts = new QPushButton(copyWindows);
    layout->addWidget(starts);
    copyWindows->setLayout(layout);
    starts->setAttribute(Qt::WA_DeleteOnClose);
    starts->setText(QStringLiteral("点我开始"));
    connect(starts, &QPushButton::clicked,
            [=]() {
        starts->setText(QStringLiteral("正在拷贝，请稍后"));
        starts->setEnabled(false);
    });

    pushThread->start();
    for(int i = 0;i < 4;++i)
    {
        connect(starts, &QPushButton::clicked, copyArr[i], &copyThread::runslot);
        connect(copyArr[i], &copyThread::finished,
                [=](){
            starts->setText(QStringLiteral("拷贝完成！"));
        });
    }
    copyWindows->exec();
    fileVec.clear();
}
//***********************************************************************************
