#include "generalserversocket.h"

GeneralServerSocket::GeneralServerSocket(QObject *parent, QString server_address, quint16 port) : QObject(parent)
{
    socket = new QTcpSocket(this);
    addListeningPort(server_address, port);
}

GeneralServerSocket::~GeneralServerSocket()
{

}

void GeneralServerSocket::addListeningPort(QString server_address, quint16 port)
{
    socket->bind(QHostAddress(server_address), port, QTcpSocket::DefaultForPlatform);
}

void GeneralServerSocket::Run()
{
    socket->waitForReadyRead();
    QString req;
    QDataStream in (&req);
}

