// main.cpp
#include "avtorisation.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

bool connectToDatabase() {
    // 1. Добавляем драйвер PostgreSQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    // 2. Настраиваем параметры подключения
    db.setHostName("localhost");      // Адрес сервера (если на этом же ПК)
    db.setDatabaseName("TestDB"); // Имя твоей базы данных
    db.setUserName("postgres");       // Твой логин
    db.setPassword("78Aherot9!!");  // Твой пароль
    db.setPort(5432);                 // Стандартный порт PostgreSQL

    // 3. Пытаемся открыть соединение
    if (!db.open()) {
        qCritical() << "Ошибка подключения к БД:" << db.lastError().text();
        return false;
    }

    qInfo() << "Успешное подключение к базе данных!";
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    connectToDatabase();

    // Удаляем старый файл, если он существует
    QString filePath = "../../CSVFiles/allShip.csv";
    QFile file(filePath);

    if (file.exists()) {
        if (file.remove()) {
            qDebug() << "Старый файл удалён";
        }
    }

    // Создаём новый пустой файл с заголовками
    QDir dir;
    dir.mkpath("../../CSVFiles");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "FileName,VesselName,Builder,Designer,OwnerOperator,Country,DeliveryDate,"
               "Length(m),MaxSpeed(knots),ImoNumber,Gross,SisterShip,Displacement(t),"
               "MainEngineDesigner,MainEngineModel,NumberOfEngines,ShipType\n";
        file.close();
        qDebug() << "Создан новый файл с заголовками";
    }

    Avtorisation w;
    w.show();
    return a.exec();
}
