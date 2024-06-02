#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include <QObject>
#include <QDataStream>
#include <QTcpSocket>
#include <QDebug>
#include "abstractprotocol.h"

class NetworkProtocol : public AbstractProtocol
{
public:
    NetworkProtocol();
    ~NetworkProtocol();
    QByteArray prepareMessage(const QByteArray &data, const quint32 messageId) override;
    QByteArray processMessage(QTcpSocket *tcpSocket) override;
};

#endif // NETWORKPROTOCOL_H
