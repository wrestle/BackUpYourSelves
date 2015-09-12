#ifndef RESULT_H_INCLUDE
#define RESULT_H_INCLUDE
#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>

extern storeThread *pushThread;
extern copyThread  *copyArr[4];

class Result : public QDialog
{
    Q_OBJECT
public:
     Result(const QString & title);
private:

};

#endif // RESULT

