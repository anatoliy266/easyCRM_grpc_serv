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
    QThread* thread = new QThread();
    NewConnectionWorker* newConWork = new NewConnectionWorker();
    QObject::connect(newConWork, SIGNAL(dbProperty(int,QString,QString,QString)), dbEngine, SLOT(dBProceed(int,QString,QString,QString)));
    QObject::connect(dbEngine, SIGNAL(dbData(QByteArray,bool,int,int, QString, QString)),newConWork, SLOT(setToClient(QByteArray,bool,int,int,QString,QString)));
    QObject::connect(newConWork, SIGNAL(recInsertion(QString,QString,QString,QString,QString,QString,QString,QString)), this, SLOT(recInsertion(QString,QString,QString,QString,QString,QString,QString,QString)));

    newConWork->moveToThread(thread);
    newConWork->getSocketDesc(listenServer->nextPendingConnection());

    QObject::connect(thread, SIGNAL(started()), newConWork, SLOT(listenRead()));
    QObject::connect(newConWork, SIGNAL(success()), thread, SLOT(quit()));
    QObject::connect(thread, SIGNAL(finished()), newConWork, SLOT(deleteLater()));
    QObject::connect(newConWork, SIGNAL(inLineInsertion(QString,QString,QString)), this, SLOT(inLineInsertion(QString,QString,QString)));
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

    /*проверка на повторные записи от астерикса*/

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
            /*если записей нет*/
            astQuery.prepare(query);
            astQuery.bindValue(":callerPhone", callerPhone);
            astQuery.bindValue(":callerName", callerName);
            astQuery.bindValue(":agentPhone", agentPhone);
            astQuery.bindValue(":agentName", agentName);
            astQuery.bindValue(":dateTime", dateTime);
            astQuery.bindValue(":asterUnicID", asterUnicID);
            astQuery.bindValue(":asterChannelState", asterChannelState);

            /*смотрим по таблицам организаций и агентов есть ли имя звонившего для номера звонившего и ответившего*/

            QSqlQuery agentAPhoneExist;

            QString agentPhoneExistStr = "SELECT userName FROM agentPhones WHERE agentPhone = :agentPhone";

            agentAPhoneExist.prepare(agentPhoneExistStr);
            agentAPhoneExist.bindValue(":agentPhone", callerPhone);
            QString agentStrName;
            if (agentAPhoneExist.exec())
            {
                if (agentAPhoneExist.size() != 0)
                {
                    while (agentAPhoneExist.next())
                    {
                        agentStrName = agentAPhoneExist.record().value(0).toString();
                    }
                } else {
                    agentStrName = QString::number(callerPhone);
                }
            }

            /**/


            astQuery.bindValue(":asterOrgUser", agentStrName);
            astQuery.exec();
        }
    }
    QString orgPhoneStr;
    QString agentPhoneStr;



    /*проверяем, есть ли в таблице организаций полученный номер звонившего*/

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
            /*если нет - добавляем*/

            orgPhoneStr = "INSERT INTO orgsPhones(orgPhone, orgName, userName)"
                                  "VALUES(:orgPhone, :orgName, :userName)";
            orgPhoneQuery.prepare(orgPhoneStr);
            orgPhoneQuery.bindValue(":orgPhone", callerPhone);
            orgPhoneQuery.bindValue(":orgName", callerName);
            orgPhoneQuery.bindValue(":userName", callerName);
            orgPhoneQuery.exec();
        }
    }

    /*Проверяем таблицу агентов на наличие номера звонившего*/

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
            /*если нет - добавляем*/
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

    /*та же самая проверка но теперь с номером ответившего*/

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


