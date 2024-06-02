#include "clients.h"

quint32 Clients::totalNumberConnected{0};
quint32 Clients::totalNumberDisconnected{0};

Clients::Clients(const QString address, const int port, double period, QThread *curThread, QObject *parent)
    : c_address{address}, c_port{port}, c_period{period}, QObject{parent}
{
    moveToThread(curThread);
    if(curThread->objectName().size())
    {
        QChar tempChar = curThread->objectName().at(curThread->objectName().size() - 1);
        bool ok;
        messageId = QString(tempChar).toUInt(&ok, 16) * 1000;//Получаем уникальный id для каждого сообщения из имени потока
    }
    QMetaObject::invokeMethod(this, &Clients::Init, Qt::QueuedConnection);
}

Clients::~Clients()
{
    if(tcpSocket)
    {
        tcpSocket->abort();
        tcpSocket->close();
        delete tcpSocket;
        tcpSocket = nullptr;
    }
    if(curTimer)
    {
        delete curTimer;
        curTimer = nullptr;
    }
    if(reconnectTimer)
    {
        delete reconnectTimer;
        reconnectTimer = nullptr;
    }
}

bool Clients::checkIsConnect()
{
    return tcpSocket->state() == QTcpSocket::ConnectedState;
}

void Clients::connectToServer()
{
    if(tcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        tcpSocket->connectToHost(c_address, c_port);
        if(tcpSocket->waitForConnected(timeoutServer * 1000))
        {
            totalNumberConnected++;
            qDebug() << "Connected to Server" << c_address << c_port;
        }
        else
        {
            qDebug() << this->objectName() << "Clients::connectToServer::Unconnected to Server. Address:" << c_address << "Port:" << c_port;
            tcpSocket->disconnectFromHost();
            qDebug() << this->objectName() << "Clients::connectToServer::Повторное подключение к серверу через" << reconnectToServer << "сек";
            reconnectTimer->start(reconnectToServer * 1000);
        }
    }
}

void Clients::Init()
{
    tcpSocket = new QTcpSocket(this);
    reconnectTimer = new QTimer();
    reconnectTimer->setSingleShot(true);
    connect(reconnectTimer, &QTimer::timeout, this, &Clients::connectToServer);
    connectToServer();
    curTimer = new QTimer();
    curTimer->setSingleShot(false);
    c_period = c_period * 1000 * (0.8 + QRandomGenerator::global()->generateDouble() * 0.4);//Отклонение +- 20%
    curTimer->setInterval(c_period);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Clients::readMessage);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Clients::reconnecting, Qt::DirectConnection);
    connect(curTimer, &QTimer::timeout, this, &Clients::sendMessage);
    curTimer->start();
}

void Clients::sendMessage()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)//Учитываем, что сокет установил соединение
    {
        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);
        QByteArray message = "Сообщение отправлено серверу";
        out << quint16(0) << messageId++ << message;//Первые 2 байта резервируется для размера блока
        out.device()->seek(0);
        out << quint16(Data.size() - sizeof(quint16));//Запись размера блока
        tcpSocket->write(Data);
        tcpSocket->flush();
    }
}

void Clients::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_15);
    if(in.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(tcpSocket->bytesAvailable() < 2)//Данные не могут быть меньше 2 байт
                    break;
                in >> nextBlockSize;//Размер блока
            }
            if(tcpSocket->bytesAvailable() < nextBlockSize)//Данные пришли не полностью
                break;
            QString message;
            in >> message;
            nextBlockSize = 0;
            qDebug() << this->objectName() << "Clients::readMessage:" << message;
        }
    }
    else
    {
        qDebug() << this->objectName() << "Clients::readMessage:: Read Error QDataStream";
    }
}

void Clients::reconnecting()
{
    totalNumberDisconnected++;
    qDebug() << this->objectName() << "Clients::connectToServer::Повторное подключение к серверу через " << reconnectToServer << "сек";
    reconnectTimer->start(reconnectToServer * 1000);
}
