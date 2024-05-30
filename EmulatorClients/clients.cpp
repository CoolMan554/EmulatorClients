#include "clients.h"

Clients::Clients(const QString address, const int port, QThread *curThread, QObject *parent)
    : c_address{address}, c_port{port}, QObject{parent}
{
    moveToThread(curThread);
    QMetaObject::invokeMethod(this, &Clients::Init, Qt::QueuedConnection);
}

Clients::~Clients()
{

}

bool Clients::checkIsConnect()
{
    return false;
}

void Clients::Init()
{

}
