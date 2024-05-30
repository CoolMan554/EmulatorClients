#include "emulatorclients.h"
#include "clients.h"

EmulatorClients::EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent)
    : QObject{parent}
{
    createClients(address, port, countConnection, period);
}

void EmulatorClients::createClients(const QString address, const int port, const int countConnection, const int period)
{
}
