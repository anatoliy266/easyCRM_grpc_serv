#include "jsonrpcengine.h"

JsonRpcEngine::JsonRpcEngine(QObject *parent) : QObject(parent)
{

}

JsonRpcEngine::~JsonRpcEngine()
{

}

bool JsonRpcEngine::method(KerioMethod method)
{
    QString _p_str = "\"%1\":\"%2\"";
    QString _pa_str = "\"%1\":[%2]";
    QString _po_str = "\"%1\":%2";
    QString _ptry_str = "\"%1\":%2";
    QString _string = "";
    QString jBody = "";
    mtd = method;
    bool proceed = false;
    switch(method)
    {
    case LogIn:
    {
        QString _app = "{" + _p_str.arg("name").arg("%3") +","+_p_str.arg("vendor").arg("%4") +","+_p_str.arg("version").arg("%5") + "}";
        QString _param = "{" + _p_str.arg("userName").arg("%1") +","+_p_str.arg("password").arg("%2") +","+_ptry_str.arg("application").arg(_app) + "}";

        jBody = "{" + _p_str.arg("jsonrpc").arg("2.0") +","+_ptry_str.arg("id").arg(1) +","+_p_str.arg("method").arg("Session.login") +","+_ptry_str.arg("params").arg("%1")+ "}";

        _string = jBody.arg(_param);
        proceed = true;
        break;
    };
    case LogOff:
    {
        jBody = "{" + _p_str.arg("jsonrpc").arg("2.0") +","+_p_str.arg("id").arg(1) +","+_p_str.arg("token").arg("%1")+","+_p_str.arg("method").arg("Session.logout")+"}";

        _string = jBody;
        //ad4d65f2810e4ae273df1b957c32fc85
        proceed = true;
        break;
    };
    case CallHistory:
    {


        //QString _conditionF = "{"+_p_str.arg("fieldname").arg("%7") + ","+_p_str.arg("comparator").arg("GreaterThan") + ","+_p_str.arg("value").arg("%8") +"}";
        //QString _conditionT = "{"+_p_str.arg("fieldname").arg("%9") + ","+_p_str.arg("comparator").arg("GreaterThan") + ","+_p_str.arg("value").arg("%10") +"}";
        QString _order = "{"+_p_str.arg("columnName").arg("%4") + "," + _p_str.arg("direction").arg("%5") +"}";
        QString _query = "{" + _p_str.arg("limit").arg("%2") + "," + _p_str.arg("start").arg("%3") + "," + _pa_str.arg("orderBy").arg(_order) +"}";
        QString _property = "{" + _po_str.arg("query").arg(_query) + "}";

        jBody = "{" + _p_str.arg("jsonrpc").arg("2.0") +","+_po_str.arg("id").arg("1") +","+_p_str.arg("token").arg("%1") +","+_p_str.arg("method").arg("Status.getCalls")+","+_po_str.arg("params").arg(_property)+"}";

        _string = jBody;

        proceed = true;
        break;
    };
    case Queue:
    {
        QString _query = "{}";
        QString _property = "{" + _po_str.arg("query").arg(_query) + "," + _po_str.arg("extension").arg("%2") + "}";

        jBody = "{" + _p_str.arg("jsonrpc").arg("2.0") +","+_po_str.arg("id").arg("1") +","+_p_str.arg("token").arg("%1") +","+_p_str.arg("method").arg("Status.getQueueAgents")+","+_po_str.arg("params").arg(_property)+"}";

        _string = jBody;

        proceed = true;
        break;
    };
    case NoMethod:
    {
        proceed = false;
        break;
    };
    case DownloadRec:
    {


        QString _property = "{" + _p_str.arg("id").arg("%2") + "," + _p_str.arg("transcode").arg("false") + "}";

        jBody = "{" + _po_str.arg("jsonrpc").arg("2.0") +","+_p_str.arg("id").arg(1) +","+_p_str.arg("token").arg("%1") +","+_p_str.arg("method").arg("Recordings.downloadAudioContent")+","+_po_str.arg("params").arg(_property)+"}";

        _string = jBody;

        proceed = true;
        break;
    };
    }
    if (proceed)
    {
        //_string = "{jsonrpc:2.0,\"id\":1,\"method\":\"Session.login\",\"params\":{\"userName\":\"a.stanevich\",\"password\":\"256532fdnjvfn73501505\",\"application\":{\"name\":\"KerioOperatorServ\",\"vendor\":\"King Anthony\",\"version\":\"0.1\"}}}";
        result = _string;
    }
    return proceed;
}

