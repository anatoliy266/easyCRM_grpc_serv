#ifndef LISTENSERVER_H
#define LISTENSERVER_H

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

class ListenServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit ListenServer(QObject* parent = nullptr);
protected:
    virtual void incomingConnection(qintptr socketDescriptor) override;
signals:
    void newInconingConnection(qintptr);
public slots:
};

#endif // LISTENSERVER_H
