#include "maindesktop.h"
#include "ui_maindesktop.h"
#ifndef NOT_DEBUG_AT_ALL
#include <QDebug>
#endif

MainDesktop::MainDesktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainDesktop)
{
    ui->setupUi(this);
    this->setWindowTitle("BackupSystem");
    QObject::connect(ui->aboutApp, &QAction::triggered, [](){ // 创建窗口显示信息
        QFile readme("readme");
        readme.open(QFile::ReadOnly | QFile::Text);
        QTextStream reading(&readme);
        QWidget * show_readme = new QWidget;
        show_readme->setWindowTitle(QStringLiteral("使用说明"));
        QTextBrowser * browse_readme = new QTextBrowser(show_readme);
        browse_readme->setText(reading.readAll());
        QHBoxLayout * layout = new QHBoxLayout(show_readme);
        layout->addWidget(browse_readme);
        show_readme->show();
    });
    QObject::connect(ui->errorApp, &QAction::triggered, [=](){
        QFile error_log("CopyErrorLog");
        QWidget * show_errorlog = new QWidget;
        show_errorlog->setWindowTitle(QStringLiteral("错误信息"));
        QTextBrowser * browse_errorlog = new QTextBrowser(show_errorlog);
        QHBoxLayout * layout = new QHBoxLayout(show_errorlog);
        layout->addWidget(browse_errorlog);
        if(!error_log.open(QFile::ReadOnly | QFile::Text))
        {
              browse_errorlog->setText("Empty, No Such Infomation");
        }
        else
        {
            QTextStream reading(&error_log);
            browse_errorlog->setText(reading.readAll());
        }
        show_errorlog->show();
    });
    QObject::connect(ui->InfoApp, &QAction::triggered, [](){
        QFile success_log("CopySuccessLog");
        QWidget * show_succeed_log = new QWidget;
        show_succeed_log->setWindowTitle(QStringLiteral("备份信息"));
        QTextBrowser * browse_succeed_log = new QTextBrowser(show_succeed_log);
        QHBoxLayout * layout = new QHBoxLayout(show_succeed_log);
        layout->addWidget(browse_succeed_log);
        if(!success_log.open(QFile::ReadOnly | QFile::Text))
        {
            browse_succeed_log->setText("Empty, No Such Infomation");
        }
        else
        {
            QTextStream reading(&success_log);
            browse_succeed_log->setText(reading.readAll());
        }
        show_succeed_log->show();
    });
}

MainDesktop::~MainDesktop()
{
    delete ui;
}

void MainDesktop::receive_thread_cp_done(int local_number){ // it receive "thread_cp_done()"'s signal
    QMessageBox::about(this, QStringLiteral("新增或被修改的文件数量"),
                       QStringLiteral("数量： ")+QString::number(local_number));

    emit thread_cp_done();
}


//******************************************************************************************
//*********************************** SLOT 实现 *********************************************
void MainDesktop::on_exitButton_clicked()
{
    qApp->exit();
}

/**
 * @brief MainDesktop::on_editfromPath_textChanged
 * 一旦 LineEdit的值改变，则将其保存
 */
void MainDesktop::on_editfromPath_textChanged(const QString &arg1)
{
    //QMessageBox::about(this, "Show", arg1);
    fromPath = arg1;
}

/**
 * @brief MainDesktop::on_fromTool_clicked
 * 为LineEdit添加路径值
 */
void MainDesktop::on_fromTool_clicked()
{
    QString fromDirName = QFileDialog::getExistingDirectory(this, QStringLiteral("选择来源路径"),
                                                            "/", QFileDialog::ShowDirsOnly |
                                                            QFileDialog::DontResolveSymlinks);
        if(fromDirName.isEmpty()) // 如果取消了对话框
            QMessageBox::critical(this, QStringLiteral("选择有误"), QStringLiteral("路径无效"));
        else // 如果选择正确
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

/**
 * @brief MainDesktop::on_startButton_clicked
 * 开始备份按钮
 */
void MainDesktop::on_startButton_clicked()
{
    if(fromPath.isEmpty() || toPath.isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("警告"),
                              QStringLiteral("请填充路径"));
        return;
    }
    else
        emit send_path_toback(fromPath, toPath);
    ui->startButton->setDisabled(true);
#if !defined(NOT_DEBUG_AT_ALL)
    qDebug() << " >>>>>>>>>>>>>After emit send_path_toback() ";
#endif
    QWidget *subwindow = new QWidget;
    QPushButton * sub_start = new QPushButton(subwindow);
    QHBoxLayout * layouts = new QHBoxLayout(subwindow);
    layouts->addWidget(sub_start);
    subwindow->setLayout(layouts);
    sub_start->setText(QStringLiteral("点我开始多线程备份"));
    subwindow->setWindowTitle(QStringLiteral("备份窗口"));
#if !defined(NOT_DEBUG_AT_ALL)
    qDebug() << " >>>>>>>>>>>>After Initialize the new Windows";
#endif
    connect(subwindow, &QWidget::close, subwindow, &QWidget::deleteLater);
    connect(this, &MainDesktop::thread_cp_done, [=](){    // 如果拷贝线程结束了，就通知用户
        sub_start->setText(QStringLiteral("备份完成！"));
    });
    connect(sub_start, &QPushButton::clicked, [=]() {     // 一旦开始备份
        sub_start->setEnabled(false);
        sub_start->setText(QStringLiteral("正在备份请稍后..."));
        told_thread_cp();
    });
    this->told_thread_bp();
    subwindow->show();
    ui->startButton->setDisabled(false);
#if !defined(NOT_DEBUG_AT_ALL)
    qDebug() << " >>>>>>>>>>>>>>After connect to show()";
#endif
}

