#include "newconnectionworker.h"

NewConnectionWorker::NewConnectionWorker(QObject *parent)
{
    connect(this, SIGNAL(sucess()), this, SLOT(sucessProceed()));
}

NewConnectionWorker::~NewConnectionWorker()
{

}

void NewConnectionWorker::listenRead()
{
    //qDebug() << "start reading from newconsock and " << this->thread() << " thread";
    QByteArray requestStr = newConSock->readAll();
    QString str = QString::fromUtf8(requestStr);
    if (str.split("/r/n").value(0) == "inline change")
    {
        qDebug() << "inline insertion";
        QString id = str.split("/r/n").value(0);
        QString agent = str.split("/r/n").value(1);
        QString tableName = str.split("/r/n").value(2);
        QString val = str.split("/r/n").value(3);
        emit inLineInsertion(agent, tableName, val);
    } else {
        if (str.split("/r/n").value(0) == "recordInsertion")
        {
            //qDebug() << "new call record insertion start";
            QString id = str.split("/r/n").value(0);
            QString agent = str.split("/r/n").value(1);
            QString callerPhoneTxt = str.split("/r/n").value(2);
            QString callerNameTxt = str.split("/r/n").value(3);
            QString userNameTxt = str.split("/r/n").value(4);
            QString dateTimeTxt = str.split("/r/n").value(5);
            QString queryCombo = str.split("/r/n").value(6);
            QString asterUnicID = str.split("/r/n").value(7);
            QString comment = str.split("/r/n").value(8);
            emit recInsertion(agent, callerPhoneTxt, callerNameTxt, userNameTxt, dateTimeTxt, queryCombo, asterUnicID, comment);
        } else {
            int id = QString(str.split("/r/n").value(0)).toInt();
            QString tableName = str.split("/r/n").value(1);
            QString userName = str.split("/r/n").value(2);
            int pastRow = QString(str.split("/r/n").value(3)).toInt();
            int pastCol = QString(str.split("/r/n").value(4)).toInt();
            QString query = str.split("/r/n").value(5);
            QString filter;
            QString filterString;
            QStringList tablesList;
            tablesList.append("asterCall");
            tablesList.append("crm");
            tablesList.append("orgsPhones");
            tablesList.append("query_type");
            tablesList.append("time");
            tablesList.append("users");
            QStringList filterList = query.split(";");
            int intUserName = userName.toLongLong();
            switch (tablesList.indexOf(tableName)) {
            case 0: {
                filter.clear();
                filterString.clear();
                filterString = "asterCall.agentPhone = %1 OR asterCall.callerPhone = %1";
                filter = filterString.arg(intUserName);
                if (filterList.count() > 0) {
                    if (filterList.value(0) != "null")
                    {
                        for (int i = 0; i <=filterList.count(); i++)
                        {
                            filter += " AND " + filterList.value(i);
                        }
                    }
                }
                //qDebug() << filter << "FILTER";
                asterPastRows.insert(intUserName, pastRow);
                asterPastCols.insert(intUserName, pastCol);
                emit dbProperty(id, tableName, userName, filter);
                break;
            }

            case 1: {
                filterString = "crm.user IN (%1)";
                filter = filterString.arg(intUserName);
                //qDebug() << "filterlist = " << filterList;
                if (filterList.count() > 0) {
                    if (filterList.value(0) != "null")
                    {
                        for (int i = 0; i <filterList.count(); i++)
                        {
                            if (filterList.value(i) != "")
                                filter += " AND " + filterList.value(i);
                        }
                    }
                }
                //qDebug() << filter << "FILTER";
                crmPastRows.insert(intUserName, pastRow);
                crmPastCols.insert(intUserName, pastCol);
                emit dbProperty(id, tableName, userName, filter);
                break;
            }

            case 2: {
                orgPastRows.insert(0, pastRow);
                orgPastCols.insert(0, pastCol);
                filter = "orgName NOT LIKE '' OR userName NOT LIKE ''";
                emit dbProperty(id, tableName, userName, filter);
                break;
            }

            case 3: {
                filter = "";
                emit dbProperty(id, tableName, userName, filter);
                break;
            }

            case 4: {
                break;
            }

            case 5: {
                if (id == 69)
                {
                    filter = "";
                } else {
                    filter = "id_user = " + userName;
                    if (filterList.count() > 0) {
                        if (filterList.value(0) != "null")
                        {
                            filter += " AND " + filterList.value(0);
                            if (filterList.count() > 1) {
                                for (int i = 1; i <=filterList.count(); i++)
                                {
                                    filter += " AND " + filterList.value(i);
                                }
                            }
                        }
                    }
                }
                emit dbProperty(id, tableName, userName, filter);
                break;
            }
            }
        }
    }

}

