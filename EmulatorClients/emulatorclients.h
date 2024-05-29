#ifndef EMULATORCLIENTS_H
#define EMULATORCLIENTS_H

#include <QObject>

class EmulatorClients : public QObject
{
    Q_OBJECT
public:
    explicit EmulatorClients(QObject *parent = nullptr);

signals:

};

#endif // EMULATORCLIENTS_H
