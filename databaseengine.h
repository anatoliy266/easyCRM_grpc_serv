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

class DataBaseEngine: public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    explicit DataBaseEngine(QObject* parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~DataBaseEngine();
    bool select();
    QHash<int, QByteArray> roleNames() const {return role_names;}
    QVariant data(const QModelIndex &item, int role) const;
signals:
    void generateModel();
    void update();
    void dataChanged(bool checkChanged, int id);
    void dbData(QByteArray arr, bool checkchanged, int rowsCount, int colsCount, QString table);
public slots:
    void dBProceed(int id, QString tablename, QString filter);
    void setToClient(bool checkShanged, int id);
private slots:
    void updateModel();
private:
    void generateRoleNames();
    QHash<int, QByteArray> role_names;
    int userPastRowCount;
    int userPastColCount;
    int crmPastRowCount;
    int crmPastColCount;
    int asterPastRowCount;
    int asterPastColCount;


};

#endif // DATABASEENGINE_H
