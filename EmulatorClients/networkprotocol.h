#ifndef NETWORKPROTOCOL_H
#define NETWORKPROTOCOL_H

#include <QObject>
#include <QDataStream>
#include <QTcpSocket>
#include <QDebug>
#include "abstractprotocol.h"

/**
 * @brief The NetworkProtocol class
 * Класс является конкретной реализацией абстрактного класса AbstractProtocol, который определяет протокол обмена сообщениями
 */
class NetworkProtocol : public AbstractProtocol
{
public:
    NetworkProtocol();
    ~NetworkProtocol();
    /**
     * @brief prepareMessage
     * @param data
     * @param messageId
     * Подготавливает сообщение для отправки, добавляя к нему уникальный идентификатор сообщения
     * @return
     */
    QByteArray prepareMessage(const QByteArray &data, const quint32 messageId) override;
    /**
     * @brief processMessage
     * @param tcpSocket
     * Обрабатывает входящее сообщение, полученное из сокета TCP
     * @return
     */
    QByteArray processMessage(QTcpSocket *tcpSocket) override;
};

#endif // NETWORKPROTOCOL_H
