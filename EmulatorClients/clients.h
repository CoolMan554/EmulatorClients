#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QThread>
#include <QRandomGenerator>

class Clients : public QObject
{
    Q_OBJECT
public:
    static quint32 totalNumberConnected;
    static quint32 totalNumberDisconnected;
    explicit Clients(const QString address, const int port, double period, QThread* curThread, QObject *parent = nullptr);
    ~Clients();
    bool checkIsConnect();
private:
    QString c_address;
    int c_port = 0;
    double c_period = 0;
    QTcpSocket *tcpSocket{};
    QTimer *curTimer{};
    QTimer *reconnectTimer{};
    quint16 nextBlockSize{0};///<Размер блока данных от сервера
    quint32 messageId{0};///<Уникальный id для каждого сообщения
    QByteArray Data;///Для отправки сообщение серверу    
    const int timeoutServer = 2;///<Таймауит сервера
    const int reconnectToServer = 2;///<Повторное подключение к серверу

private slots:
    void Init();
    void sendMessage();
    void readMessage();
    void reconnecting();
    void connectToServer();

signals:
    void Disconnect();
};

#endif // CLIENTS_H
