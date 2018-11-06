#include "newconnectionworker.h"

NewConnectionWorker::NewConnectionWorker(QObject *parent):QObject(parent)
{

}

NewConnectionWorker::~NewConnectionWorker()
{

}

QSqlDatabase* NewConnectionWorker::prepareDBConnection()
{
    auto* db1 = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db1->setDatabaseName("P://1//123.db");
    return db1;
}

void NewConnectionWorker::listenRead()
{
    //read from\write to socket
    qDebug() << Q_FUNC_INFO;

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    dStruct data;
    QByteArray requestStr = socket->readAll();
    QDataStream out(requestStr);
    quint8 _size;
    out.device()->atEnd();
    out >> _size;
    out.device()->atEnd();
    out >> data._table;
    out.device()->atEnd();
    out >> data._rows;
    out.device()->atEnd();
    out >> data._cols;
    out.device()->atEnd();
    out >> data._query;

    qDebug() << data._cols << data._rows << data._table << data._query;

    QByteArray _result;

    auto database = this->prepareDBConnection();

    if (database->open())
    {
        DataBaseEngine* dbEngine = new DataBaseEngine(this, database);
        _result = dbEngine->dBProceed(0, data._table, data._query);

        database->close();
        delete dbEngine;
    }
    //qDebug() << _result;
    QString connection = database->connectionName();
    delete database;
    QSqlDatabase::removeDatabase(connection);

    auto result = _result;
    QDataStream in(&result, QIODevice::ReadWrite);
    quint8 _blockSize;
    int _id_;
    int _rows_ = 0;
    int _cols_ = 0;
    in.device()->atEnd();
    in >> _blockSize;
    in.device()->atEnd();

    in >> _id_;
    in.device()->atEnd();
    in >> _rows_;
    in.device()->atEnd();
    in >> _cols_;

    qDebug() << _id_ << _rows_ << _cols_ << _blockSize;

    if (data._rows != _rows_ || data._cols != _cols_)
    {
        qDebug() << "data.rows != _rows_ || data._cols != _cols_";
        socket->write(_result);
        socket->flush();
    } else {
        qDebug() << "data._rows/_cols == _rows_/_cols_";
        socket->write(QByteArray("nothing change"));
        socket->flush();
    }
    qDebug() << "it's not crashed, wow!";
    socket->disconnectFromHost();
    socket = nullptr;
    this->thread()->quit();
}

void NewConnectionWorker::newIncomingConnection(qintptr socketDescriptor)
{
    // create socket, set descriptor from main thread, connect signals from socket
    qDebug() << "NewConnectionWorker::newIncomingConnection slot called";
    QTcpSocket* newConWorkSocket = new QTcpSocket(this);
    newConWorkSocket->setSocketDescriptor(socketDescriptor, QAbstractSocket::ConnectedState, QIODevice::ReadWrite);
    QObject::connect(newConWorkSocket, SIGNAL(readyRead()), this, SLOT(listenRead()));
    qDebug() << newConWorkSocket->bytesAvailable() << "bytes for read";
    qDebug() << newConWorkSocket->isReadable();
    qDebug() << newConWorkSocket->socketType();
    qDebug() << newConWorkSocket->state();
}
