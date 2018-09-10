#ifndef LISTENSOCKET_H
#define LISTENSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QTcpServer>
#include <cstdlib>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QList>
#include <QVariant>

#include "databaseengine.h"
#include "newconnectionworker.h"

class ListenSocket: public QObject
{
    Q_OBJECT
public:
    explicit ListenSocket(QObject* parent = nullptr);
    ~ListenSocket();
    void runListenServer();
    void stopListenServer();
signals:
    void update();
    void setDBEngine(DataBaseEngine* dbengine);
    void setToClient(QByteArray data, bool check);
public slots:
    void newListenConnection();
    void listenRead();
    void listenReadEnd();
    void getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT);
    void recInsertion(QString agent, QString callerPhoneTxt, QString callerNameTxt, QString userNameTxt, QString dateTimeTxt, QString queryCombo, QString asterUnicID, QString comment);
    void inLineInsertion(QString agent, QString tableName, QString val);
private slots:

private:
    QTcpServer* listenServer;
    QSqlDatabase db;
    DataBaseEngine* dbEngine;
    QMap<int, QVariant> asterPastRows;
    QMap<int, QVariant> asterPastCols;
    QMap<int, QVariant> crmPastRows;
    QMap<int, QVariant> crmPastCols;
};

#endif // LISTENSOCKET_H
