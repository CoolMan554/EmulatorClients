#include "networkprotocol.h"

NetworkProtocol::NetworkProtocol()
{

}

NetworkProtocol::~NetworkProtocol()
{

}

QByteArray NetworkProtocol::prepareMessage(const QByteArray &data, const quint32 messageId)
{
    QByteArray bufferData;
    bufferData.clear();
    QDataStream out(&bufferData, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Version::Qt_5_15);
    out << quint16(0) << messageId << data;
    out.device()->seek(0);
    out << quint16(bufferData.size() - sizeof(quint16));
    return bufferData;
}

QByteArray NetworkProtocol::processMessage(QTcpSocket *tcpSocket)
{
    QDataStream in(tcpSocket);
    QByteArray message;
    message.clear();
    in.setVersion(QDataStream::Version::Qt_5_15);
    quint16 nextBlockSize{0};///<Размер блока данных от сервера
    if(tcpSocket->isOpen() && tcpSocket->isValid())
    {
        if(in.status() == QDataStream::Ok)
        {
            for(;;)
            {
                if(nextBlockSize == 0)
                {
                    if(tcpSocket->bytesAvailable() < 2)//Данные не могут быть меньше 2 байт
                    {
                        qDebug() << "NetworkProtocol::processMessage" << "Error -> Data < 2";
                        break;
                    }
                    in >> nextBlockSize;
                    if(tcpSocket->bytesAvailable() < nextBlockSize)//Данные пришли не полностью
                    {
                        qDebug() << "NetworkProtocol::processMessage" << "Error -> Data < nextBlockSize";
                        break;
                    }
                    in >> message;
                    break;
                }
            }
        }
        else qDebug() << "NetworkProtocol::processMessage" << "Error -> QDataStream";
    }
    else qDebug() << "NetworkProtocol::processMessage" << "Error -> TcpSocket not open or not Valid";
    return message;
}
