#include "databaseengine.h"
#include <QDebug>

DataBaseEngine::DataBaseEngine(QObject* parent, QSqlDatabase db): QSqlRelationalTableModel(parent, db)
{
    //emit generateModel();
    connect(this, SIGNAL(update()), this, SLOT(updateModel()));
    connect(this, SIGNAL(dataChanged(bool,int)), this, SLOT(setToClient(bool,int)));

}

DataBaseEngine::~DataBaseEngine()
{

}

bool DataBaseEngine::select()
{
    bool tQ = QSqlRelationalTableModel::select();
    if (tQ)
        generateRoleNames();
    return tQ;
}

void DataBaseEngine::generateRoleNames()
{
    role_names.clear();
    for (int i = 0; i<record().count(); i++)
    {
        role_names[Qt::UserRole + i + 1]=record().fieldName(i).toUtf8();
    }
}

QVariant DataBaseEngine::data(const QModelIndex &item, int role) const
{
    if (item.row() >= rowCount())
            return QString();

        if (role < Qt::UserRole)
            return QSqlQueryModel::data(item, role);
        else
        {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(item.row(), columnIdx);
            return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
}

void DataBaseEngine::updateModel()
{
    this->select();
    while(this->canFetchMore())
    {
        this->fetchMore();
    }
}

void DataBaseEngine::dBProceed(int id, QString tablename, QString filter)
{
    this->setTable(tablename);
    QStringList tablesList;
    tablesList.append("asterCall");
    tablesList.append("crm");
    tablesList.append("orgsPhones");
    tablesList.append("query_type");
    tablesList.append("time");
    tablesList.append("users");
    switch (tablesList.indexOf(tablename)) {
    case 0: {
        this->setRelation(2, QSqlRelation("orgsPhones", "orgPhone", "orgName"));
        this->setRelation(4, QSqlRelation("agentPhones", "agentPhone", "agentsName"));
        this->setRelation(5, QSqlRelation("time", "unixTime", "strTime"));
        //this->setFilter(filter);
        emit update();
        this->setFilter(filter);
        emit update();
        if (asterPastRowCount != this->rowCount() || asterPastColCount != this->columnCount())
        {

            asterPastRowCount = this->rowCount();
            asterPastColCount = this->columnCount();
            emit dataChanged(true, id);
        } else {
            emit dataChanged(false, id);
        }
        break;
    }

    case 1: {

        this->setRelation(1, QSqlRelation("users", "id_user", "user"));
        this->setRelation(2, QSqlRelation("time", "unixTime", "strTime"));
        this->setFilter(filter);
        emit update();
        if (crmPastRowCount != this->rowCount() || crmPastColCount != this->columnCount())
        {

            crmPastRowCount = this->rowCount();
            crmPastColCount = this->columnCount();
            emit dataChanged(true, id);
        } else {
            emit dataChanged(false, id);
        }
        break;
    }

    case 2: {
        break;
    }
    case 3: {
        emit update();
        emit dataChanged(true, id);
        break;
    }
    case 4: {
        break;
    }
    case 5: {
        this->setFilter(filter);
        emit update();
        if (userPastRowCount != this->rowCount() || userPastColCount != this->columnCount())
        {
            crmPastRowCount = 0;
            crmPastColCount = 0;
            asterPastRowCount = 0;
            asterPastColCount = 0;

            userPastRowCount = this->rowCount();
            userPastColCount = this->columnCount();
            emit dataChanged(true, id);
        } else {
            crmPastRowCount = 0;
            crmPastColCount = 0;
            asterPastRowCount = 0;
            asterPastColCount = 0;
            emit dataChanged(true, id);
        }
        break;
    }
    }
}

void DataBaseEngine::setToClient(bool checkShanged, int id)
{
    if (checkShanged)
    {
        QByteArray tVals;
        QDataStream out(&tVals, QIODevice::ReadWrite);
        out << quint8(0);
        out << id;
        out << this->roleNames();
        out << this->rowCount();
        out << this->columnCount();
        QVariant data;
        for (int i = 0; i< this->rowCount(); i++)
        {
            for (int j = 0; j< this->columnCount(); j++)
            {
                data = this->data(this->index(i, j), Qt::DisplayRole);
                out << data;
            }
        }
        out.device()->seek(0);
        out << quint8(tVals.size()-sizeof(quint8));

        emit dbData(tVals, checkShanged, this->rowCount(), this->columnCount(), this->tableName());
    }
}

