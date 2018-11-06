#ifndef JSONRPCPARAMS_H
#define JSONRPCPARAMS_H

#include <QObject>

namespace JParam {
enum KerioMethod {
    NoMethod,
    LogIn,
    LogOff,
    Queue,
    CallHistory,
    DownloadRec
};
enum KerioParams {
    Query,
    Login,
    Pwd,
    Application,
    Extension,
    Start,
    Limit,
    OrderField,
    Direction,
    RecordId,
    Transcode,
    Fields,
    TimeFrom,
    TimeTo,
    ConditionField,
    Token
};
enum OrderDirection {
    Asc,
    Desc
};


class JsonRpcParams : public QObject
{
    Q_OBJECT
public:
    explicit JsonRpcParams(QObject *parent = nullptr):QObject(parent){}
    ~JsonRpcParams(){}
};
};



#endif // JSONRPCPARAMS_H
