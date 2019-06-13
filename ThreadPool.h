#pragma once

#include <QObject>
#include <QRunnable>
#include <QThreadPool>
#include <memory>
#include <functional>


namespace TaoThread {
    using WorkCallback = std::function<bool()>;
    using WorkResultCallback = std::function<void(bool)>;

    Q_DECLARE_METATYPE(WorkCallback);
    Q_DECLARE_METATYPE(WorkResultCallback);
    class ThreadObject : public QObject, public QRunnable
    {
        Q_OBJECT
    public:
        explicit ThreadObject(const WorkCallback &work) : m_workCall(work) {}
        void run() override
        {
            bool ok = m_workCall();
            emit readyResult(ok);
        }
    signals:
        void readyResult(bool);
    private:
        WorkCallback m_workCall;
    };
    class ThreadPool : public QObject
    {
        Q_OBJECT
    public:
        static ThreadPool *getInstance()
        {
            static ThreadPool poll;
            return &poll;
        }
        //workCall in sub thread, resultCall in main thread
        void work(const WorkCallback &workCall, const WorkResultCallback &resultCall)
        {
            ThreadObject *obj = new ThreadObject(workCall);
            obj->setAutoDelete(true);
            connect(obj, &ThreadObject::readyResult, this, resultCall);
            QThreadPool::globalInstance()->start(obj);
        }
    private:
        ThreadPool() {}
    };
}
