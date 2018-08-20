#ifndef GENERALOMSKWORKER_H
#define GENERALOMSKWORKER_H

#include <QObject>
#include <QDebug>
#include "generalsocket.h"

class GeneralOMSKWorker: public QObject
{
    Q_OBJECT
public:
    explicit GeneralOMSKWorker(QObject* parent = nullptr);
    ~GeneralOMSKWorker();
signals:
    void setAsterRec(QString query, quint64 callerphone, quint64 callername, quint64 agentphone, quint64 agentname, quint64 datetime, quint64 asterunicID, QString asterchannelState, QString strDT);
public slots:
    void getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
    void createGeneralSocket();
public slots:
private:
    GeneralSocket* genOMSKSock;
};

#endif // GENERALOMSKWORKER_H
