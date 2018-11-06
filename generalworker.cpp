#include "generalworker.h"

GeneralWorker::GeneralWorker(QObject *parent): QObject(parent)
{
    ks = new KerioStream();
    ks->connectToHost("some", 4021, "/admin/api/jsonrpc/");
    token = ks->authorization("some", "some", "quickCRMserv", "King Anthony", "1.1");
}

GeneralWorker::~GeneralWorker()
{

}

QSqlDatabase* GeneralWorker::prepareDBConnection()
{
    QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName("P://1//123.db");
    return db;
}

void GeneralWorker::start()
{
//    ks = new KerioStream();
//    ks->connectToHost("ats.550550.ru", 4021, "/admin/api/jsonrpc/");
//    token = ks->authorization("a.stanevich", "256532FdnjvfN73501505", "quickCRMserv", "King Anthony", "1.1");
//    qDebug() << token;
//    QTimer* tmr = new QTimer();
//    tmr->setInterval(2000);
//    connect(tmr, SIGNAL(timeout()), this, SLOT(read()), Qt::QueuedConnection);
//    if (token != "")
//        tmr->start();
//    else
//        qDebug() << "not logged in";
}

void GeneralWorker::read()
{
    if (separator == 1)
    {
        QList<QVariant> callList = ks->getCallHistory(token, 0, -1, "ANSWERED_DURATION", JParam::Asc);
        separator = 0;
        getAsterRec(callList);
    } else {
        QList<QVariant> queueList = ks->getQueueUsers(token);
        getInLineUpdate(queueList);
        separator = 1;
    }
}


void GeneralWorker::getAsterRec(QList<QVariant> callList)
{

    if (callList.value(0) != QVariant(0))
    {
        for (int i = 0; i < callList.count(); i++)
        {
            QStringList rowList = callList.value(i).toStringList();
            if (rowList.count() > 0)
            {
                qlonglong callerPhone = 0;
                qlonglong callerName = 0;
                qlonglong agentPhone = 0;
                qlonglong agentName = 0;
                uint dateTime = QDateTime::currentDateTime().toTime_t();
                QString str_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");
                qlonglong asterUnicId = 0;
                QString asterChannelState = "";
                QString asterOrgUser = "";
                for (int j = 0; j < rowList.count(); j++)
                {
                    if (rowList.value(j).split("@").value(0) == "id")
                    {
                        asterUnicId = rowList.value(j).split("@").value(1).split(".").value(0).toLongLong();
                    }
                    if (rowList.value(j).split("@").value(0) == "FROM")
                    {
                        if (rowList.value(j).split("@").value(1) != "**120")
                        {
                            callerPhone = rowList.value(j).split("@").value(1).toLongLong();
                            callerName = rowList.value(j).split("@").value(1).toLongLong();
                        }
                    }
                    if (rowList.value(j).split("@").value(0) == "TO")
                    {
                        if (rowList.value(j).split("@").value(1) != "**120")
                        {
                            agentPhone = rowList.value(j).split("@").value(1).toLongLong();
                            agentName = rowList.value(j).split("@").value(1).toLongLong();
                        }
                    }
                    if (rowList.value(j).split("@").value(0) == "STATUS")
                    {
                        if (rowList.value(j).split("@").value(1) == "1")
                            asterChannelState = "Up";
                        else
                            asterChannelState = "Other";
                    }
                }

                QStringList agentList = agentsList();


                if (agentList.indexOf(QString::number(callerPhone)) != -1 || agentList.indexOf(QString::number(agentPhone)) != -1)
                {
                    if (callerPhone != 0 && agentPhone != 0)
                    {
                        if (agentList.indexOf(QString::number(callerPhone)) != -1 && agentList.indexOf(QString::number(agentPhone)) == -1)
                        {
                            auto _cPhone = callerPhone;
                            callerPhone = agentPhone;
                            agentPhone = _cPhone;

                            auto _cName = callerName;
                            callerName = agentName;
                            agentName = _cName;
                        }

                        asterOrgUser = returnUnicue("orgName", "orgsPhones", "orgPhone = " + QString::number(callerPhone)).toString();

                        if (asterChannelState == "Up")
                        {
                            QString _str = "SELECT * FROM asterCall WHERE asterCall.callerPhone = %1 "
                                                                      "AND asterCall.callerName = %2 "
                                                                      "AND asterCall.agentName = %3 "
                                                                      "AND asterCall.agentPhone = %4 "
                                                                      "AND asterCall.asterUnicID = %5";

                            if (!checkForExitingRec(_str.arg(callerPhone).arg(callerName).arg(agentName).arg(agentPhone).arg(asterUnicId)))
                            {
                                /*unicue record*/
                                QString org_checkStr = "SELECT * FROM orgsPhones WHERE orgPhone = %1";

//                                qDebug() << org_checkStr.arg(callerPhone);

                                if (!checkForExitingRec(org_checkStr.arg(callerPhone)))
                                {
                                    QString org_insStr = "INSERT INTO orgsPhones(orgPhone, orgName, userName)"
                                                         "VALUES(%1, %2, %3)";

//                                    qDebug() << org_insStr.arg(callerPhone).arg(callerPhone).arg(callerPhone);

                                    if (execute(org_insStr.arg(callerPhone).arg(callerPhone).arg(callerPhone)))
                                    {
//                                        qDebug() << "org rec insertion sucess";


                                    } else {
//                                        qDebug() << "org insertion failed";
                                    }

                                } else {
//                                    qDebug() << "org phone already exist";
                                }

                                QString ins_str = "INSERT INTO asterCall(callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, asterOrgUser)"
                                                  "VALUES(%1, %2, %3, %4, %5, %6, '%7', '%8')";

                                if (this->execute(ins_str.arg(callerPhone).arg(callerName).arg(agentPhone).arg(agentName).arg(dateTime).arg(asterUnicId).arg(asterChannelState).arg(asterOrgUser)))
                                {
                                    QString time_ins_str = "INSERT INTO time(unixTime, strTime)"
                                                           "VALUES(%1, '%2')";

                                    if (execute(time_ins_str.arg(dateTime).arg(str_time)))
                                    {
//                                        qDebug() << "time insertion sucess";
                                    } else {
//                                        qDebug() << "time insertion failed";
                                    }
                                } else {
//                                    qDebug() << "failed";
                                }
                            } else {
                                /*not unicue record*/
//                                qDebug() << "record already exist";
                            }
                        }
                    }
                } else {
//                    qDebug() << "another call";
                }
            }
        }
    }
}

