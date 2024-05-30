#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>

class Clients : public QObject
{
    Q_OBJECT
public:
    explicit Clients(const QString address, const int port, const int countConnection, const int period, QObject *parent = nullptr);
private:
    QString c_address;
    int c_port = 0;
    int c_countConnection = 0;
    int c_period = 0;

signals:

};

#endif // CLIENTS_H
