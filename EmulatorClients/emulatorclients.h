#ifndef EMULATORCLIENTS_H
#define EMULATORCLIENTS_H

#include <QObject>

class EmulatorClients : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorClients(QString address, int port, int countConnection, int period, QObject *parent = nullptr);
private:
    QString m_address;
    int m_port = 0;
    int m_countConnection = 0;
    int m_period = 0;


signals:

};

#endif // EMULATORCLIENTS_H
