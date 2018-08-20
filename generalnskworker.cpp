#include "generalnskworker.h"

GeneralNSKWorker::GeneralNSKWorker(QObject *parent)
{

}

GeneralNSKWorker::~GeneralNSKWorker()
{

}

void GeneralNSKWorker::createGeneralSocket()
{
    genNSKSock = new GeneralSocket();
    connect(genNSKSock, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)), this, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)));
    genNSKSock->setAsterHost("voip.nsk.550550.ru");
}

void GeneralNSKWorker::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{
    emit setAsterRec(query, callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, strDT);
}
