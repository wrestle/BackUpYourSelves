#include "result.h"

Result::Result(const QString & title)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(title);
    QPushButton *start = new QPushButton(this);
    start->setAttribute(Qt::WA_DeleteOnClose);
    start->setText(QStringLiteral("开始拷贝"));
    //QTextBrowser *details = new QTextBrowser(this);
    for(int i = 0;i < 4;++i)
        connect(start, &QPushButton::clicked, copyArr[i], &copyThread::runslot);

}
