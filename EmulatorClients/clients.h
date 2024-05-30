#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QThread>

class Clients : public QObject
{
    Q_OBJECT
public:
    explicit Clients(const QString address, const int port, QThread* curThread, QObject *parent = nullptr);
    ~Clients();
    bool checkIsConnect();
private:
    QString c_address;
    int c_port = 0;

private slots:
    void Init();

signals:

};

#endif // CLIENTS_H
