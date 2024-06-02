#ifndef CONFIGCOMMANDLINE_H
#define CONFIGCOMMANDLINE_H

#include <QObject>
#include <QCommandLineParser>
#include <QDebug>

/**
 * @brief The ConfigCommandLine class
 * Класс предназначен для обработки аргументов командной строки
 */
class ConfigCommandLine
{
public:
    ConfigCommandLine();
    /**
     * @brief parseArguments
     * Парсит аргументы командной строки с использованием QCommandLineParser
     * @return
     */
    bool parseArguments();
    QString getAddress() const;
    int getPort() const;
    int getNumberOfConnections() const;
    double getPeriod() const;
private:
    QString address;///<Адрес сервера
    int port;///<Порт сервера
    int countOfConnections;///<Количество подключений
    double period;///<Период для отправки сообщений
};

#endif // CONFIGCOMMANDLINE_H
