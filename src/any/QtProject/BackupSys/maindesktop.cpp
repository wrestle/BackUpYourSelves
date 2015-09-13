#include "maindesktop.h"
#include "ui_maindesktop.h"

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
    if(fromPath.isEmpty() || toPath.isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("警告"),
                              QStringLiteral("请填充路径"));
        return;
    }
}
