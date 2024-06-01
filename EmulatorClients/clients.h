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
    explicit Clients(const QString address, const int port, double period, QThread* curThread, QObject *parent = nullptr);
    ~Clients();
    bool checkIsConnect();
private:
    QString c_address;
    int c_port = 0;
    double c_period = 0;
    QTcpSocket *tcpSocket;
    QTimer *curTimer;
    quint16 nextBlockSize;///<Размер блока данных от сервера
    quint32 messageId{0};///<Уникальный id для каждого сообщения
    QByteArray Data;///Для отправки сообщение серверу
    void connectToServer();

private slots:
    void Init();
    void sendMessage();
    void readMessage();
    void HandleDisconnect();

signals:
    void Disconnect();
};

#endif // CLIENTS_H
