#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSharedPointer>
#include <QDebug>
#include "exitcodes.h"
#include "emulatorclients.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RU");

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("emulatorclients-test");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("TestEmulator");
    parser.addHelpOption(); parser.addVersionOption();
    QCommandLineOption addressOption(QStringList() << "a" << "address" , QCoreApplication::translate("main", "Entering the address of the target server (Example: 127.0.0.1)"));
    QCommandLineOption portOption(QStringList() << "p" << "port" , QCoreApplication::translate("main", "Entering the port of the target server (Example: 7001)"));
    QCommandLineOption connectOption(QStringList() << "c" << "connect" , QCoreApplication::translate("main", "Number of connected sessions"));
    QCommandLineOption periodOption(QStringList() << "per" << "period" , QCoreApplication::translate("main", "Message period"));
    parser.addOption(addressOption);


    parser.process(app);

    if(!parser.isSet(portOption))
    {
        qCritical() << "Не указан порт целевого сервера! Запустите программу заново с полем -p или --port";
        return ExitCodes::ExitCode::Error;
    }

    QString address = parser.value(addressOption);
    int port = parser.value(portOption).toInt();
    int connection = parser.value(connectOption).toInt();
    int period = parser.value(periodOption).toInt();

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
