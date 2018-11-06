#ifndef GENERALWORKER_H
#define GENERALWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QVariant>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>

#include <QFile>
#include <QDateTime>
#include <QLibrary>

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include <keriostream.h>

class GeneralWorker: public QObject
{
    Q_OBJECT
public:
    explicit GeneralWorker(QObject* parent = nullptr);
    ~GeneralWorker();
    QSqlDatabase *prepareDBConnection();
    QStringList agentsList();
    bool checkForExitingRec(QString query);
    bool execute(QString query);
    QVariant returnUnicue(QString table, QString field, QString filter);

signals:
    void setReadData(QByteArray readData);
    void setAsterRec(QList<QVariant> callList);
    void setInLineUpdate(QList<QVariant> queueList);

public slots:
    void read();
    void getAsterRec(QList<QVariant> callList);
    void getInLineUpdate(QList<QVariant> queueList);
    void start();
private:

    KerioStream* ks;
    QTimer *tmr;
    QString token;
    int separator = 1;

};

#endif // GENERALWORKER_H
