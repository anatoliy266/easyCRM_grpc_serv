#ifndef JSONRPCENGINE_H
#define JSONRPCENGINE_H

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

#include <cstdlib>
#include <string>
#include <stdio.h>

#include "jsonrpcparams.h"

using namespace JParam;

class JsonRpcEngine : public QObject
{
    Q_OBJECT
public:
    explicit JsonRpcEngine(QObject *parent = nullptr);
    ~JsonRpcEngine();
    bool method(KerioMethod method = NoMethod);
    bool setParameter(KerioParams param, QString val);
    bool setParameter(KerioParams param, int val);
    bool setParameter(QHash<KerioParams, QVariant> params);
    bool setParameter(KerioParams param, QVariant val);
    bool setParameter(KerioParams param, QStringList val);
    bool setToken(QString token);
    bool removeToken();
    QString token();
    std::string generate();
    QHash<QString, QVariant> parse(QString jString);
    void clear();
    bool replaceParameter(KerioParams param);
    void setAppInfo(QString name = "\"Default App\"", QString vendor = "\"Default Vendor\"", QString rev = "1.0");
signals:

public slots:
private:
    bool checkRequiredParams(KerioMethod method);
    QHash<KerioParams, QVariant> paramList;

    QString appName;
    QString appVendor;
    QString appRev;
    QString genMethod;
    QString result;
    KerioMethod mtd;
    QString x_token;
};

#endif // JSONRPCENGINE_H
