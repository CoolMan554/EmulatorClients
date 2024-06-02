#include "clients.h"

quint32 Clients::totalNumberConnected{0};
quint32 Clients::totalNumberDisconnected{0};

Clients::Clients(const QString address, const int port, double period, QThread *curThread, int messageId, QObject *parent)
    : c_address{address}, c_port{port}, c_period{period},  QObject{parent}
{
    moveToThread(curThread);
    this->messageId = ((quint32)messageId - 1) * 1000;//Получаем уникальный id для каждого сообщения из имени потока
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
        curTimer->stop();
        delete curTimer;
        curTimer = nullptr;
    }
    if(reconnectTimer)
    {
        reconnectTimer->stop();
        delete reconnectTimer;
        reconnectTimer = nullptr;
    }
}

bool Clients::checkIsConnect()
{
    return tcpSocket->state() == QTcpSocket::ConnectedState;
}

void Clients::generateRandomData(QByteArray &data, int size)
{
    data.resize(size);
    QRandomGenerator::global()->fillRange(reinterpret_cast<quint32*>(data.data()), size / sizeof(quint32));
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
            qDebug() << this->thread()->objectName() << "Clients::connectToServer::Unconnected to Server. Address:" << c_address << "Port:" << c_port;
            tcpSocket->disconnectFromHost();
            qDebug() << this->thread()->objectName() << "Clients::connectToServer::Повторное подключение к серверу через" << reconnectToServer << "сек";
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
        // Сгенерировать случайные данные
        QByteArray messageData;
        generateRandomData(messageData, 16384);
        out.setVersion(QDataStream::Qt_5_15);
        out << quint16(0) << messageId++ << messageData;//Первые 2 байта резервируется для размера блока
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
    quint16 nextBlockSize{0};///<Размер блока данных от сервера
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
            QByteArray message;
            in >> message;
            qDebug() << this->thread()->objectName() << "Clients::readMessage:" << QString::fromUtf8(message);
        }
    }
    else
    {
        qDebug() << this->thread()->objectName() << "Clients::readMessage:: Read Error QDataStream";
    }
}

void Clients::reconnecting()
{
    totalNumberDisconnected++;
    qDebug() << this->thread()->objectName() << "Clients::connectToServer::Повторное подключение к серверу через" << reconnectToServer << "сек";
    reconnectTimer->start(reconnectToServer * 1000);
}
