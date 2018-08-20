#include "generalworker.h"

GeneralWorker::GeneralWorker(QObject *parent)
{

}

GeneralWorker::~GeneralWorker()
{

}

void GeneralWorker::createGeneralSocket()
{
    genSock = new GeneralSocket();
    connect(genSock, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)), this, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString, QString)));
    genSock->setAsterHost("voip.rev.550550.ru");
}

void GeneralWorker::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{
    emit setAsterRec(query, callerPhone, callerName, agentPhone, agentName,  dateTime, asterUnicID, asterChannelState, strDT);
}