bool JsonRpcEngine::setToken(QString token)
{
    x_token = token;
    return true;
}

bool JsonRpcEngine::removeToken()
{
    x_token = "";
    return true;
}

QString JsonRpcEngine::token()
{
    return x_token;
}

bool JsonRpcEngine::setParameter(KerioParams param, QString val)
{
    QVariant var(val);
    paramList[param] = var;
    return true;

}

bool JsonRpcEngine::setParameter(KerioParams param, QStringList val)
{
    QVariant var(val);
    paramList[param] = var;
    return true;
}

bool JsonRpcEngine::setParameter(KerioParams param, int val)
{
    QVariant var(val);
    paramList[param] = var;
    return true;
}

bool JsonRpcEngine::setParameter(KerioParams param, QVariant val)
{
    paramList[param] = val;
    return true;
}

bool JsonRpcEngine::setParameter(QHash<KerioParams, QVariant> params)
{
    int large;
    int less;
    QList<KerioParams> paramsA = paramList.keys();
    QList<KerioParams> paramsB = params.keys();
    if (paramsA.count() > paramsB.count())
    {
        large = paramsA.count();
        less = paramsB.count();
        for (int i = 0; i < large; i++)
        {
            for (int j = 0; j < less; j++)
            {
                if (paramsA.value(i) == paramsB.value(j))
                {
                    paramList.remove(paramsA.value(i));
                }
            }
        }
    }

    else {
        large = paramsB.count();
        less = paramsA.count();
        for (int i = 0; i < large; i++)
        {
            for (int j = 0; j < less; j++)
            {
                if (paramsA.value(j) == paramsB.value(i))
                {
                    paramList.remove(paramsA.value(j));
                }
            }
        }
    }
    paramList.unite(params);
    return true;
}

bool JsonRpcEngine::replaceParameter(KerioParams param)
{
    paramList.remove(param);
    return true;
}

void JsonRpcEngine::clear()
{
    paramList.clear();
}

std::string JsonRpcEngine::generate()
{
    QString arg1 = "";
    QString arg2 = "";
    QString arg3 = "";
    QString arg4 = "";
    QString arg5 = "";
    QString arg6 = "";
    QString arg7 = "";
    QString arg8 = "";
    QString arg9 = "";
    QString arg10 = "";
    QString _json = result;
    QString result_json = "";
    if (mtd != NoMethod)
    {
        if (checkRequiredParams(mtd))
        {
            switch (mtd) {
            case LogIn:
            {
                arg1 = paramList[Login].toString();
                arg2 = paramList[Pwd].toString();
                arg3 = appName;
                arg4 = appVendor;
                arg5 = appRev;
                result_json = _json.arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5);
                break;
            };
            case LogOff:
            {
                arg1 = paramList[Token].toString();
                result_json = _json.arg(arg1);
                break;
            };
            case CallHistory:
            {
                arg1 = paramList[Token].toString();
                arg2 = QString::number(-1);
                arg3 = QString::number(0);
                arg4 = paramList[OrderField].toString();

                if(paramList[Direction].toInt() == 1)
                    arg5 = "Desc";
                else
                    arg5 = "Asc";


                result_json = _json.arg(arg1).arg(arg2).arg(arg3).arg(arg4).arg(arg5);
                break;
            };
            case Queue:
            {
                arg1 = paramList[Token].toString();
                arg2 = paramList[Extension].toString();
                result_json = _json.arg(arg1).arg(arg2);
                break;
            };
            case DownloadRec:
            {
                arg1 = paramList[Token].toString();
                arg2 = paramList[RecordId].toString();
                arg3 = paramList[Transcode].toString();
                result_json = _json.arg(arg1).arg(arg2);
                break;
            };
            default:
                break;
            }
        } else {
            qDebug() << "Not enought parameters!";
        }
        return result_json.toStdString();// = "{\"jsonrpc\":2.0,\"id\":1,\"method\":\"Session.login\",\"params\":{\"userName\":\"a.stanevich\",\"password\":\"256532fdnjvfn73501505\",\"application\":{\"name\":\"KerioOperatorServ\",\"vendor\":\"King Anthony\",\"version\":\"0.1\"}}}";;
    } else {
        qDebug() << "Method are not specified!";
        return "";
    }
}

