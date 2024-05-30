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
    if(pos >= limit)
        pos = 0;
    if(pos >= currentThreads.count())
        createThread();
    return currentThreads[pos++];
}

QThread *ThreadCollections::getCurrentThread() const
{
    return currentThreads[pos];
}

void ThreadCollections::createThread()
{
    QThread *t = new QThread();
    t->setObjectName(QString("%1#%2").arg(objectName).arg(pos));
    currentThreads.append(t);
    t->start();
}
