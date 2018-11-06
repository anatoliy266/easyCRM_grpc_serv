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

//#include "databaseengine.h"
#include "newconnectionworker.h"
#include "generalworker.h"
#include "listenserver.h"

class ListenSocket: public QObject
{
    Q_OBJECT
public:
    explicit ListenSocket(QObject* parent = nullptr);
    ~ListenSocket();
    void runListenServer();
    void runKerioWorker();
signals:
    void setSocketHandler(qintptr socketDescriptor);
public slots:
    void newIncomingConnection(qintptr socketDescriptor);
    //void newListenConnection();
private slots:

private:
    //QTcpServer* listenServer;
};

#endif // LISTENSOCKET_H
