#include "userform.h"
#include "ui_userform.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>

UserForm::UserForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserForm)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setAlternatingRowColors(true);

    setupComboBox(ui->comboBox, "MainEngineDesigner", "Name");
    setupComboBox(ui->comboBox_2, "MainEngineModel", "NameModel");
    setupComboBox(ui->comboBox_3, "ShipType", "Name");

    // Если ui->label больше не нужен для статистики, его можно скрыть или удалить в дизайнере.
    //ui->label->hide();
}

UserForm::~UserForm()
{
    delete ui;
}

void UserForm::displayQueryInTable(const QString &query)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "База данных не подключена!");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery sqlQuery(db);

    if (!sqlQuery.exec(query)) {
        QMessageBox::critical(this, "Ошибка запроса",
                              "Не удалось выполнить запрос:\n" + sqlQuery.lastError().text());
        delete model;
        return;
    }

    model->setQuery(std::move(sqlQuery));

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка модели",
                              "Ошибка при установке данных:\n" + model->lastError().text());
        delete model;
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

void UserForm::setupComboBox(QComboBox *comboBox, const QString &tableName, const QString &columnName)
{
    comboBox->clear();
    comboBox->addItem("Выберите значение...", 0);

    QSqlQuery query("SELECT \"ID\", \"" + columnName + "\" FROM \"" + tableName + "\"");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        comboBox->addItem(name, id);
    }

    if (query.lastError().isValid()) {
        qDebug() << "Ошибка при заполнении" << tableName << ":" << query.lastError().text();
    }
}

void UserForm::updateShipsCount(const QString &whereClause)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) return;

    QString fromJoinClause =
        "FROM \"Ships\" s "
        "LEFT JOIN \"ShipType\" st ON s.\"IDShipType\" = st.\"ID\" "
        "LEFT JOIN \"NumberOfEngines\" noe ON s.\"id\" = noe.\"IDShip\" "
        "LEFT JOIN \"MainEngineModel\" mem ON noe.\"IDEngine\" = mem.\"ID\" "
        "LEFT JOIN \"MainEngineDesigner\" med ON med.\"ID\" = mem.\"IDDesigner\" ";

    QSqlQuery q(db);
    QString queryCount = "SELECT COUNT(DISTINCT s.\"id\") " + fromJoinClause + whereClause;

    int shipsCount = 0;
    if (q.exec(queryCount) && q.next()) {
        shipsCount = q.value(0).toInt();
    }

    // Базовый текст сообщения
    QString message = QString("Найденное число судов: <b>%1</b>").arg(shipsCount);

    // ПРОВЕРКА: Проверяем выбор на "Container Ship" или "Контейнерные суда"
    QString currentShipType = ui->comboBox_3->currentText().trimmed();
    if (ui->checkBox_3->isChecked() &&
        (currentShipType == "Container Ship" || currentShipType == "Контейнерные суда")) {

        // 1. Сбор средних показателей (длина и водоизмещение)
        QString queryStats = "SELECT AVG(s.\"Length(m)\"), AVG(s.\"Displacement(t)\") " + fromJoinClause + whereClause;
        double avgLength = 0.0;
        double avgDisplacement = 0.0;

        if (q.exec(queryStats) && q.next()) {
            avgLength = q.value(0).toDouble();
            avgDisplacement = q.value(1).toDouble();
        }

        // 2. Поиск самого популярного производителя (топ-1 для интерфейса)
        QString queryTopDesigner = "SELECT med.\"Name\" " + fromJoinClause + whereClause +
                                   " GROUP BY med.\"Name\" ORDER BY COUNT(s.\"id\") DESC LIMIT 1";
        QString topDesigner = "Нет данных";
        if (q.exec(queryTopDesigner) && q.next()) {
            QString res = q.value(0).toString().trimmed();
            if (!res.isEmpty()) topDesigner = res;
        }

        // 3. Поиск самой популярной модели двигателя (топ-1 для интерфейса)
        QString queryTopModel = "SELECT mem.\"NameModel\" " + fromJoinClause + whereClause +
                                " GROUP BY mem.\"NameModel\" ORDER BY COUNT(s.\"id\") DESC LIMIT 1";
        QString topModel = "Нет данных";
        if (q.exec(queryTopModel) && q.next()) {
            QString res = q.value(0).toString().trimmed();
            if (!res.isEmpty()) topModel = res;
        }

        // --- НОВЫЙ БЛОК: ВЫВОД ПОДРОБНОЙ СТАТИСТИКИ В КОНСОЛЬ (qDebug) ---
        qDebug().noquote() << "\n==================================================";
        qDebug().noquote() << "СТАТИСТИКА ДВИГАТЕЛЕЙ ДЛЯ КОНТЕЙНЕРОВОЗОВ";
        qDebug().noquote() << "Всего судов в выборке:" << shipsCount;
        qDebug().noquote() << "==================================================";

        // Вывод количества по каждому производителю
        QString queryAllDesigners = "SELECT med.\"Name\", COUNT(s.\"id\") " + fromJoinClause + whereClause +
                                    " GROUP BY med.\"Name\" ORDER BY COUNT(s.\"id\") DESC";
        if (q.exec(queryAllDesigners)) {
            qDebug().noquote() << "[Количество по производителям]:";
            while (q.next()) {
                QString designer = q.value(0).toString().trimmed();
                int count = q.value(1).toInt();
                if (designer.isEmpty()) designer = "Не указан (NULL)";
                qDebug().noquote() << QString("  - %1: %2 шт.").arg(designer).arg(count);
            }
        }

        qDebug().noquote() << "--------------------------------------------------";

        // Вывод количества по каждой модели
        QString queryAllModels = "SELECT mem.\"NameModel\", COUNT(s.\"id\") " + fromJoinClause + whereClause +
                                 " GROUP BY mem.\"NameModel\" ORDER BY COUNT(s.\"id\") DESC";
        if (q.exec(queryAllModels)) {
            qDebug().noquote() << "[Количество по моделям двигателей]:";
            while (q.next()) {
                QString model = q.value(0).toString().trimmed();
                int count = q.value(1).toInt();
                if (model.isEmpty()) model = "Не указана (NULL)";
                qDebug().noquote() << QString("  - %1: %2 шт.").arg(model).arg(count);
            }
        }
        qDebug().noquote() << "==================================================\n";
        // ----------------------------------------------------------------

        // Формируем лаконичную статистику для QMessageBox
        message += QString("<br><hr>"
                           "<h3>Статистика по Container Ship:</h3>"
                           "• <b>Средняя длина:</b> %1 м<br>"
                           "• <b>Среднее водоизмещение:</b> %2 т<br>"
                           "• <b>Популярный производитель:</b> %3<br>"
                           "• <b>Популярная модель:</b> %4")
                       .arg(avgLength, 0, 'f', 2)
                       .arg(avgDisplacement, 0, 'f', 2)
                       .arg(topDesigner)
                       .arg(topModel);
    }

    // Вывод диалогового окна
    QMessageBox::information(this, "Результат фильтрации", message);
}

