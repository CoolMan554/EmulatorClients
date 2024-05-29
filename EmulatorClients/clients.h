#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>

class Clients : public QObject
{
    Q_OBJECT
public:
    explicit Clients(QObject *parent = nullptr);

signals:

};

#endif // CLIENTS_H
