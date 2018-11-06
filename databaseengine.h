#ifndef DATABASEENGINE_H
#define DATABASEENGINE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlRecord>
#include <QByteArray>
#include <QDataStream>
#include <QSqlRelationalTableModel>
#include <QStringList>
#include <QString>
#include <QVariant>
#include <QHash>
#include <QMap>

class DataBaseEngine: public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit DataBaseEngine(QObject* parent = nullptr, QSqlDatabase* db = new QSqlDatabase());
    ~DataBaseEngine();
    bool select();
    QHash<int, QByteArray> roleNames() const {return role_names;}
    QVariant data(const QModelIndex &item, int role) const;
    bool setTablesList(QStringList tables);

    int _rows() {return this->rowCount();}
    int _cols() {return this->columnCount();}
signals:
    void upd();
public slots:
    void updateModel();
    QByteArray dBProceed(int id, QString tablename, QString filter);
private:
    void generateRoleNames();
    QHash<int, QByteArray> role_names;
    QStringList tablesList;

    QList<QVariant> asterPast;
    QList<QVariant> crmPast;


};

#endif // DATABASEENGINE_H
