#ifndef EMULATORCLIENTS_H
#define EMULATORCLIENTS_H

#include <QObject>
#include <QTimer>
#include "clients.h"
#include "threadcollections.h"

class EmulatorClients : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent = nullptr);
    ~EmulatorClients();
    void createClients(const QString address, const int port, const int countConnection, const double period);
private:
    ThreadCollections threads;///<Потокобезопасная коллекция с потоками
    QList<Clients*> curClients;///<Текущие клиенты
    QTimer *currentTime;///<Таймер для вывода логов эмулятора клиентов

signals:

private slots:
    void logStatus();

};

#endif // EMULATORCLIENTS_H
