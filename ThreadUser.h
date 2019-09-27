#pragma once

#include "ThreadPool.h"
#include "ThreadWorkerController.h"
#include <QDebug>
namespace TaoThread
{
    class ThreadUser : public QObject
    {
        Q_OBJECT
    public:
        ThreadUser(int id)
        {
            m_id = id;

//            ThreadPool::getInstance()->work(
//                [id](){
//                    qWarning() << QThread::currentThreadId() << "do" << id;
//                    return true;
//                },
//                [this, id](bool result){
//                    qWarning() <<QThread::currentThreadId() << "work result:" << result << id;
//                    showId();
//                });
            ThreadController::getInstance()->work(
                [id](){
                    qWarning() << QThread::currentThreadId() << "do" << id;
                    return true;
                },
                [this, id](bool result){
                    qWarning() <<QThread::currentThreadId() << "work result:" << result << id;
                    showId();
                }
                );
        }
        void showId()
        {
            qWarning() << QThread::currentThreadId() << "showID " << m_id;
        }
    private:
        int m_id;
    };
}
