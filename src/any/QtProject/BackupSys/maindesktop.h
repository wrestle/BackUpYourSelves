#ifndef MAINDESKTOP_H
#define MAINDESKTOP_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class MainDesktop;
}

class MainDesktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainDesktop(QWidget *parent = 0);
    ~MainDesktop();

    void told_thread_bp(){ // 作为startButton clicked 的槽函数，发送信号。信号由main中
        emit thread_bp_start(); //pushThread 的 run_slot 槽函数接收。-
    }

    void told_thread_cp(){
        emit thread_cp_start();
    }

    void receive_thread_cp_done(){
        emit thread_cp_done();
    }

signals:
    void send_path_toback(const QString from_path, const QString to_path);
    void thread_bp_start();
    void thread_cp_start();
    void thread_cp_done();
private slots:
    void on_exitButton_clicked();

    void on_editfromPath_textChanged(const QString &arg1);

    void on_fromTool_clicked();

    void on_edittoPath_textChanged(const QString &arg1);

    void on_toTool_clicked();

    void on_startButton_clicked();

private:
    Ui::MainDesktop *ui;
    QString fromPath;
    QString toPath;
};

#endif // MAINDESKTOP_H
