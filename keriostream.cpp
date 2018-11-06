#include "keriostream.h"


KerioStream::KerioStream(QObject *parent): QObject(parent)
{
//    qDebug() << "KerioStream created";
}

KerioStream::~KerioStream()
{

}

/*
POST /admin/api/jsonrpc/ HTTP/1.1
Accept: application/json-rpc
Content-Type: application/json-rpc; charset=UTF-8
Host: fw.company.tld:4081
Content-Length: 234
Connection: close
*/


bool KerioStream::connectToHost(std::string hostName, int port, string target)
{
    //qDebug() << "specifying host";
    bool check = false;
    core = new KerioCurlCore();
    if(core->setUrl(hostName, port, target))
    {
//        qDebug() << "Connected";
        check = true;
    } else {
//        qDebug() << "Connection failed";
        check = false;
    }
    return check;

}

//string login_json = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"Session.login\",\"params\":{\"userName\":\"a.stanevich\",\"password\":\"256532fdnjvfn73501505\",\"application\":{\"name\":\"KerioOperatorServ\",\"vendor\":\"King Anthony\",\"version\":\"0.1\"}}}";

QString KerioStream::authorization(std::string user, std::string pwd, std::string app, std::string vendor, QString rev)
{
    //qDebug() << "autorization started";
    QHash<KerioParams, QVariant> params;
    params[Login] = QString::fromStdString(user);
    params[Pwd] = QString::fromStdString(pwd);
    core->setAppInfo(QString::fromStdString(app), QString::fromStdString(vendor), rev);
    core->prepare(LogIn, params);
    QHash<QString, QVariant> result = core->perform();
    return result["token"].toString();

}

bool KerioStream::logOut(QString token)
{
    bool logout = false;
    QHash<KerioParams, QVariant> params;
    params[Token] = token;
    core->prepare(LogOff, params);
    QHash<QString, QVariant> result = core->perform();
    if (result["redirectUrl"].toString() == "https://ats.550550.ru:4021/admin/login")
    {
        logout = true;
    } else {
        logout = false;
    }
    return  logout;
}

QList<QVariant> KerioStream::getCallHistory(QString token, int start, int limit, QString orderField, OrderDirection direction)
{
    QList<QVariant> resultList;
    resultList.clear();
    QHash<KerioParams, QVariant> params;
    params[Token] = token;
    params[Start] = start;
    params[Limit] = limit;
    params[OrderField] = orderField;
    params[Direction] = direction;
    core->prepare(CallHistory, params);
    QHash<QString, QVariant> result = core->perform();
    int totalItems = result["totalItems"].toInt();
    if (totalItems == 0)
    {
        resultList.append(totalItems);
    } else {
        for (int i = 0; i < totalItems; i++)
        {
            QString iter = QString::number(i);
            resultList.append(result[iter]);
        }
    }
    return resultList;
}

QList<QVariant> KerioStream::getQueueUsers(QString token, int extension)
{
    QList<QVariant> resultList;
    resultList.clear();
    QHash<KerioParams, QVariant> params;
    params[Token] = token;
    params[Extension] = extension;
    core->prepare(Queue, params);
    QHash<QString, QVariant> result = core->perform();
    int totalItems = result["totalItems"].toInt();
    //qDebug() << "total items : " << totalItems;
    if (totalItems == 0)
    {
        resultList.append(0);
    } else {
        for (int i = 0; i < totalItems; i++)
        {
            QString iter = QString::number(i);
            //qDebug() << result[iter];
            resultList.append(result[iter]);
        }
    }
    return resultList;
}