void JsonRpcEngine::setAppInfo(QString name, QString vendor, QString rev)
{
    appName = name;
    appVendor = vendor;
    appRev = rev;
}

bool JsonRpcEngine::checkRequiredParams(KerioMethod method)
{
    bool proceed = false;
    bool check = false;
    switch (method) {
    case LogIn:
    {
        if (paramList[Login] != NULL)
        {
            check = true;
        } else {
            check = false;
            break;
        }
        if (paramList[Pwd] != NULL)
        {
            check = true;
        } else {
            check = false;
            break;
        }
        if (appName == "" || appVendor == "" || appRev == "")
        {
            setAppInfo();
        }
        break;
    };
    case LogOff:
    {
        if (paramList[Token] == NULL)
        {
            check = false;
        } else {
            check = true;
        }
        break;
    };
    case CallHistory:
    {
        if (paramList[Token] == NULL)
        {
            check = false;
        } else {
            if (paramList[Limit] == "")
            {
                paramList[Limit] = 200;
            }
            if (paramList[Start] == "")
            {
                paramList[Start] = 0;
            }
            if (paramList[OrderField] == "")
            {
                paramList[OrderField] = "ANSWERED_DURATION";
            }
            if (paramList[Direction] == NULL)
            {
                paramList[Direction] = "Desc";
            }
            if (paramList[Fields] == "")
            {
                QStringList list;
                list.append("TIMESTAMP,");
                list.append(" FROM_NUM,");
                list.append(" TO_NUM,");
                list.append(" STATUS,");
                list.append(" ANSWERED_DURATION");
                paramList[Fields] = list;
            }
            if (paramList[ConditionField] == "")
            {
                paramList[ConditionField] = "TIMESTAMP";
            }
            if (paramList[TimeFrom] == NULL)
            {
                paramList[TimeFrom] = 0;
            }
            if (paramList[TimeTo] == NULL)
            {
                QDateTime dt = QDateTime::currentDateTime();
                paramList[TimeTo] = dt.toTime_t();
            }
            check = true;
        }


        break;
    };
    case Queue:
    {
        if (paramList[Token] == NULL)
        {
            check = false;
        } else {
            if (paramList[Extension] == NULL)
            {
                check = false;
                break;
            } else {
                check = true;
            }
        }
        break;
    };
    case NoMethod:
    {
        check = false;
        break;
    };
    case DownloadRec:
    {
        if (paramList[Token] == NULL)
        {
            check = false;
        } else {
            if (paramList[RecordId] == NULL)
            {
                check = false;
                break;
            } else {
                if (paramList[Transcode] == NULL)
                {
                    paramList[Transcode] = false;
                }
                check = true;
            }
        }
        break;
    };
    }
    if (check)
        proceed = true;
    else
        proceed = false;
    return proceed;
}