void ListenSocket::recInsertion(QString agent, QString callerPhoneTxt, QString callerNameTxt, QString userNameTxt, QString dateTimeTxt, QString queryCombo, QString asterUnicID, QString comment)
{
    /*добавление записи из вьюхи астерикса в историю звонков(crm)*/

    db.open();
    QSqlQuery dtQuery;
    QString dtQ = "SELECT unixTime FROM time WHERE strTime IN (:dateTime)";
    int dateTime;
    QSqlRecord dtRec;
    dtQuery.prepare(dtQ);
    dtQuery.bindValue(":dateTime", dateTimeTxt);
    if (dtQuery.exec())
    {
        while(dtQuery.next())
        {
            dtRec = dtQuery.record();
        }
    }
    dateTime = dtRec.value(0).toInt();

    QSqlQuery historyQuery;
    QString query = "INSERT INTO crm(user, dateTime, org, query, fio, telephone, comment)"
            "VALUES(:user, :dateTime, :org, :query, :fio, :telephone, :comment)";
    int agentInt = agent.toInt();
    historyQuery.prepare(query);
    historyQuery.bindValue(":user", agentInt);
    historyQuery.bindValue(":dateTime", dateTime);
    historyQuery.bindValue(":org", callerNameTxt);
    historyQuery.bindValue(":query", queryCombo);
    historyQuery.bindValue(":fio", userNameTxt);
    historyQuery.bindValue(":telephone", callerPhoneTxt);
    historyQuery.bindValue(":comment", comment);
    bool histcheck = historyQuery.exec();
    if (histcheck)
    {
        int asterUnicIDint = asterUnicID.toInt();
        bool astCheck;
        QSqlQuery asterDelQuery;
        QString delQuery = "DELETE FROM asterCall WHERE asterUnicID IN (:asterUnicID)";
        asterDelQuery.prepare(delQuery);
        asterDelQuery.bindValue(":asterUnicID", asterUnicIDint);
        astCheck = asterDelQuery.exec();
        if (astCheck)
            qDebug() << "sucess";
        else
            qDebug() << "error";


        /*orgsPhones Insertion*/
        QSqlQuery orgQuery;
        QSqlQuery orgInQuery;
        QString orgStr;
        QString orgInStr;
        QSqlRecord orgRec;
        QList<QSqlRecord> orgRecList;
        qlonglong orgPhone = callerPhoneTxt.toLongLong();

        orgStr = "SELECT orgPhone FROM orgsPhones WHERE orgPhone = :orgPhone";
        orgQuery.prepare(orgStr);
        orgQuery.bindValue(":orgPhone", orgPhone);
        if (orgQuery.exec())
        {
            while (orgQuery.next())
            {
                orgRec = orgQuery.record();
                orgRecList.append(orgRec);
            }
        }
        bool incheck;
        bool updCheck;
        if (orgRecList.count() > 0)
        {
            orgInStr = "UPDATE orgsPhones SET orgName = :orgName, userName = :userName WHERE orgPhone = :orgPhone";
            orgInQuery.prepare(orgInStr);
            orgInQuery.bindValue(":orgName", callerNameTxt);
            orgInQuery.bindValue(":userName", userNameTxt);
            orgInQuery.bindValue(":orgPhone", orgPhone);
            updCheck = orgInQuery.exec();
        } else {
            orgInStr = "INSERT INTO orgsPhones(orgPhone, orgName, userName)"
                    "VALUES(:orgPhone, :orgName, :userName)";
            orgInQuery.prepare(orgInStr);
            orgInQuery.bindValue(":orgPhone", orgPhone);
            orgInQuery.bindValue(":orgName", callerNameTxt);
            orgInQuery.bindValue(":userName", userNameTxt);
            incheck = orgInQuery.exec();
        }

        /*agentsPhones Insertion*/

        QSqlQuery agentQuery;
        QSqlQuery agentInQuery;
        QString agentStr;
        QString agentInStr;
        QSqlRecord agentRec;
        QList<QSqlRecord> agentRecList;
        qlonglong agentPhone = callerPhoneTxt.toLongLong();

        agentStr = "SELECT agentPhone FROM agentPhones WHERE agentPhone = :orgPhone";
        agentQuery.prepare(agentStr);
        agentQuery.bindValue(":orgPhone", agentPhone);
        if (agentQuery.exec())
        {
            while (agentQuery.next())
            {
                agentRec = agentQuery.record();
                agentRecList.append(agentRec);
            }
        }
        if (agentRecList.count() > 0)
        {
            agentInStr = "UPDATE agentPhones SET agentsName = :orgName, userName = :userName WHERE agentPhone = :agentPhone";
            agentInQuery.prepare(agentInStr);
            agentInQuery.bindValue(":orgName", callerNameTxt);
            agentInQuery.bindValue(":userName", userNameTxt);
            agentInQuery.bindValue(":agentPhone", agentPhone);
            agentInQuery.exec();
        } else {
            agentInStr = "INSERT INTO agentPhones(agentPhone, agentsName, userName)"
                    "VALUES(:orgPhone, :orgName, :userName)";
            agentInQuery.prepare(agentInStr);
            agentInQuery.bindValue(":orgPhone", agentPhone);
            agentInQuery.bindValue(":orgName", callerNameTxt);
            agentInQuery.bindValue(":userName", userNameTxt);
            agentInQuery.exec();
        }
    }
    db.close();
}

void ListenSocket::inLineInsertion(QString agent, QString tableName, QString val)
{
    db.open();
    QSqlQuery inLineQuery;
    int intAgent = agent.toInt();
    int intVal = val.toInt();
    QString inLineString = "UPDATE users SET inLine = :val where id_user = :agent";
    inLineQuery.prepare(inLineString);
    inLineQuery.bindValue(":val", intVal);
    inLineQuery.bindValue(":agent", intAgent);
    inLineQuery.exec();
    db.close();
    Q_UNUSED(tableName);
}
