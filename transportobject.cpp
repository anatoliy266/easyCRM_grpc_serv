#include "transportobject.h"

TransportObject::TransportObject(QObject *parent)
{

}

TransportObject::~TransportObject()
{

}

void TransportObject::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{
    emit setAsterRec(query, callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, strDT);
}