void GeneralWorker::getInLineUpdate(QList<QVariant> queueList)
{

    //emit setInLineUpdate(queueList);
    QStringList agentList = this->agentsList();
    QHash<QString, QVariant> queue;
//    qDebug() << queueList.value(0);
    for (int c = 0; c<agentList.count(); c++)
    {
        queue[agentList.value(c)] = 0;
    }
    for (int i = 0; i<queueList.count(); i++)
    {
        QStringList row = queueList.value(i).toStringList();
        for (int j = 0; j < row.count(); j++)
        {
            if (row.value(j).split("@").value(0) == "EXTENSION_NUMBER")
            {
                queue[row.value(j).split("@").value(1)] = 1;
            }
        }
    }

    QString inl_ins_str = "UPDATE users SET inLine = %1 WHERE number = %2";

    for (int agent = 0; agent < agentList.count(); agent++)
    {
        execute(inl_ins_str.arg(queue[agentList.value(agent)].toInt()).arg(agentList.value(agent).toInt()));
    }
}

QStringList GeneralWorker::agentsList()
{
    QStringList _agentsList;
    auto _db = prepareDBConnection();
    if (_db->open())
    {
        QSqlQuery _query;
        QString _queryStr = "SELECT number FROM users";
        _query.prepare(_queryStr);
        if (_query.exec())
        {
            while (_query.next())
            {
                _agentsList.append(_query.record().value(0).toString());
            }
            _db->close();
        } else {
            _db->close();
        }
        _query.clear();
    } else {
        _db->close();
    }
    QString connection = _db->connectionName();
    delete _db;
    QSqlDatabase::removeDatabase(connection);
    return _agentsList;
}

bool GeneralWorker::checkForExitingRec(QString query)
{
    bool c = false;
    auto _db = prepareDBConnection();
    if (_db->open())
    {
        QSqlQuery _query;
        _query.prepare(query);
        if (_query.exec())
        {
            QList<QSqlRecord> _recList;
            while(_query.next())
            {
                 _recList.append(_query.record());
            }
            _query.clear();
            if (_recList.count() > 0)
            {
//                qDebug() << "not unicue org phone";
                _db->close();
                c = true;
            } else {
//                qDebug() << "unicue org phone";
                _db->close();
                c = false;
            }
        } else {
//            qDebug() << "checking org phone failed!";
            _db->close();
            c = false;
        }

    } else {
        c = false;
    }
    QString connection = _db->connectionName();
    delete _db;
    QSqlDatabase::removeDatabase(connection);
    return c;
}

bool GeneralWorker::execute(QString query)
{
    bool c = false;
    auto _db = prepareDBConnection();
    if (_db->open())
    {
        QSqlQuery _query;
        _query.prepare(query);
        if (_query.exec())
        {
            c = true;
        } else {
            c = false;
        }
        _query.clear();
    }
    QString connection = _db->connectionName();
    delete _db;
    QSqlDatabase::removeDatabase(connection);
    return c;
}

QVariant GeneralWorker::returnUnicue(QString table, QString field, QString filter)
{
    QString _queryStr = "SELECT %1 FROM %2 WHERE %3";
    QVariant val = "some";
    auto _db = prepareDBConnection();
    if (_db->open())
    {
        QSqlQuery _query;
        _query.prepare(_queryStr.arg(field).arg(table).arg(filter));
        if (_query.exec())
        {
            QList<QSqlRecord> _rList;
            while(_query.next())
            {
                _rList.append(_query.record());
            }
            if (_rList.count() == 1)
            {
                val = _rList.value(0).value(0);
            } else {
                val = "some";
            }
            _db->close();
        } else {
            val = "some";
            _db->close();
        }
        _query.clear();
    } else {
        val = "some";
        _db->close();
    }
    QString connection = _db->connectionName();
    delete _db;
    QSqlDatabase::removeDatabase(connection);
    return val;
}
