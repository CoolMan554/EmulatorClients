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
    static quint32 totalNumberConnected;///<Общее количество подключений
    static quint32 totalNumberDisconnected;///<Общее количество отключений
    explicit Clients(const QString address, const int port, double period, QThread* curThread, int messageId, QObject *parent = nullptr);
    ~Clients();
    bool checkIsConnect();
private:
    QString c_address;
    int c_port = 0;
    double c_period = 0;
    QTcpSocket *tcpSocket{};
    QTimer *curTimer{};
    QTimer *reconnectTimer{};
    quint32 messageId{0};///<Уникальный id для каждого потока
    QByteArray Data;///Для отправки сообщение серверу    
    const int timeoutServer = 2;///<Таймаут сервера
    const int reconnectToServer = 2;///<Повторное подключение к серверу

    void generateRandomData(QByteArray &data, int size);

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