QHash<QString, QVariant> JsonRpcEngine::parse(QString jString)
{
    QHash<QString, QVariant> result;
    QHash<QString, QVariant> token_hash;
    QHash<QString, QVariant> fields_values;
    QHash<QString, QVariant> agents_values;
    QHash<QString, QVariant> calls_values;
    QHash<QString, QVariant> error_hash;
    QJsonDocument jDoc = QJsonDocument::fromJson(jString.toUtf8());
    auto resultJObj = jDoc.object()["result"].toObject();
    auto jKeys = jDoc.object()["result"].toObject().toVariantHash().keys();
    for (int i = 0; i < jKeys.count(); i++)
    {
        if (jKeys.value(i) == "token")
        {
            token_hash[jKeys.value(i)] = resultJObj[jKeys.value(i)].toString();
            result = token_hash;
            break;
        } else {
            if (jKeys.value(i) == "callHistory")
            {
                int totalItems = 0;
                if (!resultJObj["totalItems"].isNull())
                    totalItems = resultJObj["totalItems"].toInt();
                auto callHistoryStruct = resultJObj["callHistory"];

                if (totalItems == 0)
                {
                    fields_values["totalItems"] = 0;
                } else {
                    fields_values["totalItems"] = paramList[Limit];
                    auto callHistoryArray = callHistoryStruct.toArray();
                    for (int j = 0; j < totalItems; j++)
                    {
                        auto members = callHistoryArray.at(j).toObject().toVariantHash().keys();
                        auto values = callHistoryArray.at(j).toObject().toVariantHash().values();
                        QStringList row;
                        for (int q = 0; q < members.count(); q++)
                        {
                            row.append(members.value(q) + "@" + values.value(q).toString());
                        }
                        fields_values[QString::number(j)] = row;
                        row.clear();
                    }
                    result = fields_values;
                    break;
                }
            }
            if (jKeys.value(i) == "agents")
            {
                int totalItems = 0;
                if (!resultJObj["totalItems"].isNull())
                {
                    totalItems = resultJObj["totalItems"].toInt();
                }
                auto agentsStruct = resultJObj["agents"];
                if (totalItems == 0)
                {
                    agents_values["totalItems"] = 0;
                } else {
                    agents_values["totalItems"] = totalItems;
                }
                auto agentsArray = agentsStruct.toArray();
                for (int j = 0; j < totalItems; j++)
                {
                    auto members = agentsArray.at(j).toObject().toVariantHash().keys();
                    auto values = agentsArray.at(j).toObject().toVariantHash().values();
                    QStringList row;
                    for (int q = 0; q < members.count(); q++)
                    {
                        row.append(members.value(q) + "@" + values.value(q).toString());
                    }
                    agents_values[QString::number(j)] = row;
                    row.clear();
                }
                result = agents_values;
                break;
            }
            if (jKeys.value(i) == "recordingList")
            {
                break;
            }
            if (jKeys.value(i) == "fileDownload")
            {
                break;
            }
            if (jKeys.value(i) == "calls")
            {
                int totalItems = 0;
                if (!resultJObj["totalItems"].isNull())
                {
                    totalItems = resultJObj["totalItems"].toInt();
                }
                auto callsStruct = resultJObj["calls"];
                if (totalItems == 0)
                {
                    calls_values["totalItems"] = 0;
                } else {
                    calls_values["totalItems"] = totalItems;
                }
                auto callsArray = callsStruct.toArray();
                for (int j = 0; j < totalItems; j++)
                {
                    auto members = callsArray.at(j).toObject().toVariantHash().keys();
                    auto values = callsArray.at(j).toObject().toVariantHash().values();
                    QStringList row;
                    for (int q = 0; q < members.count(); q++)
                    {
                        if (members.value(q) == "FROM")
                        {
                            auto fromStruct = callsArray.at(j).toObject()["FROM"].toObject();
                            row.append(members.value(q) + "@" + fromStruct["NUMBER"].toString());
                        } else {
                            if (members.value(q) == "TO")
                            {
                                auto toStruct = callsArray.at(j).toObject()["TO"].toObject();
                                row.append(members.value(q) + "@" + toStruct["NUMBER"].toString());
                            } else {
                                row.append(members.value(q) + "@" + values.value(q).toString());
                            }
                        }
                    }
                    calls_values[QString::number(j)] = row;
                    row.clear();
                }
                result = calls_values;
                break;
            }
            if (jKeys.count() == 0)
            {
                result["redirectUrl"]  = "https://ats.550550.ru:4021/admin/login";
                break;
            }
        }
    }
    return result;
}





