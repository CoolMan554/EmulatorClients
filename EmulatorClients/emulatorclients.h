#ifndef EMULATORCLIENTS_H
#define EMULATORCLIENTS_H

#include <QObject>

class EmulatorClients : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorClients(QString address, int port, int countConnection, int period, QObject *parent = nullptr);
private:
    QString m_address = "127.0.0.1";
    int m_port;
    int m_countConnection = 100;
    int m_period = 1;


signals:

};

#endif // EMULATORCLIENTS_H
