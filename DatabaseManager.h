#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectToDatabase(const QString &host,
                           const QString &databaseName,
                           const QString &user,
                           const QString &password,
                           int port = 5432);

    void disconnectFromDatabase();
    bool isConnected() const;

    // Получить все суда
    QSqlQueryModel* getAllShips();

    // Получить суда-прототипы (sister ships)
    QSqlQueryModel* getSisterShips();

    // Получить детальную информацию о судах с прототипами
    QSqlQueryModel* getShipsWithSisterInfo();

    // Получить информацию о двигателях
    QSqlQueryModel* getShipsWithEngines();

    QString lastError() const;

private:
    QSqlDatabase m_db;
    QString m_lastError;
};

#endif // DATABASEMANAGER_H
