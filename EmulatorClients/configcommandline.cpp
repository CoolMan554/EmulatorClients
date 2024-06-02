#include "configcommandline.h"

ConfigCommandLine::ConfigCommandLine()
{

}

bool ConfigCommandLine::parseArguments()
{
    QCoreApplication::setApplicationName("emulatorclients-test");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "TestEmulator"));
    parser.addHelpOption(); parser.addVersionOption();
    QCommandLineOption addressOption(QStringList() << "a" << "address" , QCoreApplication::translate("main", "Entering the address of the target server (Example: 127.0.0.1)"), "address", "127.0.0.1");
    QCommandLineOption portOption(QStringList() << "p" << "port" , QCoreApplication::translate("main", "Entering the port of the target server (Example: 7001)"), "port");
    QCommandLineOption connectOption(QStringList() << "c" << "connect" , QCoreApplication::translate("main", "Number of connected sessions"), "connect", "100");
    QCommandLineOption periodOption(QStringList() << "per" << "period" , QCoreApplication::translate("main", "Message period"), "period", "1");
    parser.addOption(addressOption);
    parser.addOption(portOption);
    parser.addOption(connectOption);
    parser.addOption(periodOption);

    parser.process(QCoreApplication::arguments());

    if(!parser.isSet(portOption))
    {
        qCritical() << "ConfigCommandLine::parseArguments::Не указан порт целевого сервера! Запустите программу заново с полем -p или --port";
        return false;
    }

    this->address = parser.value(addressOption);
    this->port = parser.value(portOption).toInt();
    this->countOfConnections = parser.value(connectOption).toInt();
    this->period = parser.value(periodOption).toDouble();

    return true;
}

QString ConfigCommandLine::getAddress() const
{
    return address;
}

int ConfigCommandLine::getPort() const
{
    return port;
}

int ConfigCommandLine::getNumberOfConnections() const
{
    return countOfConnections;
}

double ConfigCommandLine::getPeriod() const
{
    return period;
}
