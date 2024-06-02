#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSharedPointer>
#include <QDebug>
#include "exitcodes.h"
#include "emulatorclients.h"
#include "configcommandline.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RU");

    QCoreApplication app(argc, argv);

    ConfigCommandLine config;

    if(!config.parseArguments())
    {
        return ExitCodes::ExitCode::Error;///<Ошибка в парсинге аргументов
    }

    qDebug() << "Address:" << config.getAddress() << "port:" << config.getPort() << "countConnection:" << config.getNumberOfConnections() << "period" << config.getPeriod();

    int exitCode =  ExitCodes::ExitCode::Restart;
    int       exceptionCount = 1;
    const int exceptionCountMax = 3;

    do
    {
        try
        {
            QSharedPointer<EmulatorClients> pEmulatorClients = QSharedPointer<EmulatorClients>::create(config.getAddress(), config.getPort(), config.getNumberOfConnections(), config.getPeriod());
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
