#include "listensocket.h"
#include <QtNetwork>


ListenSocket::ListenSocket(QObject *parent): QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("P://1//123.db");
    if (!db.open())
    {
        qDebug() << "Database not open!!!";
    } else {
        qDebug() << "Database open!!!";
    };
    runListenServer();
    connect(listenServer, SIGNAL(newConnection()), this, SLOT(newListenConnection()));
}

ListenSocket::~ListenSocket()
{

}

void ListenSocket::runListenServer()
{
    listenServer = new QTcpServer(this);
    if (listenServer->listen(QHostAddress::Any, 50051))
    {
        qDebug() << "Server started";
    } else {
        qDebug() << "Sever not started";
        listenServer->close();
    }
}

void ListenSocket::newListenConnection()
{
    qDebug() << "New connection";
    db.open();
    dbEngine = new DataBaseEngine(this, db);
    QThread* thread = new QThread(0);
    NewConnectionWorker* newConWork = new NewConnectionWorker();
    QObject::connect(newConWork, SIGNAL(dbProperty(int,QString,QString)), dbEngine, SLOT(dBProceed(int,QString,QString)));
    QObject::connect(dbEngine, SIGNAL(dbData(QByteArray,bool,int,int, QString)),this, SLOT(checkPastRowCount(QByteArray,bool,int,int, QString)));
    QObject::connect(this, SIGNAL(setToClient(QByteArray,bool)), newConWork, SLOT(setToClient(QByteArray,bool)));
    QObject::connect(newConWork, SIGNAL(recInsertion(QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(recInsertion(QString,QString,QString,QString,QString,QString,QString,QString)));
    newConWork->getSocketDesc(listenServer->nextPendingConnection());
    newConWork->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), newConWork, SLOT(listenRead()));
    QObject::connect(newConWork, SIGNAL(success()), thread, SLOT(quit()));
    QObject::connect(thread, SIGNAL(finished()), newConWork, SLOT(deleteLater()));
    thread->start();



}

void ListenSocket::listenRead()
{
    /*QThread* thread = new QThread(0);
    NewConnectionWorker* newConWork = new NewConnectionWorker();
    QObject::connect(newConWork, SIGNAL(success()), this, SLOT(listenRead()));
    QObject::connect(newConWork, SIGNAL(dbProperty(int,QString,QString)), dbEngine, SLOT(dBProceed(int,QString,QString)));
    QObject::connect(dbEngine, SIGNAL(dbData(QByteArray)), newConWork, SLOT(setToClient(QByteArray)));
    newConWork->getSocketDesc(sender());
    newConWork->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), newConWork, SLOT(listenRead()));
    QObject::connect(thread, SIGNAL(finished()), newConWork, SLOT(deleteLater()));
    thread->start();*/
}

