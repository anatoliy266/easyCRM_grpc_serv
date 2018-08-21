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
    QByteArray requestStr = newConSock->readAll();
    QString str = QString::fromUtf8(requestStr);
    if (str.split("/r/n").value(0) == "recordInsertion")
    {
        qDebug() << "new call record insertion start";
        QString id = str.split("/r/n").value(0);
        QString agent = str.split("/r/n").value(1);
        QString callerPhoneTxt = str.split("/r/n").value(2);
        QString callerNameTxt = str.split("/r/n").value(3);
        QString userNameTxt = str.split("/r/n").value(4);
        QString dateTimeTxt = str.split("/r/n").value(5);
        QString queryCombo = str.split("/r/n").value(6);
        QString asterUnicID = str.split("/r/n").value(7);
        QString comment = str.split("/r/n").value(8);
        qDebug() << id;
        qDebug() << agent;
        qDebug() << callerNameTxt;
        qDebug() << callerPhoneTxt;
        qDebug() << userNameTxt;
        qDebug() << dateTimeTxt;
        qDebug() << queryCombo;
        qDebug() << asterUnicID;
        qDebug() << comment;
        qDebug() << "properties to insert get!";
        emit recInsertion(agent, callerPhoneTxt, callerNameTxt, userNameTxt, dateTimeTxt, queryCombo, asterUnicID, comment);
    } else {
        int id = QString(str.split("/r/n").value(0)).toInt();
        QString tableName = str.split("/r/n").value(1);
        QString userName = str.split("/r/n").value(2);
        QString query = str.split("/r/n").value(3);
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
            emit dbProperty(id, tableName, filter);
            break;
        }

        case 1: {
            filterString = "crm.user IN (%1)";
            filter = filterString.arg(intUserName);
            if (filterList.count() > 0) {
                if (filterList.value(0) != "null")
                {
                    for (int i = 0; i <filterList.count(); i++)
                    {
                        filter += " AND " + filterList.value(i);
                    }
                }
            }
            emit dbProperty(id, tableName, filter);
            break;
        }

        case 2: {
            if (filterList.count() > 0) {
                if (filterList.value(0) != "null")
                {
                    filter = filterList.value(0);
                    if (filterList.count() > 1) {
                        for (int i = 1; i <filterList.count(); i++)
                        {
                            filter += " AND " + filterList.value(i);
                        }
                    }

                }
            }
            emit dbProperty(id, tableName, filter);
            break;
        }

        case 3: {
            filter = "";
            emit dbProperty(id, tableName, filter);
            break;
        }

        case 4: {
            break;
        }

        case 5: {
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
            emit dbProperty(id, tableName, filter);
            break;
        }
        }
    }
}

void NewConnectionWorker::setToClient(QByteArray arr, bool suces)
{
    if (suces)
    {
        writeSock = static_cast<QTcpSocket*>(newConSock);
        writeSock->write(arr);
        writeSock->flush();
        emit sucess();
    }

}

void NewConnectionWorker::sucessProceed()
{
    emit success();
}

void NewConnectionWorker::getSocketDesc(QObject* obj)
{
    newConSock = static_cast<QTcpSocket*>(obj);
    connect(newConSock, SIGNAL(readyRead()), this, SLOT(listenRead()));
}




/* /////OLD/////*/
 //AND (asterCall.agentPhone = %1 OR asterCall.callerPhone = %1)
