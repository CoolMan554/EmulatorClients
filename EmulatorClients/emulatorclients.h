#ifndef EMULATORCLIENTS_H
#define EMULATORCLIENTS_H

#include <QObject>

class EmulatorClients : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorClients(const QString address, const int port, const int countConnection, const int period, QObject *parent = nullptr);
    void createClients(const QString address, const int port, const int countConnection, const int period);
private:


signals:

};

#endif // EMULATORCLIENTS_H
