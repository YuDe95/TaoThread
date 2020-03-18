#pragma once
#include "ThreadCommon.h"
#include <QObject>

#include <QHash>
namespace TaoThread
{

    class ThreadObject : public QObject, public QRunnable
    {
        Q_OBJECT
    public:
        explicit ThreadObject(const WorkCallback &work, uint32_t id) : m_workCall(work), m_id(id) {}
        void run() override
        {
            bool ok = m_workCall();
            emit readyResult(ok);
            emit finished(m_id);
        }

    signals:
        void readyResult(bool);
        void finished(uint32_t);
    private:
        WorkCallback m_workCall;
        uint32_t m_id;
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
        uint32_t work(const WorkCallback &workCall, const WorkResultCallback &resultCall)
        {
            ThreadObject *obj = new ThreadObject(workCall,m_rollId);
            m_objMap[m_rollId] = obj;
            obj->setAutoDelete(true);

            connect(obj, &ThreadObject::readyResult, this, resultCall);
            connect(obj, &ThreadObject::finished, this, &ThreadPool::onFinished);
            QThreadPool::globalInstance()->start(obj);
            m_rollId++;
            return m_rollId - 1;
        }
        bool cancel(uint32_t id)
        {
            if (m_objMap.contains(id))
            {
                return m_objMap.remove(id);
            }
            return false;
        }
    protected slots:
        void onFinished(uint32_t id)
        {
            if (m_objMap.contains(id))
            {
                m_objMap.remove(id);
            }
        }
    private:
        ThreadPool() {}
    private:
        uint32_t m_rollId = 0;
        QHash<uint32_t, ThreadObject *> m_objMap;
    };

    }
