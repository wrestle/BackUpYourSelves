#include "maindesktop.h"
#include "ui_maindesktop.h"
#include <QDebug>

MainDesktop::MainDesktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDesktop)
{
    ui->setupUi(this);
}

MainDesktop::~MainDesktop()
{
    delete ui;
}


//******************************************************************************************
//*********************************** SLOT 实现 *********************************************
void MainDesktop::on_exitButton_clicked()
{
    qApp->exit();
}

void MainDesktop::on_editfromPath_textChanged(const QString &arg1)
{
    //QMessageBox::about(this, "Show", arg1);
    fromPath = arg1;
}
void MainDesktop::on_fromTool_clicked()
{
    QString fromDirName = QFileDialog::getExistingDirectory(this, QStringLiteral("选择来源路径"),
                                                            "/", QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks);
        if(fromDirName.isEmpty())
            QMessageBox::critical(this, QStringLiteral("选择有误"), QStringLiteral("路径无效"));
        else
        {
            ui->editfromPath->setText(fromDirName);
        }
        return;
}

void MainDesktop::on_edittoPath_textChanged(const QString &arg1)
{
    toPath = arg1;
}
void MainDesktop::on_toTool_clicked()
{
    QString toDirName = QFileDialog::getExistingDirectory(this, QStringLiteral("选择目的路径"),
                                                            "/", QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks);
        if(toDirName.isEmpty())
            QMessageBox::critical(this, QStringLiteral("选择有误"), QStringLiteral("路径无效"));
        else
        {
            ui->edittoPath->setText(toDirName);
        }
        return;
}

void MainDesktop::on_startButton_clicked()
{
    //int avail = queFull.available();
    //queFull.acquire(avail);

    if(fromPath.isEmpty() || toPath.isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("警告"),
                              QStringLiteral("请填充路径"));
        return;
    }
    else
        emit send_path_toback(fromPath, toPath);

    qDebug() << " >>>>>>>>>>>>>After emit send_path_toback() ";
    QWidget *subwindow = new QWidget;
    //QDialog *subwindows = new QDialog(this);
    QPushButton * sub_start = new QPushButton(subwindow);
    QHBoxLayout * layouts = new QHBoxLayout;
    layouts->addWidget(sub_start);
    subwindow->setLayout(layouts);
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    sub_start->setAttribute(Qt::WA_DeleteOnClose);
    sub_start->setText(QStringLiteral("点我开始多线程备份"));
    subwindow->setWindowTitle(QStringLiteral("备份窗口"));
    qDebug() << " >>>>>>>>>>>>After Initialize the new Windows";
    connect(this, &MainDesktop::thread_cp_done, [=](){
        sub_start->setText(QStringLiteral("备份完成！"));
    });
    connect(sub_start, &QPushButton::clicked, [=]() {
        sub_start->setEnabled(false);
        sub_start->setText(QStringLiteral("正在备份请稍后..."));
        told_thread_cp();
    });
    qDebug() << " >>>>>>>>>>>>>Before Connect to show()";
    //connect(sub_start, &QPushButton::clicked, this, &MainDesktop::told_thread_cp);
    //connect(ui->startButton, &QPushButton::clicked, this, &MainDesktop::told_thread_bp);
    //connect(ui->startButton, &QPushButton::clicked, subwindow, &QWidget::show);
    this->told_thread_bp();
    subwindow->show();
    qDebug() << " >>>>>>>>>>>>>>After connect to show()";
}
