#include <QCoreApplication>

//#include "generalworker.h"
#include "listenworker.h"

#include <QThread>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>

#include <stdlib.h>

void debug()
{
    qDebug() << "thread finished!!!!!!!";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<qintptr>("qintptr");

//    QThread* thread = new QThread();
//    QThread* thread1 = new QThread();

//    GeneralWorker* generalWork = new GeneralWorker();
    ListenWorker* listenWork = new ListenWorker();
    listenWork->createListenSocket();

//    QObject::connect(thread1, SIGNAL(started()), listenWork, SLOT(createListenSocket()));
//    QObject::connect(thread, SIGNAL(started()), generalWork, SLOT(start()));

//    generalWork->moveToThread(thread);
//    listenWork->moveToThread(thread1);

//    thread->start();
//    thread1->start();

    return a.exec();
}
