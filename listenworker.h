#ifndef LISTENWORKER_H
#define LISTENWORKER_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include "listensocket.h"

class ListenWorker: public QObject
{
    Q_OBJECT
public:
    explicit ListenWorker(QObject* parent = nullptr);
    ~ListenWorker();

signals:
public slots:
    void createListenSocket();
};

#endif // LISTENWORKER_H
