#include "EditorForm.h"
#include "ui_EditorForm.h"

// Стандартные инклюды Qt для работы со стандартными модели данных
#include <QStandardItemModel>
#include <QStandardItem>

// Инклюды для работы с базами данных SQL
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>

// Инклюды для интерфейса и логики
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QMap>
#include <QMapIterator>

EditorForm::EditorForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditorForm)
{
    ui->setupUi(this);

    // Настройка tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setAlternatingRowColors(true);

    setupComboBox(ui->comboBox, "MainEngineDesigner", "Name");
    setupComboBox(ui->comboBox_3, "ShipType", "Name");
    updateEngineModels();
    connect(ui->checkBox, &QCheckBox::toggled, this, &EditorForm::updateEngineModels);
    connect(ui->comboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &EditorForm::updateEngineModels);
}

EditorForm::~EditorForm()
{
    delete ui;
}

// Облегченный метод для подсчета судов и вывода всплывающего окна
void EditorForm::updateShipsCount(const QString &whereClause)
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

    // Показываем всплывающее окно с информацией
    QMessageBox::information(this, "Результат фильтрации",
                             QString("Найдено судов в таблице: <b>%1</b>").arg(shipsCount));
}

void EditorForm::updateEngineModels()
{
    ui->comboBox_2->clear();

    // Добавляем дефолтную подсказку
    ui->comboBox_2->addItem("Выберите значение...", 0);

    // Базовый SQL-запрос для моделей
    QString queryStr = "SELECT \"ID\", \"NameModel\" FROM \"MainEngineModel\"";

    // Если чекбокс 1 активен, то фильтруем модели по ID выбранного производителя
    if (ui->checkBox->isChecked()) {
        // Извлекаем ID производителя, который сохранен в UserRole первого комбобокса
        int designerId = ui->comboBox->currentData().toInt();
        queryStr += QString(" WHERE \"IDDesigner\" = %1").arg(designerId);
    }

    QSqlQuery query(queryStr);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBox_2->addItem(name, id);
    }
}

void EditorForm::displayQueryInTable(const QString &queryStr)
{
    // Получаем текущее соединение с БД
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "База данных не подключена!");
        return;
    }

    // Выполняем запрос к БД
    QSqlQuery sqlQuery(db);
    if (!sqlQuery.exec(queryStr)) {
        QMessageBox::critical(this, "Ошибка запроса",
                              "Не удалось выполнить запрос:\n" + sqlQuery.lastError().text());
        return;
    }

    // Используем QStandardItemModel вместо QSqlQueryModel, чтобы ячейки можно было редактировать
    QStandardItemModel *model = new QStandardItemModel(this);

    // Заполняем заголовки колонок, беря их псевдонимы (AS) из SQL-запроса
    int columnCount = sqlQuery.record().count();
    for (int col = 0; col < columnCount; ++col) {
        model->setHorizontalHeaderItem(col, new QStandardItem(sqlQuery.record().fieldName(col)));
    }

    // Заполняем модель данными из БД строк за строкой
    while (sqlQuery.next()) {
        QList<QStandardItem*> rowItems;
        for (int col = 0; col < columnCount; ++col) {
            QVariant value = sqlQuery.value(col);
            QStandardItem *item = new QStandardItem(value.toString());

            // Сохраняем оригинальное значение ячейки в скрытую роль Qt::UserRole.
            item->setData(value, Qt::UserRole);

            // Ограничиваем редактирование для ID и связанных через LEFT JOIN таблиц
            if (col == 0 || col == 2 || col == 11 || col == 12 || col == 13) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }

            rowItems.append(item);
        }
        model->appendRow(rowItems);
    }

    // ИСПРАВЛЕНИЕ УТЕЧКИ ПАМЯТИ:
    QAbstractItemModel *oldModel = ui->tableView->model();

    // Устанавливаем новую редактируемую модель в tableView
    ui->tableView->setModel(model);

    // Если старая модель существовала — удаляем её, освобождая память
    if (oldModel) {
        delete oldModel;
    }

    // Прячем самую первую колонку "ID" (индекс 0) от пользователя
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

void EditorForm::setupComboBox(QComboBox *comboBox, const QString &tableName, const QString &columnName)
{
    comboBox->clear();
    comboBox->addItem("Выберите значение...", 0);

    QSqlQuery query("SELECT \"ID\", \"" + columnName + "\" FROM \"" + tableName + "\"");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        comboBox->addItem(name, id);
    }
}

