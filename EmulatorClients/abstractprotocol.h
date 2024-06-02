#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H

#include <QObject>
#include <QTcpSocket>

/**
 * @brief The AbstractProtocol class
 * Является абстрактным базовым классом, который определяет интерфейс для протоколов обмена сообщениями
 */
class AbstractProtocol
{
public:
    AbstractProtocol();
    virtual ~AbstractProtocol() = default;

    /**
     * @brief prepareMessage
     * @param data
     * @param messageId
     * Подготавливает сообщение для отправки, добавляя к нему уникальный идентификатор сообщения
     * @return
     */
    virtual QByteArray prepareMessage(const QByteArray &data, const quint32 messageId) = 0;
    /**
     * @brief processMessage
     * @param tcpSocket
     * Обрабатывает входящее сообщение, полученное из сокета TCP
     * @return
     */
    virtual QByteArray processMessage(QTcpSocket *tcpSocket) = 0;
};

#endif // ABSTRACTPROTOCOL_H
