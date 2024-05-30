#include <QDebug>
#include "emulatorclients.h"

EmulatorClients::EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent)
    : threads {countConnection, "Client"}, QObject{parent}
{
    createClients(address, port, countConnection, period);
    currentTime = new QTimer{};
    currentTime->setSingleShot(false);
    currentTime->setInterval(10 * 1000);///@todo: Период всегда 10 секунд?
    connect(currentTime, &QTimer::timeout, this, &EmulatorClients::logStatus);
    currentTime->start();
}

EmulatorClients::~EmulatorClients()
{
    foreach(auto curClient, curClients)
        curClient->deleteLater();
    delete currentTime;
}

void EmulatorClients::createClients(const QString address, const int port, const int countConnection, const int period)
{
    for(int i = 0; i < countConnection; ++i)
    {
        Clients *client = new Clients(address, port, threads.getThread());
        curClients.append(client);
    }
}

void EmulatorClients::logStatus()
{
    quint32 currentCountConnected = 0;//Текущее количество подключений

    for(const auto &curClient : curClients)
    {
        if(curClient)
        {
            if(curClient->checkIsConnect())
            {
                currentCountConnected++;
            }
            else countDisconnected++;
        }
    }
    countConnected += currentCountConnected;
    qDebug() << "Текущее количество подключений:" << currentCountConnected << "Общее количество подключений:" << countConnected << "Отключений:" << countDisconnected;
}
