#include <QGuiApplication>
#include "TaoThreadUser.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    TaoThread::ThreadUser user;
    return app.exec();
}
