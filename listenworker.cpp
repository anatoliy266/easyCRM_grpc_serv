#include "listenworker.h"

ListenWorker::ListenWorker(QObject *parent): QObject(parent)
{

}

ListenWorker::~ListenWorker()
{

}

void ListenWorker::createListenSocket()
{
    ListenSocket* listenSock = new ListenSocket();
    listenSock->runListenServer();
    listenSock->runKerioWorker();
}