void ListenSocket::getAsterRec(QString query, quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{
    db.open();
    qDebug() << query;
    QSqlQuery astQuery;
    QSqlQuery orgPhoneQuery;
    QSqlQuery timeQuery;
    QSqlQuery checkQuery;
    QSqlQuery agentPhoneQuery;
    QSqlQuery checkQuery1;
    QSqlQuery agentPhoneQuery1;
    QSqlQuery agentCheck;
    QSqlQuery agentCheck1;
    QSqlQuery orgPhoneQuery1;
    QSqlQuery asterCheckRec;
    //asterCall(callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, asterOrgUser)
    QString asterCheckRecStr = "SELECT * FROM asterCall WHERE asterCall.callerPhone = :callerPhone "
                               "AND asterCall.callerName = :callerName "
                               "AND asterCall.agentName = :agentName "
                               "AND asterCall.agentPhone = :agentPhone "
                               "AND asterCall.asterUnicID = :asterUnicID";
    asterCheckRec.prepare(asterCheckRecStr);
    asterCheckRec.bindValue(":callerPhone", callerPhone);
    asterCheckRec.bindValue(":callerName", callerName);
    asterCheckRec.bindValue(":agentName", agentName);
    asterCheckRec.bindValue(":agentPhone", agentPhone);
    asterCheckRec.bindValue(":asterUnicID", asterUnicID);
    QSqlRecord asterCheckRecRec;
    QList<QSqlRecord> asterCheckRecList;
    if (asterCheckRec.exec())
    {
        asterCheckRecRec.clear();
        asterCheckRecList.clear();
        while (asterCheckRec.next())
        {
            asterCheckRecRec = asterCheckRec.record();
            asterCheckRecList.append(asterCheckRecRec);
        }
        if (asterCheckRecList.count() == 0)
        {
            astQuery.prepare(query);
            astQuery.bindValue(":callerPhone", callerPhone);
            astQuery.bindValue(":callerName", callerName);
            astQuery.bindValue(":agentPhone", agentPhone);
            astQuery.bindValue(":agentName", agentName);
            astQuery.bindValue(":dateTime", dateTime);
            astQuery.bindValue(":asterUnicID", asterUnicID);
            astQuery.bindValue(":asterChannelState", asterChannelState);
            astQuery.bindValue(":asterOrgUser", callerPhone);
            astQuery.exec();
        }
    }
    QString orgPhoneStr;
    QString agentPhoneStr;



    QString checkOrgPhoneStr = "SELECT * FROM orgsPhones WHERE orgPhone LIKE :orgPhone";
    checkQuery.prepare(checkOrgPhoneStr);
    checkQuery.bindValue(":orgPhone", callerPhone);

    QSqlRecord checkRec;
    QList<QSqlRecord> recList;
    if (checkQuery.exec())
    {
        recList.clear();
        checkRec.clear();
        while (checkQuery.next())
        {
            checkRec = checkQuery.record();
            recList.append(checkRec);
        }
        if (recList.count() == 0)
        {
            orgPhoneStr = "INSERT INTO orgsPhones(orgPhone, orgName, userName)"
                                  "VALUES(:orgPhone, :orgName, :userName)";
            orgPhoneQuery.prepare(orgPhoneStr);
            orgPhoneQuery.bindValue(":orgPhone", callerPhone);
            orgPhoneQuery.bindValue(":orgName", callerName);
            orgPhoneQuery.bindValue(":userName", callerName);
            orgPhoneQuery.exec();
        }
    }

    checkQuery1.prepare(checkOrgPhoneStr);
    checkQuery1.bindValue(":orgPhone", agentPhone);

    if (checkQuery1.exec())
    {
        recList.clear();
        checkRec.clear();
        while (checkQuery1.next())
        {
            checkRec = checkQuery1.record();
            recList.append(checkRec);
        }
        if (recList.count() == 0)
        {
            orgPhoneStr = "INSERT INTO agentPhones(agentPhone, agentsName, userName)"
                          "VALUES(:agentPhone, :agentsName, :userName)";
            orgPhoneQuery1.prepare(orgPhoneStr);
            orgPhoneQuery1.bindValue(":agentPhone", callerPhone);
            orgPhoneQuery1.bindValue(":agentsName", callerName);
            orgPhoneQuery1.bindValue(":userName", callerName);
            orgPhoneQuery1.exec();
        }
    }
    /* ///////////////////////////////////////////////////////////////// */
    /* ///////////////////////////////////////////////////////////////// */
    /* ///////////////////////////////////////////////////////////////// */
    /* ///////////////////////////////////////////////////////////////// */
    /* ///////////////////////////////////////////////////////////////// */
    /* ///////////////////////////////////////////////////////////////// */
    QString agentCheckStr = "SELECT * FROM agentPhones WHERE agentPhone LIKE :agentPhone";


    agentCheck.prepare(agentCheckStr);
    agentCheck.bindValue(":agentPhone", agentPhone);
    QSqlRecord checkRec1;
    QList<QSqlRecord> recList1;

    if (agentCheck.exec())
    {
        recList1.clear();
        checkRec1.clear();
        while (agentCheck.next())
        {
            checkRec1 = agentCheck.record();
            recList1.append(checkRec1);
        }
        if (recList1.count() == 0)
        {
            agentPhoneStr = "INSERT INTO agentPhones(agentPhone, agentsName, userName)"
                            "VALUES(:agentPhone, :agentsName, :userName)";
            agentPhoneQuery.prepare(agentPhoneStr);
            agentPhoneQuery.bindValue(":agentPhone", agentPhone);
            agentPhoneQuery.bindValue(":agentsName", agentName);
            agentPhoneQuery.bindValue(":userName", agentName);
            agentPhoneQuery.exec();
        }
    }

    agentCheck1.prepare(agentCheckStr);
    agentCheck1.bindValue(":agentPhone", callerPhone);
    if (agentCheck1.exec())
    {
        recList1.clear();
        checkRec1.clear();
        while (agentCheck1.next())
        {
            checkRec1 = agentCheck1.record();
            recList1.append(checkRec1);
        }
        if (recList.count() == 0)
        {
            agentPhoneStr = "INSERT INTO orgsPhones(orgPhone, orgName, userName)"
                            "VALUES(:orgPhone, :orgName, :userName)";
            agentPhoneQuery1.prepare(agentPhoneStr);
            agentPhoneQuery1.bindValue(":orgPhone", agentPhone);
            agentPhoneQuery1.bindValue(":orgName", agentName);
            agentPhoneQuery1.bindValue(":userName", agentName);
            agentPhoneQuery1.exec();
        }
    }

    QString timeStr = "INSERT INTO time(unixTime, strTime)"
                                 "VALUES(:unixTime, :strTime)";
    timeQuery.prepare(timeStr);
    timeQuery.bindValue(":unixTime", dateTime);
    timeQuery.bindValue(":strTime", strDT);
    timeQuery.exec();
    db.close();
}

void ListenSocket::listenReadEnd()
{
    dbEngine->deleteLater();
    db.close();
    sender()->thread()->quit();
}

void ListenSocket::checkPastRowCount(QByteArray arr, bool checkchanged, int rowsCount, int colsCount, QString table)
{

    QStringList tablesList;
    tablesList.append("asterCall");
    tablesList.append("crm");
    tablesList.append("orgsPhones");
    tablesList.append("query_type");
    tablesList.append("time");
    tablesList.append("users");
    switch (tablesList.indexOf(table)) {
    case 0: {
        qDebug() << asterPastRowsCount << "aster past rows count before";
        if (asterPastRowsCount != rowsCount || asterPastColsCount != colsCount)
        {
            asterPastRowsCount = rowsCount;
            asterPastColsCount = colsCount;
            emit setToClient(arr, checkchanged);
        }
        qDebug() << asterPastRowsCount << "aster past rows count after";
        break;
    }
    case 1: {
        qDebug() << crmPastRowsCount << "crm past rows count before";
        if (crmPastRowsCount != rowsCount || crmPastColsCount != colsCount)
        {
            crmPastRowsCount = rowsCount;
            crmPastColsCount = colsCount;
            emit setToClient(arr, checkchanged);
        }
        qDebug() << crmPastRowsCount << "crm past rows count after";
        break;
    }
    case 3: {
        emit setToClient(arr, checkchanged);
    }
    case 5: {
        emit setToClient(arr, checkchanged);
    }
    }
}

void ListenSocket::recInsertion(QString agent, QString callerPhoneTxt, QString callerNameTxt, QString userNameTxt, QString dateTimeTxt, QString queryCombo, QString asterUnicID, QString comment)
{
    db.open();
    QSqlQuery historyQuery;
    QString query = "INSERT INTO crm(user, dateTime, org, query, fio, telephone, comment)"
            "VALUES(:user, :dateTime, :org, :query, :fio, :telephone, comment)";
    historyQuery.prepare(query);
    historyQuery.bindValue(":user", agent);
    historyQuery.bindValue(":dateTime", dateTimeTxt);

}
