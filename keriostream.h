#ifndef KERIOSTREAM_H
#define KERIOSTREAM_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QDateTime>


#include <cstdlib>
#include <string>
#include <stdio.h>
#include <array>

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/system.h>

#include "keriocurlcore.h"


using namespace std;

class KerioStream: public QObject
{
    struct MemoryStruct{
        char* memory;
        size_t size;
    };
public:
    explicit KerioStream(QObject *parent = nullptr);
    ~KerioStream();
    bool connectToHost(std::string hostName, int port, std::string target);
    QString authorization(std::string user, std::string pwd, std::string app = "", std::string vendor = "", QString rev = "1.0");
    QList<QVariant> getCallHistory(QString token, int start = 0, int limit =200, QString orderField = "TIMESTAMP", OrderDirection direction = JParam::Desc);
    QList<QVariant> getQueueUsers(QString token, int extension = 24);
    bool logOut(QString token);

private:
    std::string x_token = "";
    KerioCurlCore* core;
};

#endif // KERIOSTREAM_H
