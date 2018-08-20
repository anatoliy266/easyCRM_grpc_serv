#include "listenworker.h"

ListenWorker::ListenWorker(QObject *parent)
{

}

ListenWorker::~ListenWorker()
{

}

void ListenWorker::createListenSocket()
{
    listenSock = new ListenSocket();
    connect(this, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)), listenSock, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)));
}

void ListenWorker::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDt)
{
    emit setAsterRec(query, callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, strDt);
}
