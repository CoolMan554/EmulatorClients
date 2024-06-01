#include "clients.h"
#include <QRandomGenerator>

Clients::Clients(const QString address, const int port, double period, QThread *curThread, QObject *parent)
    : c_address{address}, c_port{port}, c_period{period}, QObject{parent}
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

void Clients::connectToServer()
{
    tcpSocket->connectToHost(c_address, c_port);
    if(tcpSocket->waitForConnected(2000))
    {
        qDebug() << "Connected to Server" << c_address << c_port;
    }
    else
    {
        emit Disconnect();
    }
}

void Clients::Init()
{
    tcpSocket = new QTcpSocket(this);
    connectToServer();
    curTimer = new QTimer();
    curTimer->setSingleShot(false);
    c_period = c_period * 1000 * (0.8 + QRandomGenerator::global()->generateDouble() * 0.4);
    curTimer->setInterval(c_period);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Clients::readMessage);
    connect(this, &Clients::Disconnect, this, &Clients::HandleDisconnect);
    connect(curTimer, &QTimer::timeout, this, &Clients::sendMessage);
    curTimer->start();
}

void Clients::sendMessage()
{

}

void Clients::readMessage()
{

}

void Clients::HandleDisconnect()
{
    tcpSocket->abort();
    tcpSocket->close();
}
