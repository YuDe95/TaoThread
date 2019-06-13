#include <QGuiApplication>
#include "ThreadUser.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qWarning() << "mainthread" << QThread::currentThreadId();
    TaoThread::ThreadUser u1(1), u2(2), u3(3), u4(4), u5(5), u6(6), u7(7), u8(8), u9(9), u10(10);
    return app.exec();
}
