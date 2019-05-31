#pragma once

#include <QThread>
#include <QObject>

#include <memory>
#include <functional>

namespace TaoThread {
    
    using WorkerCallback = std::function<bool()>;

    Q_DECLARE_METATYPE(WorkerCallback)

    class ThreadObject : public QObject
    {
        Q_OBJECT
    public:

    public slots:
        void doWork(const WorkerCallback& callback)
        {
            bool ret = callback();
            emit readyWork(ret);
        }
    signals:
        void readyWork(bool result);
    };
    class ThreadController : public QObject 
    {
        Q_OBJECT
    public:
        ThreadController() 
        {
            qRegisterMetaType<WorkerCallback>();
            m_pThreadObject = new ThreadObject;
            m_pThreadObject->moveToThread(&m_thread);
            connect(&m_thread, &QThread::finished, m_pThreadObject, &QObject::deleteLater);
            connect(m_pThreadObject, &ThreadObject::readyWork, this, &ThreadController::readyWork);
            connect(this, &ThreadController::doWork, m_pThreadObject, &ThreadObject::doWork);
        }
        ~ThreadController()
        {
            if (m_thread.isRunning()) 
            {
                m_thread.quit();
                m_thread.wait();
            }
        }
    signals:
        //for thread
        void doWork(const WorkerCallback& callback);

        //for caller
        void readyWork(bool result);
    public slots:
        void work(const WorkerCallback& callback)
        {
            if (!m_thread.isRunning()) 
            {
                m_thread.start();
            }
            emit doWork(callback);
        }
    private:
        QThread m_thread;
        ThreadObject* m_pThreadObject = nullptr;
    };

}
