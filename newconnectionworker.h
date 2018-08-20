#ifndef NEWCONNECTIONWORKER_H
#define NEWCONNECTIONWORKER_H

#include <QObject>
#include <QTcpSocket>

#include "databaseengine.h"

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

class NewConnectionWorker: public QObject
{
    Q_OBJECT
public:
    explicit NewConnectionWorker(QObject* parent = nullptr);
    ~NewConnectionWorker();
    void getSocketDesc(QObject *obj);
signals:
    void success();
    void sucess();
    void update();
    void dbRoles(QString table, int id);
    void dbProperty(int id, QString table, QString filter);
    void recInsertion(QString agent, QString callerPhoneTxt, QString callerNameTxt, QString userNameTxt, QString dateTimeTxt, QString queryCombo, QString asterUnicID, QString comment);
public slots:
    void listenRead();
    void setToClient(QByteArray arr, bool suces);
    void sucessProceed();

private:
    QTcpSocket* newConSock;
    QTcpSocket* writeSock;

};

#endif // NEWCONNECTIONWORKER_H
