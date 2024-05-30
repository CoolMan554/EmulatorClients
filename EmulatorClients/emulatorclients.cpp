#include "emulatorclients.h"

EmulatorClients::EmulatorClients(QString address, int port, int countConnection, int period, QObject *parent)
    : QObject{parent}
{
    m_address = address;
    m_port = port;
    m_countConnection = countConnection;
    m_period = period;
}
