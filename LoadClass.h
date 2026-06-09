#ifndef LOADCLASS_H
#define LOADCLASS_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>

class LoadClass {
public:
    LoadClass(QSqlDatabase &db);
    bool loadFromCsv(const QString &fileName);

private:
    int getOrInsertShipType(const QString &typeName);
    QSqlDatabase m_db;
    int getOrInsertDesigner(const QString &name);
    int getOrInsertModel(const QString &modelName, int designerId);
    double parseDouble(QString value);
    QStringList parseCSVLine(const QString &line); // Локальная копия парсера для автономности
};

#endif // LOADCLASS_H
