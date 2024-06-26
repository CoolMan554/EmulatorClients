#include <QDebug>
#include "emulatorclients.h"

EmulatorClients::EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent)
    : threads {countConnection, "Client"}, QObject{parent}
{
    createClients(address, port, countConnection, period);
    currentTime = new QTimer{};
    currentTime->setSingleShot(false);
    currentTime->setInterval(10 * 1000);
    connect(currentTime, &QTimer::timeout, this, &EmulatorClients::logStatus);
    currentTime->start();
}

EmulatorClients::~EmulatorClients()
{
    foreach(auto curClient, curClients)
        curClient->deleteLater();
    if(currentTime)
    {
        currentTime->stop();
        delete currentTime;
        currentTime = nullptr;
    }
}

void EmulatorClients::createClients(const QString address, const int port, const int countConnection, const double period)
{
    for(int i = 0; i < countConnection; ++i)
    {
        if(threads.getThread())
        {
            Clients *client = new Clients(address, port, period, threads.getCurrentThread(), threads.getThreadPosition());
            curClients.append(client);
        }
        else
        {
            qDebug() << "EmulatorClients::createClients::Лимит создание потоков превышен";
            break;
        }
    }
}

void EmulatorClients::logStatus()
{
    quint32 currentCountConnected = 0;//Текущее количество подключений
    quint32 totalCountConnected = 0;//Общее количество подключений
    quint32 totalCountDisconnected = 0;//Общее количество отключений

    for(const auto &curClient : curClients)
    {
        if(curClient)
        {
            totalCountConnected = curClient->totalNumberConnected;
            totalCountDisconnected = curClient->totalNumberDisconnected;
            if(curClient->checkIsConnect())
            {
                currentCountConnected++;
            }
        }
    }
    qDebug() << "Текущее количество подключений:" << currentCountConnected << "Общее количество подключений:" << totalCountConnected << "Общее количество отключений:" << totalCountDisconnected;
}
