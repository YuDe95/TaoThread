#pragma once
#include <QObject>

#include "ThreadCommon.h"

#include <QMap>
namespace TaoThread
{
class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    ThreadWorker(int id, const WorkCallback &workCall, QObject *parent = nullptr) : QObject(parent), m_id(id), m_workCall(workCall) {}

signals:
    void workFinished(bool, int);
public slots:
    void doWork()
    {
        bool ret = m_workCall();
        emit workFinished(ret, m_id);
    }
private:
    const int m_id;
    WorkCallback m_workCall;
};
class ThreadController : public QObject
{
    Q_OBJECT
public:
    static ThreadController *getInstance()
    {
        static ThreadController controller;
        return &controller;
    }

    ~ThreadController()
    {
        for (auto k : m_threadMap.uniqueKeys())
        {
            if (m_threadMap.value(k)->isRunning())
            {
                m_threadMap.value(k)->quit();
                m_threadMap.value(k)->wait();
            }
        }
        qDeleteAll(m_threadMap);
        m_threadMap.clear();
        m_resultCallback.clear();
    }
    int work(const WorkCallback &workCall, const WorkResultCallback &resultCall)
    {
        QThread *thread = new QThread;
        m_rollId++;
        ThreadWorker *obj = new ThreadWorker(m_rollId, workCall);
        m_threadMap[m_rollId] = thread;
        m_resultCallback[m_rollId] = resultCall;
        obj->moveToThread(thread);

        connect(thread, &QThread::finished, obj, &QObject::deleteLater);
        connect(thread, &QThread::started, obj, &ThreadWorker::doWork);
        connect(obj, &ThreadWorker::workFinished, this, &ThreadController::onWorkFinished);
        thread->start();
        return m_rollId;
    }
    void cancle(int id)
    {
        auto it = m_threadMap.find(id);
        if (it != m_threadMap.end())
        {
            if ((*it)->isRunning())
            {
                (*it)->terminate();
            }
        }
        m_resultCallback.remove(id);
    }
protected slots :
    void onWorkFinished(bool ok, int id)
        {
        auto it = m_threadMap.find(id);
        if (it != m_threadMap.end())
        {
            if ((*it)->isRunning())
            {
                (*it)->quit();
                (*it)->wait();
                (*it)->deleteLater();
            }
            m_threadMap.remove(id);
        }
        auto caller = m_resultCallback.find(id);
        if (caller != m_resultCallback.end())
        {
            (*caller)(ok);
            m_resultCallback.remove(id);
        }
    }
protected:
    ThreadController(QObject *parent = nullptr) : QObject(parent) {}

private:
    int m_rollId = 0;
    QMap<int, QThread *> m_threadMap;
    QMap<int, WorkResultCallback> m_resultCallback;
};
}
