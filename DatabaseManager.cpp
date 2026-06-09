#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    disconnectFromDatabase();
}

bool DatabaseManager::connectToDatabase(const QString &host,
                                        const QString &databaseName,
                                        const QString &user,
                                        const QString &password,
                                        int port)
{
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setPort(port);

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qDebug() << "Database connection error:" << m_lastError;
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}

void DatabaseManager::disconnectFromDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::isConnected() const
{
    return m_db.isOpen();
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

QSqlQueryModel* DatabaseManager::getAllShips()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query(m_db);

    // Используем QString с правильным экранированием
    QString queryString =
        "SELECT "
        "s.id, "
        "s.\"VesselName\" AS \"Название судна\", "
        "s.\"Builder\" AS \"Строитель\", "
        "s.\"Designer\" AS \"Проектировщик\", "
        "s.\"OwnerOperator\" AS \"Владелец/Оператор\", "
        "s.\"Country\" AS \"Страна\", "
        "s.\"DeliveryDate\" AS \"Дата поставки\", "
        "s.\"Length(m)\" AS \"Длина (м)\", "
        "s.\"MaxSpeed(knots)\" AS \"Макс. скорость (узлы)\", "
        "s.\"ImoNumber\" AS \"IMO номер\", "
        "s.\"Gross\" AS \"Валовая вместимость\", "
        "s.\"SisterShip\" AS \"ID прототипа\", "
        "s.\"Displacement(kg)\" AS \"Водоизмещение (кг)\", "
        "st.\"Name\" AS \"Тип судна\" "
        "FROM \"Ships\" s "
        "LEFT JOIN \"ShipType\" st ON s.\"IDShipType\" = st.\"ID\" "
        "ORDER BY s.\"VesselName\"";

    if (!query.exec(queryString)) {
        m_lastError = query.lastError().text();
        qDebug() << "Query error:" << m_lastError;
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* DatabaseManager::getSisterShips()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query(m_db);

    QString queryString =
        "SELECT "
        "s1.id, "
        "s1.\"VesselName\" AS \"Название судна\", "
        "s1.\"SisterShip\" AS \"ID прототипа\", "
        "s2.\"VesselName\" AS \"Название прототипа\", "
        "s1.\"Builder\" AS \"Строитель\", "
        "s1.\"Country\" AS \"Страна\", "
        "s1.\"DeliveryDate\" AS \"Дата поставки\" "
        "FROM \"Ships\" s1 "
        "LEFT JOIN \"Ships\" s2 ON s1.\"SisterShip\" = s2.id "
        "WHERE s1.\"SisterShip\" IS NOT NULL "
        "ORDER BY s1.\"VesselName\"";

    if (!query.exec(queryString)) {
        m_lastError = query.lastError().text();
        qDebug() << "Query error:" << m_lastError;
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* DatabaseManager::getShipsWithSisterInfo()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query(m_db);

    QString queryString =
        "SELECT "
        "s.id, "
        "s.\"VesselName\" AS \"Название судна\", "
        "st.\"Name\" AS \"Тип судна\", "
        "s.\"Builder\" AS \"Строитель\", "
        "s.\"Country\" AS \"Страна\", "
        "s.\"DeliveryDate\" AS \"Дата поставки\", "
        "COALESCE(s2.\"VesselName\", 'Нет прототипа') AS \"Судно-прототип\", "
        "s2.\"Country\" AS \"Страна прототипа\", "
        "COALESCE(med.\"Name\", 'Нет данных') AS \"Проектировщик двигателя\", "
        "COALESCE(mem.\"NameModel\", 'Нет данных') AS \"Модель двигателя\", "
        "COALESCE(noe.\"Number\", 0) AS \"Количество двигателей\" "
        "FROM \"Ships\" s "
        "LEFT JOIN \"ShipType\" st ON s.\"IDShipType\" = st.\"ID\" "
        "LEFT JOIN \"Ships\" s2 ON s.\"SisterShip\" = s2.id "
        "LEFT JOIN \"NumberOfEngines\" noe ON s.id = noe.\"IDShip\" "
        "LEFT JOIN \"MainEngineModel\" mem ON noe.\"IDEngine\" = mem.\"ID\" "
        "LEFT JOIN \"MainEngineDesigner\" med ON mem.\"IDDesigner\" = med.\"ID\" "
        "ORDER BY s.\"VesselName\"";

    if (!query.exec(queryString)) {
        m_lastError = query.lastError().text();
        qDebug() << "Query error:" << m_lastError;
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* DatabaseManager::getShipsWithEngines()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query(m_db);

    QString queryString =
        "SELECT "
        "s.\"VesselName\" AS \"Название судна\", "
        "med.\"Name\" AS \"Проектировщик\", "
        "mem.\"NameModel\" AS \"Модель двигателя\", "
        "noe.\"Number\" AS \"Количество\", "
        "s.\"MaxSpeed(knots)\" AS \"Макс. скорость\" "
        "FROM \"Ships\" s "
        "INNER JOIN \"NumberOfEngines\" noe ON s.id = noe.\"IDShip\" "
        "INNER JOIN \"MainEngineModel\" mem ON noe.\"IDEngine\" = mem.\"ID\" "
        "INNER JOIN \"MainEngineDesigner\" med ON mem.\"IDDesigner\" = med.\"ID\" "
        "ORDER BY s.\"VesselName\"";

    if (!query.exec(queryString)) {
        m_lastError = query.lastError().text();
        qDebug() << "Query error:" << m_lastError;
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}
