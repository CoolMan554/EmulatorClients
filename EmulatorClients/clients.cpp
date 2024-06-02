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
    if(networkProtocol)
    {
        delete networkProtocol;
        networkProtocol = nullptr;
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
    data.clear();
    data.resize(size);
    QRandomGenerator::global()->fillRange(reinterpret_cast<quint32*>(data.data()), size / sizeof(quint32));
}

void Clients::generateRandomString(QString &str, int size)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789";
    int charsetSize = sizeof(charset) - 1;
    size = QRandomGenerator::global()->bounded(size);//Выбираем рандомный размер сообщения
    str.clear();
    str.reserve(size);

    for(int i = 0; i < size; i++)
    {
        int index = QRandomGenerator::global()->bounded(charsetSize);//Выбираем рандомный символ для сообщения
        str.append(charset[index]);
    }
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
    networkProtocol = new NetworkProtocol;
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
    if(networkProtocol)
    {
        if(tcpSocket->state() == QAbstractSocket::ConnectedState)//Учитываем, что сокет установил соединение
        {
            Data.clear();
            QDataStream out(&Data, QIODevice::WriteOnly);
            // Сгенерировать случайные данные
            QByteArray messageData;
            messageData.clear();
            QString messageStr;
            generateRandomString(messageStr, highSize / 2);
            messageData = messageStr.toUtf8();
            tcpSocket->write(networkProtocol->prepareMessage(messageData, messageId++));
            tcpSocket->flush();
        }
    }
    else qDebug() << this->thread()->objectName() << "Clients::readMessage:: networkProtocol is not valid";
}

void Clients::readMessage()
{
    if(networkProtocol)
    {
        QByteArray message = networkProtocol->processMessage(tcpSocket);
        if(message.size())
        {
            qDebug() << this->thread()->objectName() << "Clients::readMessage:" << QString::fromUtf8(message);
        }
        else qDebug() << this->thread()->objectName() << "Clients::readMessage:: Read Error QDataStream";
    }
    else
    {
        qDebug() << this->thread()->objectName() << "Clients::readMessage:: networkProtocol is not valid";
    }
}

void Clients::reconnecting()
{
    totalNumberDisconnected++;
    qDebug() << this->thread()->objectName() << "Clients::connectToServer::Повторное подключение к серверу через" << reconnectToServer << "сек";
    reconnectTimer->start(reconnectToServer * 1000);
}
