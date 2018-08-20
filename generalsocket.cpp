#include "generalsocket.h"

#include <QDebug>
#include <QString>
#include <stdio.h>
#include <math.h>
#include <string>
#include <QString>
#include <array>
#include <QStringList>
#include <QIODevice>
#include <QTime>


GeneralSocket::GeneralSocket(QObject *parent): QObject(parent)
{

}

GeneralSocket::~GeneralSocket()
{

}

void GeneralSocket::asterSocketLogin()
{
    QString str = "Action: Login\r\nActionID: 0\r\nUsername: reader\r\nSecret: Gksjhd7865JhJL\r\n\r\n";
    QByteArray asterRequestStr = str.toUtf8();
    asterSocket->write(asterRequestStr, asterRequestStr.size());
    QString queueStr = "Action: Events\r\nEventmask: call\r\n\r\n";
    QByteArray asterQueueStr = queueStr.toUtf8();
    asterSocket->write(asterQueueStr, asterQueueStr.size());
}

void GeneralSocket::asterSocketRead()
{  
    QString returnStr = QString::fromUtf8(asterSocket->readAll());
    QStringList returnStrList = returnStr.split("\r\n");
    if (returnStrList.value(0) == "Event: Newstate" || returnStrList.value(0) == "Event: Dial")
    {
        if (returnStrList.value(0) == "Event: Newstate")
        {
            for (int i = 0; i < callerIDNumList.count(); i++)
            {

                if (returnStrList.value(5) == callerIDNumList.value(i) || returnStrList.value(7) == connectedLineNumList.value(i))
                {
                    if (returnStrList.value(7) == "ConnectedLineNum: " )
                    {
                        if (returnStrList.value(4) == "ChannelStateDesc: Up")
                            {
                            qDebug() << "inline changed";
                        }
                    } else {
                        if (returnStrList.value(4) == "ChannelStateDesc: Up")
                        {
                            qDebug() << "asterCall check";
                            bool rec = false;
                            quint64 callerNum = QString(QStringList(QString(returnStrList.value(5)).split(": ")).value(1)).toULongLong();
                            quint64 agentNum = QString(QStringList(QString(returnStrList.value(7)).split(": ")).value(1)).toULongLong();
                            quint64 callerName = QString(QStringList(QString(returnStrList.value(5)).split(": ")).value(1)).toULongLong();
                            quint64 agentName = QString(QStringList(QString(returnStrList.value(7)).split(": ")).value(1)).toULongLong();
                            QDateTime datetime = QDateTime::currentDateTime();
                            quint64 dateTime = datetime.toTime_t();
                            quint64 asterUnicID = QString(QStringList(QString(QStringList(QString(returnStrList.value(9)).split(": ")).value(1)).split(".")).value(0)).toULongLong();
                            QString channelStateDesc = QStringList(QString(returnStrList.value(4)).split(": ")).value(1);
                            QStringList agentsList;
                            agentsList.append("123");
                            agentsList.append("129");
                            agentsList.append("136");
                            agentsList.append("140");
                            agentsList.append("142");
                            agentsList.append("154");
                            agentsList.append("163");
                            for (int i = 0; i < agentsList.count(); i++)
                            {
                                if (QStringList(QString(returnStrList.value(7)).split(": ")).value(1) != agentsList.value(i))
                                {
                                    quint64 fakeAgent = callerNum;
                                    callerNum = agentNum;
                                    agentNum = fakeAgent;
                                    quint64 fakeAgentName = callerName;
                                    callerName = agentName;
                                    agentName = fakeAgentName;
                                }

                            }
                            qDebug() << "IN|UP? " << channelStateDesc;
                            QString sTime = datetime.toString("dd.MM.yyyy hh:mm");
                            rec = setRecord(callerNum, callerName, agentNum, agentName, dateTime, asterUnicID, channelStateDesc, sTime);
                            if (rec)
                            {
                                qDebug() << "record set!";
                            } else {
                                qDebug() << "record not set!";
                            }
                        }
                    }
                }
            }
        }
        if (returnStrList.value(0) == "Event: Dial")
        {
            qDebug() << "endcall check";
            int asterUnicId = QString(QStringList(QString(QStringList(QString(returnStrList.value(4)).split(": ")).value(1)).split(".")).value(0)).toInt();
            if (asterUnicId != 0)
            {
                QString filter = "asterUnicID LIKE " + QString::number(asterUnicId);
            } else {
                qDebug() << "asterunicID = 0";
            }
        }
    }
    asterSocket->waitForReadyRead();
}

void GeneralSocket::asterSocketWrite(int clID, QString query)
{
    Q_UNUSED(clID);
    Q_UNUSED(query);
}

bool GeneralSocket::setRecord(quint64 callerPhone, quint64 callerName, quint64 agentPhone, quint64 agentName, quint64 dateTime, quint64 asterUnicID, QString asterChannelState, QString strDT)
{


    QString strQuery;
    strQuery = "INSERT INTO asterCall(callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, asterOrgUser)"
               "VALUES(:callerPhone, :callerName, :agentPhone, :agentName, :dateTime, :asterUnicID, :asterChannelState, :asterOrgUser)";
    emit setAsterRec(strQuery, callerPhone, callerName, agentPhone, agentName, dateTime, asterUnicID, asterChannelState, strDT);
    return true;
}

void GeneralSocket::setAsterHost(QString hostName)
{
    callerIDNumList.clear();
    connectedLineNumList.clear();
    QStringList hostNames;
    hostNames.append("voip.rev.550550.ru");
    hostNames.append("voip.nsk.550550.ru");
    hostNames.append("voip.omsk.550550.ru");
    switch (hostNames.indexOf(hostName)) {
    case 0: {
        callerIDNumList.append("CallerIDNum: 123");
        callerIDNumList.append("CallerIDNum: 129");
        callerIDNumList.append("CallerIDNum: 140");
        callerIDNumList.append("CallerIDNum: 142");
        callerIDNumList.append("CallerIDNum: 154");
        connectedLineNumList.append("ConnectedLineNum: 123");
        connectedLineNumList.append("ConnectedLineNum: 129");
        connectedLineNumList.append("ConnectedLineNum: 140");
        connectedLineNumList.append("ConnectedLineNum: 142");
        connectedLineNumList.append("ConnectedLineNum: 154");
        break;
    }
    case 1: {
        callerIDNumList.append("CallerIDNum: 163");
        connectedLineNumList.append("ConnectedLineNum: 163");
        break;
    }
    case 2: {
        callerIDNumList.append("CallerIDNum: 136");
        connectedLineNumList.append("ConnectedLineNum: 136");
    }
    }
    asterSocket = new QTcpSocket(this);
    asterSocket->connectToHost(hostName, 5038, QIODevice::ReadWrite);
    if (asterSocket->waitForReadyRead(300))
    {
        qDebug() << "Connected";
    } else {
        qDebug() << "connection failed";
    };
    asterSocketLogin();
    connect(asterSocket, SIGNAL(readyRead()), this, SLOT(asterSocketRead()));
    asterSocket->waitForReadyRead();
}
