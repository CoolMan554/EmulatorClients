#include "clients.h"

Clients::Clients(const QString address, const int port, const int countConnection, const int period, QObject *parent)
    : c_address{address}, c_port{port}, c_countConnection{countConnection}, c_period{period}, QObject{parent}
{

}