void UserForm::on_pushButton_clicked()
{
    // Селектор колонок для отображения в tableView
    QString selectClause =
        "SELECT "
        "s.\"VesselName\" AS \"Название судна\", "
        "st.\"Name\" AS \"Тип судна\", "
        "s.\"Builder\" AS \"Строитель\", "
        "s.\"Country\" AS \"Страна\", "
        "s.\"DeliveryDate\" AS \"Дата поставки\", "
        "s.\"Length(m)\" AS \"Длина (м)\", "
        "s.\"MaxSpeed(knots)\" AS \"Макс. скорость (узлы)\", "
        "s.\"ImoNumber\" AS \"IMO номер\", "
        "s.\"Gross\" AS \"Валовая вместимость\", "
        "s.\"Displacement(t)\" AS \"Водоизмещение(т)\", "
        "s.\"SisterShip\" AS \"Число судов аналогов\", "
        "med.\"Name\" AS \"Производитель двигателя\", "
        "mem.\"NameModel\" AS \"Модель двигателя\", "
        "noe.\"Number\" AS \"Число двигателей\" ";

    QString fromJoinClause =
        "FROM \"Ships\" s "
        "LEFT JOIN \"ShipType\" st ON s.\"IDShipType\" = st.\"ID\" "
        "LEFT JOIN \"NumberOfEngines\" noe ON s.\"id\" = noe.\"IDShip\" "
        "LEFT JOIN \"MainEngineModel\" mem ON noe.\"IDEngine\" = mem.\"ID\" "
        "LEFT JOIN \"MainEngineDesigner\" med ON med.\"ID\" = mem.\"IDDesigner\" ";

    QString whereClause = "";

    // Конструируем блок условий WHERE в зависимости от галочек
    if(ui->checkBox->isChecked() && ui->checkBox_2->isChecked() && ui->checkBox_3->isChecked())
    {
        whereClause = "WHERE med.\"Name\" = '" + ui->comboBox->currentText() + "' AND "
                                                                               "mem.\"NameModel\" = '" + ui->comboBox_2->currentText() + "' AND "
                                                        "st.\"Name\" = '" + ui->comboBox_3->currentText() + "'";
    }
    else if(ui->checkBox->isChecked() && ui->checkBox_2->isChecked())
    {
        whereClause = "WHERE med.\"Name\" = '" + ui->comboBox->currentText() + "' AND "
                                                                               "mem.\"NameModel\" = '" + ui->comboBox_2->currentText() + "'";
    }
    else if(ui->checkBox_2->isChecked() && ui->checkBox_3->isChecked())
    {
        whereClause = "WHERE mem.\"NameModel\" = '" + ui->comboBox_2->currentText() + "' AND "
                                                                                      "st.\"Name\" = '" + ui->comboBox_3->currentText() + "'";
    }
    else if(ui->checkBox->isChecked() && ui->checkBox_3->isChecked())
    {
        whereClause = "WHERE med.\"Name\" = '" + ui->comboBox->currentText() + "' AND "
                                                                               "st.\"Name\" = '" + ui->comboBox_3->currentText() + "'";
    }
    else if(ui->checkBox->isChecked())
    {
        whereClause = "WHERE med.\"Name\" = '" + ui->comboBox->currentText() + "' ";
    }
    else if(ui->checkBox_2->isChecked())
    {
        whereClause = "WHERE mem.\"NameModel\" = '" + ui->comboBox_2->currentText() + "' ";
    }
    else if(ui->checkBox_3->isChecked())
    {
        whereClause = "WHERE st.\"Name\" = '" + ui->comboBox_3->currentText() + "' ";
    }

    // 1. Обновляем основную таблицу на экране
    displayQueryInTable(selectClause + fromJoinClause + whereClause);

    // 2. Вызываем всплывающее окно с количеством судов
    updateShipsCount(whereClause);
}
