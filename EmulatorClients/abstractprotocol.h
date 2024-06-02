#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H

#include <QObject>
#include <QTcpSocket>

class AbstractProtocol
{
public:
    AbstractProtocol();
    virtual ~AbstractProtocol() = default;

    virtual QByteArray prepareMessage(const QByteArray &data, const quint32 messageId) = 0;
    virtual QByteArray processMessage(QTcpSocket *tcpSocket) = 0;
};

#endif // ABSTRACTPROTOCOL_H
