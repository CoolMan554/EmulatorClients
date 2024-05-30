#ifndef THREADCOLLECTIONS_H
#define THREADCOLLECTIONS_H

#include <QThread>

#define MAX_THREADS_LIMIT 100

class ThreadCollections
{

    void createThread();
    QList<QThread*> currentThreads{};
    int pos = 0;///<Текущая позиция потока
    int limit = 0;///<Максимальное количество подключений
    QString objectName;
public:
    explicit ThreadCollections(int curLimit = MAX_THREADS_LIMIT, QString objectName = "Unknown");
    ~ThreadCollections();
    QThread* getThread();
    QThread* getCurrentThread() const;

};

#endif // THREADCOLLECTIONS_H
