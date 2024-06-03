#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QThread>
#include <QRandomGenerator>
#include "networkprotocol.h"

/**
 * @brief The Clients class
 * Отвечает за управление подключениями к серверу, отправку и прием сообщений через TCP-сокеты с использованием конкретного протокола обмена сообщениями.
 */
class Clients : public QObject
{
    Q_OBJECT
public:
    static quint32 totalNumberConnected;///<Общее количество подключений
    static quint32 totalNumberDisconnected;///<Общее количество отключений
    explicit Clients(const QString address, const int port, double period, QThread* curThread, int messageId, QObject *parent = nullptr);
    ~Clients();
    /**
     * @brief checkIsConnect
     * Проверка текущего подключения к серверу
     * @return
     */
    bool checkIsConnect();
private:
    QString c_address;
    int c_port = 0;
    double c_period = 0.0;
    QTcpSocket *tcpSocket{};
    NetworkProtocol *networkProtocol{};///<Протокол предназначен для формирования сообщения
    QTimer *curTimer{};///<Таймер для отправки сообщения серверу
    QTimer *reconnectTimer{};///<Таймер для повторно подключения к серверу
    quint32 messageId{0};///<Уникальный id для каждого потока
    QByteArray Data;///Для отправки сообщение серверу    
    const int timeoutServer = 2;///<Таймаут сервера
    const int reconnectToServer = 2;///<Повторное подключение к серверу
    const quint16 highSize{16384};///<Максимальный размер бинарного массива для отправки серверу

    /**
     * @brief generateRandomData
     * @param data
     * Генерирует случайные бинарные данные для отправки серверу в пределах верхнего лимита
     * @param size
     */
    void generateRandomData(QByteArray &data, int size);
    /**
     * @brief generateRandomString
     * @param str
     * Генерирует случайную строку для отправки серверу в пределах верхнего лимита
     * @param size
     */
    void generateRandomString(QString &str, int size);

private slots:
    /**
     * @brief Init
     * Инициализирует соединение и устанавливает таймеры
     */
    void Init();
    /**
     * @brief sendMessage
     * Отправление сообщение серверу
     */
    void sendMessage();
    /**
     * @brief readMessage
     * Обрабатывает входящие сообщения от сервера
     */
    void readMessage();
    /**
     * @brief reconnecting
     * Обрабатывает логику повторного подключения, увеличивая счетчик отключений и перезапуская таймер для новой попытки подключения
     */
    void reconnecting();
    /**
     * @brief connectToServer
     * Устанавливает соединение с сервером.
     * Если соединение не удается, запускается таймер для повторной попытки подключения.
     */
    void connectToServer();
};

#endif // CLIENTS_H
