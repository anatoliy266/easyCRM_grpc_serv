#ifndef GENERALSERVERSOCKET_H
#define GENERALSERVERSOCKET_H

#include <QObject>
#include <QDataStream>
#include <QTcpSocket>
#include <QSqlRecord>
#include <QHostAddress>
#include <QList>

class GeneralServerSocket : public QObject
{
    Q_OBJECT
public:
    explicit GeneralServerSocket(QObject *parent = nullptr, QString server_address, quint16 port);
    ~GeneralServerSocket();
    void Run();
    void addListeningPort(QString server_address, quint16 port);


signals:

public slots:

private:
    QTcpSocket* socket;
};

#endif // GENERALSERVERSOCKET_H
