#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QThread>

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
