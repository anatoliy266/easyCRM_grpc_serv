#include <QCoreApplication>
#include "generalworker.h"
#include "generalnskworker.h"
#include "generalomskworker.h"
#include "listenworker.h"
#include "transportobject.h"
#include <QThread>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread* thread = new QThread;
    QThread* thread1 = new QThread;
    QThread* thread2 = new QThread;
    QThread* thread3 = new QThread;

    GeneralWorker* generalWork = new GeneralWorker();
    ListenWorker* listenWork = new ListenWorker();
    GeneralNSKWorker* generalNSKWork = new GeneralNSKWorker();
    GeneralOMSKWorker* generalOMSKWork = new GeneralOMSKWorker();
    TransportObject* transportObj = new TransportObject();

    QObject::connect(thread1, SIGNAL(started()), listenWork, SLOT(createListenSocket()));
    QObject::connect(thread, SIGNAL(started()), generalWork, SLOT(createGeneralSocket()));
    QObject::connect(thread2, SIGNAL(started()), generalNSKWork, SLOT(createGeneralSocket()));
    QObject::connect(thread3, SIGNAL(started()), generalOMSKWork, SLOT(createGeneralSocket()));

    QObject:: connect(generalWork, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)), transportObj, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)));
    QObject::connect(generalNSKWork, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)), transportObj, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)));
    QObject::connect(generalOMSKWork, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)), transportObj, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)));
    QObject::connect(transportObj, SIGNAL(setAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)), listenWork, SLOT(getAsterRec(QString,quint64,quint64,quint64,quint64,quint64,quint64,QString,QString)));


    generalWork->moveToThread(thread);
    listenWork->moveToThread(thread1);
    generalNSKWork->moveToThread(thread2);
    generalOMSKWork->moveToThread(thread3);


    thread->start();
    thread1->start();
    thread2->start();
    thread3->start();


    return a.exec();
}
