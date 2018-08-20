#ifndef GENERALSOCKET_H
#define GENERALSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

class GeneralSocket: public QObject
{
    Q_OBJECT
public:
    explicit GeneralSocket(QObject* parent = nullptr);
    ~GeneralSocket();
    void asterSocketLogin();
    void asterSocketWrite(int clID, QString query);
    void setAsterHost(QString hostName);
signals:
    void setAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
public slots:
    void asterSocketRead();
private:
    bool setRecord(quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
    QTcpSocket* asterSocket;
    bool inLine;
    QStringList callerIDNumList;
    QStringList connectedLineNumList;
};

#endif // GENERALSOCKET_H
