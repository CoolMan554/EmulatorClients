#ifndef CONFIGCOMMANDLINE_H
#define CONFIGCOMMANDLINE_H

#include <QObject>
#include <QCommandLineParser>
#include <QDebug>

class ConfigCommandLine
{
public:
    ConfigCommandLine();
    bool parseArguments();
    QString getAddress() const;
    int getPort() const;
    int getNumberOfConnections() const;
    double getPeriod() const;
private:
    QString address;
    int port;
    int countOfConnections;
    double period;
};

#endif // CONFIGCOMMANDLINE_H
