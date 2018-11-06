#ifndef NEWCONNECTIONWORKER_H
#define NEWCONNECTIONWORKER_H

#include <QObject>
#include <QTcpSocket>

#include "databaseengine.h"

#include "params.h"

#include <QObject>
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
#include <QEventLoop>
#include <QEvent>
#include <QIODevice>
#include <QAbstractSocket>

enum DataMethod {
    getHistory,
    getCalls,
    getQuery,
    getUsers,
    getOrgs
};

typedef struct DataStruct {
    QString _table;
    int _rows;
    int _cols;
    QString _query;
    DataMethod method;
} dStruct;


class NewConnectionWorker: public QObject
{
    Q_OBJECT
public:
    explicit NewConnectionWorker(QObject* parent = nullptr);
    ~NewConnectionWorker();
    QSqlDatabase *prepareDBConnection();
signals:
public slots:
    void newIncomingConnection(qintptr socketDescriptor);
    void listenRead();
private:


};

#endif // NEWCONNECTIONWORKER_H
