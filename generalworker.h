#ifndef GENERALWORKER_H
#define GENERALWORKER_H

#include <QObject>
#include "generalsocket.h"
#include <QDebug>

class GeneralWorker: public QObject
{
    Q_OBJECT
public:
    explicit GeneralWorker(QObject* parent = nullptr);
    ~GeneralWorker();
signals:
    void setAsterRec(QString query, quint64 callerphone, quint64 callername, quint64 agentphone, quint64 agentname, quint64 datetime, quint64 asterunicID, QString asterchannelState, QString strDT);
public slots:
    void getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
    void createGeneralSocket();
private:
    GeneralSocket* genSock;
    int phone;
};

#endif // GENERALWORKER_H
