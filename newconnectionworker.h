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
    void dbProperty(int id, QString table, QString user, QString filter);
    void pastSafe(QMap<int, QVariant> PastRows, QMap<int, QVariant> PastCols, QString table);
    void recInsertion(QString agent, QString callerPhoneTxt, QString callerNameTxt, QString userNameTxt, QString dateTimeTxt, QString queryCombo, QString asterUnicID, QString comment);
    void getPastBuffer(QMap<int, QVariant> aPastRows, QMap<int, QVariant> aPastCols, QMap<int, QVariant> cPastRows, QMap<int, QVariant> cPastCols);
    void inLineInsertion(QString agent, QString tableName, QString val);
public slots:
    void listenRead();
    void setToClient(QByteArray arr, bool checkchanged, int rowsCount, int colsCount, QString table, QString userName);
    void sucessProceed();

private:
    QTcpSocket* newConSock;
    QTcpSocket* writeSock;
    QMap<int, QVariant> asterPastRows;
    QMap<int, QVariant> asterPastCols;
    QMap<int, QVariant> crmPastRows;
    QMap<int, QVariant> crmPastCols;
    QMap<int, QVariant> orgPastRows;
    QMap<int, QVariant> orgPastCols;

};

#endif // NEWCONNECTIONWORKER_H