void NewConnectionWorker::setToClient(QByteArray arr, bool checkchanged, int rowsCount, int colsCount, QString table, QString userName)
{
    bool pastChecked = false;
    QStringList tablesList;
    tablesList.append("asterCall");
    tablesList.append("crm");
    tablesList.append("orgsPhones");
    tablesList.append("query_type");
    tablesList.append("time");
    tablesList.append("users");
    int intUser = userName.toInt();
    switch (tablesList.indexOf(table)) {
    case 0: {
        if (asterPastRows.size() == 0 || asterPastCols.size() == 0)
        {
            //emit setToClient(arr, checkchanged);
            pastChecked = true;
            asterPastRows.insert(intUser, rowsCount);
            asterPastCols.insert(intUser, colsCount);
            //qDebug() << asterPastRows[intUser] << " : " << intUser << "user aster rows count";
            //qDebug() << asterPastCols[intUser] << " : " << intUser << "user aster cols count";
            break;
        } else {
            if (asterPastRows[intUser] != rowsCount || asterPastCols[intUser] != colsCount)
            {
                //emit setToClient(arr, checkchanged);
                pastChecked = true;
                asterPastRows.insert(intUser, rowsCount);
                asterPastCols.insert(intUser, colsCount);
                //qDebug() << asterPastRows[intUser] << " : " << intUser << "user aster rows count";
                //qDebug() << asterPastCols[intUser] << " : " << intUser << "user aster cols count";

            }
            break;
        }

    }
    case 1: {
        if (crmPastRows.size() == 0 || crmPastCols.size() == 0)
        {
            //emit setToClient(arr, checkchanged);
            pastChecked = true;
            crmPastRows.insert(intUser, rowsCount);
            crmPastCols.insert(intUser, colsCount);
            break;
        } else {
            if (crmPastRows[intUser] != rowsCount || crmPastCols[intUser] != colsCount)
            {
                //emit setToClient(arr, checkchanged);
                pastChecked = true;
                crmPastRows.insert(intUser, rowsCount);
                crmPastCols.insert(intUser, colsCount);
                break;
            }
        }

    }
    case 2: {
        if (orgPastRows.size() == 0 || orgPastCols.size() == 0)
        {
            //emit setToClient(arr, checkchanged);
            pastChecked = true;
            orgPastRows.insert(0, rowsCount);
            orgPastCols.insert(0, colsCount);
            break;
        } else {
            if (orgPastRows[0] != rowsCount || orgPastCols[0] != colsCount)
            {
                //emit setToClient(arr, checkchanged);
                pastChecked = true;
                orgPastRows.insert(0, rowsCount);
                orgPastCols.insert(0, colsCount);
                break;
            }
        }
    }
    case 3: {
        pastChecked = true;
        break;
    }
    case 5: {
        pastChecked = true;
        asterPastRows.insert(intUser, 0);
        asterPastCols.insert(intUser, 0);
        crmPastRows.insert(intUser, 0);
        crmPastCols.insert(intUser, 0);
        break;
    }
    }
    if (pastChecked)
    {

        writeSock->write(arr);
        writeSock->flush();
        pastChecked = false;
    }
    emit sucess();
    Q_UNUSED(checkchanged);
}

void NewConnectionWorker::sucessProceed()
{
    emit success();
}

void NewConnectionWorker::getSocketDesc(QObject* obj)
{
    newConSock = static_cast<QTcpSocket*>(obj);
    writeSock = static_cast<QTcpSocket*>(obj);
    connect(newConSock, SIGNAL(readyRead()), this, SLOT(listenRead()));
    //qDebug() << "dynamic cast cosket from " << obj->thread() << " thread and newconsock from " << this->thread() << " thread";
}




/* /////OLD/////*/
 //AND (asterCall.agentPhone = %1 OR asterCall.callerPhone = %1)