void EditorForm::on_pushButton_clicked()
{
    QString query =
        "SELECT "
        "s.\"id\" AS \"ID\", "
        "s.\"VesselName\" AS \"Название судна\", "
        "st.\"Name\" AS \"Тип судна\", "
        "s.\"Builder\" AS \"Строитель\", "
        "s.\"Country\" AS \"Страна\", "
        "s.\"DeliveryDate\" AS \"Дата поставки\", "
        "s.\"Length(m)\" AS \"Длина (м)\", "
        "s.\"MaxSpeed(knots)\" AS \"Макс. скорость (узлы)\", "
        "s.\"ImoNumber\" AS \"IMO номер\", "
        "s.\"Gross\" AS \"Валовая вместимость\", "
        "s.\"Displacement(t)\" AS \"Водоихмещение(т)\", "
        "s.\"SisterShip\" AS \"Число судов аналогов\", "
        "med.\"Name\" AS \"Производитель двигателя\", "
        "mem.\"NameModel\" AS \"Модель двигателя\", "
        "noe.\"Number\" AS \"Число двигателей\" "
        "FROM \"Ships\" s "
        "LEFT JOIN \"ShipType\" st ON s.\"IDShipType\" = st.\"ID\" "
        "LEFT JOIN \"NumberOfEngines\" noe ON s.\"id\" = noe.\"IDShip\" "
        "LEFT JOIN \"MainEngineModel\" mem ON noe.\"IDEngine\" = mem.\"ID\" "
        "LEFT JOIN \"MainEngineDesigner\" med ON med.\"ID\" = mem.\"IDDesigner\" ";

    QStringList filters;
    if(ui->checkBox->isChecked()) filters << "med.\"Name\" = '" + ui->comboBox->currentText() + "'";
    if(ui->checkBox_2->isChecked()) filters << "mem.\"NameModel\" = '" + ui->comboBox_2->currentText() + "'";
    if(ui->checkBox_3->isChecked()) filters << "st.\"Name\" = '" + ui->comboBox_3->currentText() + "'";

    // Формируем блок условий WHERE для передачи в функцию подсчета
    QString whereClause = "";
    if(!filters.isEmpty()) {
        whereClause = " WHERE " + filters.join(" AND ");
    }

    // 1. Применяем фильтр и обновляем таблицу
    displayQueryInTable(query + whereClause);

    // 2. Вызываем всплывающее окно с количеством найденных судов
    updateShipsCount(whereClause);
}

void EditorForm::on_pushButton_2_clicked()
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!model) {
        QMessageBox::warning(this, "Предупреждение", "Нет данных для сохранения (сначала выполните поиск).");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "База данных не подключена!");
        return;
    }

    QMap<int, QString> fieldsMapping;
    fieldsMapping[1] = "VesselName";
    fieldsMapping[3] = "Builder";
    fieldsMapping[4] = "Country";
    fieldsMapping[5] = "DeliveryDate";
    fieldsMapping[6] = "Length(m)";
    fieldsMapping[7] = "MaxSpeed(knots)";
    fieldsMapping[8] = "ImoNumber";
    fieldsMapping[9] = "Gross";
    fieldsMapping[10] = "SisterShip";

    db.transaction();
    bool allSavedSuccessfully = true;

    for (int row = 0; row < model->rowCount(); ++row) {
        int shipId = model->item(row, 0)->text().toInt();

        QStringList setClauses;
        QVariantMap valuesToBind;

        QMapIterator<int, QString> i(fieldsMapping);
        while (i.hasNext()) {
            i.next();
            QStandardItem *item = model->item(row, i.key());
            if (!item) continue;

            QVariant currentValue = item->text();
            QVariant originalValue = item->data(Qt::UserRole);

            if (currentValue != originalValue) {
                QString placeholder = QString(":field_%1").arg(i.key());
                setClauses << QString("\"%1\" = %2").arg(i.value(), placeholder);
                valuesToBind[placeholder] = currentValue;
            }
        }

        if (!setClauses.isEmpty()) {
            QSqlQuery updateQuery(db);
            QString updateStr = QString("UPDATE \"Ships\" SET %1 WHERE \"id\" = :id")
                                    .arg(setClauses.join(", "));

            updateQuery.prepare(updateStr);

            QMapIterator<QString, QVariant> b(valuesToBind);
            while (b.hasNext()) {
                b.next();
                updateQuery.bindValue(b.key(), b.value());
            }
            updateQuery.bindValue(":id", shipId);

            if (!updateQuery.exec()) {
                allSavedSuccessfully = false;
                break;
            }
        }
    }

    if (allSavedSuccessfully) {
        db.commit();
        QMessageBox::information(this, "Успех", "Все изменения успешно сохранены в базе данных!");
        on_pushButton_clicked();
    } else {
        db.rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить данные. Операция отменена.");
    }
}
