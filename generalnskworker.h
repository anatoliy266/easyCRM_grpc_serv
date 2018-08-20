#ifndef GENERALNSKWORKER_H
#define GENERALNSKWORKER_H

#include <QObject>
#include <QDebug>
#include "generalsocket.h"

class GeneralNSKWorker: public QObject
{
    Q_OBJECT
public:
    explicit GeneralNSKWorker(QObject* parent = nullptr);
    ~GeneralNSKWorker();
signals:
    void setAsterRec(QString query, quint64 callerphone, quint64 callername, quint64 agentphone, quint64 agentname, quint64 datetime, quint64 asterunicID, QString asterchannelState, QString strDT);
public slots:
    void getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
    void createGeneralSocket();
public slots:
private:
    GeneralSocket* genNSKSock;
};

#endif // GENERALNSKWORKER_H
