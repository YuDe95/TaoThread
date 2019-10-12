#include <QGuiApplication>
#include "ThreadUser.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qWarning() << "mainthread" << QThread::currentThreadId();
    TaoThread::ThreadUser u1(1), u2(2), u3(3);
    return app.exec();
}
