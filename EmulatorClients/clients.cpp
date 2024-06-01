#include "clients.h"

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
    delete tcpSocket;
    delete curTimer;
}

bool Clients::checkIsConnect()
{
    return tcpSocket->state() == QTcpSocket::ConnectedState;
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
    c_period = c_period * 1000 * (0.8 + QRandomGenerator::global()->generateDouble() * 0.4);//Отклонение +- 20%
    curTimer->setInterval(c_period);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Clients::readMessage);
    connect(this, &Clients::Disconnect, this, &Clients::HandleDisconnect);
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
            qDebug() << "Clients::readMessage:" << message;
        }
    }
    else
    {
        qDebug() << "Clients::readMessage:: Read Error QDataStream";
    }
}

void Clients::HandleDisconnect()
{
    tcpSocket->abort();
    tcpSocket->close();
}
