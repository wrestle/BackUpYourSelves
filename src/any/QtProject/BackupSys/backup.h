#ifndef BACKUP_H
#define BACKUP_H
#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDate>
#include <QQueue>

struct combine{
    combine(const QString arg1, const QString arg2):fp(arg1), tp(arg2){}
    QString fp;
    QString tp;
};

class BackUp : public QObject
{
    Q_OBJECT
public:
    BackUp();
    void receive_path_fromfront(const QString fp, const QString tp){
        from_path = fp;
        to_path   = tp;
    }
    bool start_bp(const QString localfrompath, const QString localtopath);
    void start_cp();
private:
    QString from_path;
    QString to_path;
};
static QQueue<combine> fileVec;
#endif // BACKUP_H
