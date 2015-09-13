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
