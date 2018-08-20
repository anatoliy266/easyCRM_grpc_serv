#include "generalomskworker.h"

GeneralOMSKWorker::GeneralOMSKWorker(QObject *parent)
{

}

GeneralOMSKWorker::~GeneralOMSKWorker()
{

}

void GeneralOMSKWorker::createGeneralSocket()
{
    genOMSKSock = new GeneralSocket();
    connect(genOMSKSock, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)), this, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)));
    genOMSKSock->setAsterHost("voip.omsk.550550.ru");
}

void GeneralOMSKWorker::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{
    emit setAsterRec(query, callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, strDT);
}
