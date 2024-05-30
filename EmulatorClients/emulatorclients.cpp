#include "emulatorclients.h"

EmulatorClients::EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent)
    : threads {countConnection, "Client"}, QObject{parent}
{
    createClients(address, port, countConnection, period);
}

EmulatorClients::~EmulatorClients()
{
    foreach(auto curClient, curClients)
        curClient->deleteLater();
}

void EmulatorClients::createClients(const QString address, const int port, const int countConnection, const int period)
{
    for(int i = 0; i < countConnection; ++i)
    {
        Clients *client = new Clients(address, port, threads.getThread());
        curClients.append(client);
    }
}
