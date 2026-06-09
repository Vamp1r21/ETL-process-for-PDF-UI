#include "LoadClass.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>

LoadClass::LoadClass(QSqlDatabase &db) : m_db(db) {}

QStringList LoadClass::parseCSVLine(const QString &line) {
    QStringList fields;
    QString field;
    bool inQuotes = false;
    for (int i = 0; i < line.length(); ++i) {
        QChar ch = line[i];
        if (ch == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"'; i++;
            } else inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.append(field);
            field.clear();
        } else field += ch;
    }
    fields.append(field);
    return fields;
}

double LoadClass::parseDouble(QString value) {
    if (value.isEmpty()) return 0.0;
    return value.replace(',', '.').toDouble();
}

int LoadClass::getOrInsertDesigner(const QString &name) {
    QSqlQuery query(m_db);
    query.prepare("SELECT \"ID\" FROM public.\"MainEngineDesigner\" WHERE \"Name\" = ?");
    query.addBindValue(name);
    if (query.exec() && query.next()) return query.value(0).toInt();

    QSqlQuery insert(m_db);
    insert.prepare("INSERT INTO public.\"MainEngineDesigner\" (\"Name\") VALUES (?) RETURNING \"ID\"");
    insert.addBindValue(name);
    if (insert.exec() && insert.next()) return insert.value(0).toInt();

    qDebug() << "Designer Insert Error:" << insert.lastError().text();
    return -1;
}

int LoadClass::getOrInsertModel(const QString &modelName, int designerId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT \"ID\" FROM public.\"MainEngineModel\" WHERE \"NameModel\" = ? AND \"IDDesigner\" = ?");
    query.addBindValue(modelName);
    query.addBindValue(designerId);
    if (query.exec() && query.next()) return query.value(0).toInt();

    QSqlQuery insert(m_db);
    insert.prepare("INSERT INTO public.\"MainEngineModel\" (\"NameModel\", \"IDDesigner\") VALUES (?, ?) RETURNING \"ID\"");
    insert.addBindValue(modelName);
    insert.addBindValue(designerId);
    if (insert.exec() && insert.next()) return insert.value(0).toInt();

    return -1;
}

int LoadClass::getOrInsertShipType(const QString &typeName) {
    if (typeName.trimmed().isEmpty()) return -1;  // возвращаем -1 для пустых типов

    QSqlQuery query(m_db);
    query.prepare("SELECT \"ID\" FROM public.\"ShipType\" WHERE \"Name\" = ?");
    query.addBindValue(typeName.trimmed());
    if (query.exec() && query.next()) return query.value(0).toInt();

    QSqlQuery insert(m_db);
    insert.prepare("INSERT INTO public.\"ShipType\" (\"Name\") VALUES (?) RETURNING \"ID\"");
    insert.addBindValue(typeName.trimmed());
    if (insert.exec() && insert.next()) return insert.value(0).toInt();

    qDebug() << "ShipType Insert Error:" << insert.lastError().text();
    return -1;
}

bool LoadClass::loadFromCsv(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "!!! ФАЙЛ НЕ НАЙДЕН !!! Путь:" << fileName;
        return false;
    }

    QTextStream in(&file);
    in.readLine(); // Пропуск заголовка

    int count = 0;
    int errorCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) continue;

        QStringList fields = parseCSVLine(line);
        if (fields.size() < 17) {  // минимум 17 полей (0..16)
            errorCount++;
            continue;
        }

        QString designerName = fields[13].trimmed();
        QString modelName = fields[14].trimmed();
        int numberOfEngines = fields[15].trimmed().toInt();  // 15-е поле - количество двигателей
        QString shipTypeName = fields[16].trimmed();         // 16-е поле - тип судна

        // Если двигатель пуст — пропускаем
        if (designerName.isEmpty() || modelName.isEmpty()) continue;

        // Начинаем транзакцию для одной строки
        m_db.transaction();

        int designerId = getOrInsertDesigner(designerName);
        int modelId = getOrInsertModel(modelName, designerId);
        int shipTypeId = getOrInsertShipType(shipTypeName);  // может вернуть -1 для пустого типа

        if (designerId == -1 || modelId == -1) {
            m_db.rollback();
            errorCount++;
            continue;
        }

        QSqlQuery shipQuery(m_db);
        shipQuery.prepare("INSERT INTO public.\"Ships\" ("
                          "\"VesselName\", \"Builder\", \"Designer\", \"OwnerOperator\", \"Country\", "
                          "\"DeliveryDate\", \"Length(m)\", \"MaxSpeed(knots)\", \"ImoNumber\", "
                          "\"Gross\", \"SisterShip\", \"Displacement(t)\", \"IDShipType\") "
                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) RETURNING id");

        shipQuery.addBindValue(fields[1]);
        shipQuery.addBindValue(fields[2]);
        shipQuery.addBindValue(fields[3]);
        shipQuery.addBindValue(fields[4]);
        shipQuery.addBindValue(fields[5]);

        QDate date = QDate::fromString(fields[6], "dd.MM.yyyy");
        shipQuery.addBindValue(date.isValid() ? date : QVariant(QVariant::Date));

        shipQuery.addBindValue(parseDouble(fields[7]));
        shipQuery.addBindValue(parseDouble(fields[8]));
        shipQuery.addBindValue(fields[9].isEmpty() ? QVariant(QVariant::Int) : fields[9].toInt());
        shipQuery.addBindValue(parseDouble(fields[10]));
        shipQuery.addBindValue(fields[11].toInt());
        shipQuery.addBindValue(parseDouble(fields[12]));

        // Для пустого типа судна передаём NULL
        if (shipTypeId == -1) {
            shipQuery.addBindValue(QVariant(QVariant::Int));
        } else {
            shipQuery.addBindValue(shipTypeId);
        }

        if (!shipQuery.exec() || !shipQuery.next()) {
            qDebug() << "Ship insert failed:" << shipQuery.lastError().text();
            m_db.rollback();
            errorCount++;
            continue;
        }

        int shipId = shipQuery.value(0).toInt();

        QSqlQuery countQuery(m_db);
        countQuery.prepare("INSERT INTO public.\"NumberOfEngines\" (\"IDShip\", \"IDEngine\", \"Number\") VALUES (?, ?, ?)");
        countQuery.addBindValue(shipId);
        countQuery.addBindValue(modelId);
        countQuery.addBindValue(numberOfEngines);

        if (countQuery.exec()) {
            m_db.commit(); // Сохраняем эту строку
            count++;
        } else {
            qDebug() << "Link insert failed:" << countQuery.lastError().text();
            m_db.rollback();
            errorCount++;
        }
    }

    qDebug() << "=== ИТОГ ЗАГРУЗКИ ===";
    qDebug() << "Успешно:" << count;
    qDebug() << "Ошибок/Пропущено:" << errorCount;

    return (count > 0); // Если хоть что-то загрузилось — успех
}
