#ifndef THREADCOLLECTIONS_H
#define THREADCOLLECTIONS_H

#include <QThread>

#define MAX_THREADS_LIMIT 100

/**
 * @brief The ThreadCollections class
 * Потокобезопасная коллекция потоков. Сами потоки инициируются только по мере необходимости
 * Есть некое количество потоков ограниченное Limit
 */
class ThreadCollections
{
    void createThread();
    QList<QThread*> currentThreads{};
    int pos = 0;///<Текущая позиция потока
    int limit = 0;///<Максимальное количество подключений
    QString objectName;///<Текущее имя потока
public:
    explicit ThreadCollections(int curLimit = MAX_THREADS_LIMIT, QString objectName = "Unknown");
    ~ThreadCollections();
    QThread* getThread();
    QThread* getCurrentThread() const;
    int getThreadPosition() const;

};

#endif // THREADCOLLECTIONS_H
