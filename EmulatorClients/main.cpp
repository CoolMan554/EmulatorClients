#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSharedPointer>
#include <QDebug>
#include "exitcodes.h"
#include "emulatorclients.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RU");

    QCoreApplication app(argc, argv);
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

    parser.process(app);

    if(!parser.isSet(portOption))
    {
        qCritical() << "Не указан порт целевого сервера! Запустите программу заново с полем -p или --port";
        return ExitCodes::ExitCode::Error;
    }

    const QString address = parser.value(addressOption);
    const int port = parser.value(portOption).toInt();
    const int connection = parser.value(connectOption).toInt();
    const double period = parser.value(periodOption).toInt();

    qDebug() << "Address:" << address << "port:" << port << "countConnection:" << connection << "period" << period;

    int exitCode =  ExitCodes::ExitCode::Restart;
    int       exceptionCount = 1;
    const int exceptionCountMax = 3;

    do
    {
        try
        {
            QSharedPointer<EmulatorClients> pEmulatorClients = QSharedPointer<EmulatorClients>::create(address, port, connection, period);
            exitCode = app.exec();
            qCritical() << "Critical Error EmulatorClients";
        }

        catch (const std::exception &ex) {
            qCritical() << "Error EmulatorClients" << ex.what();
            qCritical() << "EXCEPTION" << exceptionCount << "of" << exceptionCountMax;
            exceptionCount++;
        }
        catch(...)
        {
            qCritical() << "Error EmulatorClients UNKNOWN EXCEPTION";
            qCritical() << "EXCEPTION" << exceptionCount << "of" << exceptionCountMax;
            exceptionCount++;
        }

        if(!(exceptionCount <= exceptionCountMax))
            exitCode = ExitCodes::ExitCode::Exit;

    }while(exitCode != ExitCodes::ExitCode::Exit);

    return exitCode;
}
