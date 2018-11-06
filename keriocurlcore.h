#ifndef KERIOCURLCORE_H
#define KERIOCURLCORE_H

#include <QObject>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <cstdlib>
#include <string>
#include <stdio.h>
#include <iostream>

#include <curl/curl.h>

#include "jsonrpcparams.h"
#include "jsonrpcengine.h"

class KerioCurlCore : public QObject
{
    Q_OBJECT
public:
    explicit KerioCurlCore(QObject *parent = nullptr);
    ~KerioCurlCore();
    static size_t writer(char* contents, size_t nmemb, size_t size, std::string *data);
    bool prepare(KerioMethod method, QHash<KerioParams, QVariant> params);
    bool setUrl(std::string host, int port, std::string target);
    bool setAppInfo(QString app, QString vendor, QString rev);
    QHash<QString, QVariant> perform();
    bool read_cookies(CURL* curl);
signals:

public slots:
private:
    QString performQuery();
    CURL* curlCore;
    std::string global_url = "";
    std::string x_token = "";
    std::string post_request;
    JsonRpcEngine* jRpcEngine;
    KerioMethod _method_;
    char* _cookies;
};

#endif // KERIOCURLCORE_H
