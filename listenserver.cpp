#include "listenserver.h"

ListenServer::ListenServer(QObject *parent): QTcpServer(parent)
{

}

void ListenServer::incomingConnection(qintptr socketDescriptor)
{
    emit newInconingConnection(socketDescriptor);
    qDebug() << "newIncomingConnection signal emitted";
}
