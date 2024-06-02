#include "threadcollections.h"

ThreadCollections::ThreadCollections(int curLimit, QString objectName)
    : limit{curLimit}, objectName{objectName}
{
}

ThreadCollections::~ThreadCollections()
{
    foreach(auto curThread, currentThreads)
        curThread->quit();

    foreach(auto curThread, currentThreads)
        curThread->wait();

    qDeleteAll(currentThreads);
}

QThread *ThreadCollections::getThread()
{    
    if(pos < limit)///<В пределах указанного лимита
    {
        createThread();
        return currentThreads[pos++];
    }
    return nullptr;
}

QThread *ThreadCollections::getCurrentThread() const
{
    return currentThreads[pos - 1];
}

int ThreadCollections::getThreadPosition() const
{
    return pos - 1;
}

void ThreadCollections::createThread()
{
    QThread *t = new QThread();
    t->setObjectName(QString("%1#%2").arg(objectName).arg(pos));
    currentThreads.append(t);
    t->start();
}
