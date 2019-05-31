#pragma once

#include "TaoThread.h"
#include <QObject>
#include <QDebug>

namespace TaoThread
{
    class ThreadUser : public QObject
    {
        Q_OBJECT
    public:
        ThreadUser()
        {
            qWarning() << QThread::currentThreadId();
            connect(&controller, &ThreadController::readyWork, this, [&](bool ret) {
                qWarning() <<QThread::currentThreadId() << "work result:" << ret;
            });
            controller.work([&]()->bool {
                qWarning() << QThread::currentThreadId() << "do 123";
                return true;
                });
            controller.work([&]()->bool {
                qWarning() << QThread::currentThreadId() << "do 456";
                return false;
                });
        }
    private:
        ThreadController controller;
    };
}