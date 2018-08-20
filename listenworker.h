#ifndef LISTENWORKER_H
#define LISTENWORKER_H

#include <QObject>
#include <QDebug>
#include "listensocket.h"

class ListenWorker: public QObject
{
    Q_OBJECT
public:
    explicit ListenWorker(QObject* parent = nullptr);
    ~ListenWorker();

signals:
    void setAsterRec(QString query, quint64 callerphone, quint64 callername, quint64 agentphone, quint64 agentname, quint64 datetime, quint64 asterunicID, QString asterchannelState, QString strDT);
public slots:
    void createListenSocket();
    void getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
private:
    ListenSocket* listenSock;
};

#endif // LISTENWORKER_H
