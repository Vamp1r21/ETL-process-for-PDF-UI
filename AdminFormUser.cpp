#include "AdminFormUser.h"
#include "ui_AdminFormUser.h"

// Необходимые инклюды Qt для работы со стандартными моделями, списками и БД
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QMap>
#include <QMapIterator>

AdminFormUser::AdminFormUser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminFormUser)
    , model(nullptr)
{
    ui->setupUi(this);

    // Первоначальная настройка таблицы tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Заполняем комбобоксы и выводим актуальную таблицу пользователей
    updateTypeComboBox();
    updateUserComboBox();
    refreshTable();
}

AdminFormUser::~AdminFormUser()
{
    delete ui;
}

// Вспомогательный метод: Вывод пользователей в tableView с возможностью редактирования
void AdminFormUser::refreshTable()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "База данных не подключена!");
        return;
    }

    // Запрос объединяет таблицу пользователей и их ролей (LEFT JOIN)
    QString queryStr =
        "SELECT "
        "u.\"ID\" AS \"ID\", "
        "u.\"Name\" AS \"Имя пользователя\", "
        "u.\"Password\" AS \"Пароль\", "
        "t.\"Name\" AS \"Тип пользователя\" "
        "FROM \"Users\" u "
        "LEFT JOIN \"TypeUser\" t ON u.\"IDTypeUser\" = t.\"ID\" "
        "ORDER BY u.\"ID\" ASC";

    QSqlQuery sqlQuery(db);
    if (!sqlQuery.exec(queryStr)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить пользователей:\n" + sqlQuery.lastError().text());
        return;
    }

    QStandardItemModel *newModel = new QStandardItemModel(this);

    // Формируем заголовки колонок
    int columnCount = sqlQuery.record().count();
    for (int col = 0; col < columnCount; ++col) {
        newModel->setHorizontalHeaderItem(col, new QStandardItem(sqlQuery.record().fieldName(col)));
    }

    // Построчно заполняем данными
    while (sqlQuery.next()) {
        QList<QStandardItem*> rowItems;
        for (int col = 0; col < columnCount; ++col) {
            QVariant value = sqlQuery.value(col);
            QStandardItem *item = new QStandardItem(value.toString());

            // Сохраняем начальное значение в UserRole для отслеживания изменений (кнопка 4)
            item->setData(value, Qt::UserRole);

            // Запрещаем редактировать ID (колонка 0) и Тип пользователя из связанной таблицы (колонка 3)
            if (col == 0 || col == 3) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }

            rowItems.append(item);
        }
        newModel->appendRow(rowItems);
    }

    // Безопасная замена модели во избежание утечек памяти
    QAbstractItemModel *oldModel = ui->tableView->model();
    ui->tableView->setModel(newModel);
    if (oldModel) {
        delete oldModel;
    }

    // Скрываем колонку ID от пользователя, оставляя её доступной программно
    ui->tableView->hideColumn(0);
}

// Вспомогательный метод: Заполнение комбобокса 1 (Типы пользователей)
void AdminFormUser::updateTypeComboBox()
{
    ui->comboBox->clear();
    ui->comboBox->addItem("Выберите тип...", 0);

    QSqlQuery query("SELECT \"ID\", \"Name\" FROM \"TypeUser\" ORDER BY \"Name\" ASC");
    while (query.next()) {
        ui->comboBox->addItem(query.value(1).toString(), query.value(0).toInt());
    }
}

// Вспомогательный метод: Заполнение комбобокса 2 (Индексы/ID существующих пользователей)
void AdminFormUser::updateUserComboBox()
{
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("Выберите ID...", 0);

    QSqlQuery query("SELECT \"ID\" FROM \"Users\" ORDER BY \"ID\" ASC");
    while (query.next()) {
        int id = query.value(0).toInt();
        ui->comboBox_2->addItem(QString::number(id), id);
    }
}

// Кнопка 1: Запись нового пользователя
void AdminFormUser::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text().trimmed();
    QString password = ui->lineEdit_2->text().trimmed();
    int typeId = ui->comboBox->currentData().toInt(); // Извлекаем ID типа из скрытой роли

    // Валидация входных данных
    if (name.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Поля 'Имя' и 'Пароль' не могут быть пустыми!");
        return;
    }
    if (typeId == 0) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите корректный тип пользователя!");
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO \"Users\" (\"Name\", \"Password\", \"IDTypeUser\") "
                        "VALUES (:name, :password, :typeId)");
    insertQuery.bindValue(":name", name);
    insertQuery.bindValue(":password", password);
    insertQuery.bindValue(":typeId", typeId);

    if (insertQuery.exec()) {
        QMessageBox::information(this, "Успех", "Новый пользователь успешно добавлен!");

        // Очищаем поля ввода
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->comboBox->setCurrentIndex(0);

        // Обновляем все связанные компоненты формы
        refreshTable();
        updateUserComboBox();
    } else {
        QMessageBox::critical(this, "Ошибка добавления", insertQuery.lastError().text());
    }
}

// Кнопка 2: Удаление пользователя по ID из комбобокса 2
void AdminFormUser::on_pushButton_2_clicked()
{
    int userIdToDelete = ui->comboBox_2->currentData().toInt();

    if (userIdToDelete == 0) {
        QMessageBox::warning(this, "Внимание", "Выберите корректный ID пользователя для удаления!");
        return;
    }

    // Запрос подтверждения удаления у администратора
    auto answer = QMessageBox::question(this, "Подтверждение",
                                        QString("Вы уверены, что хотите безвозвратно удалить пользователя с ID %1?").arg(userIdToDelete),
                                        QMessageBox::Yes | QMessageBox::No);

    if (answer == QMessageBox::No) {
        return;
    }

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM \"Users\" WHERE \"ID\" = :id");
    deleteQuery.bindValue(":id", userIdToDelete);

    if (deleteQuery.exec()) {
        QMessageBox::information(this, "Успех", "Пользователь успешно удален.");

        // Сбрасываем выбор и обновляем данные
        ui->comboBox_2->setCurrentIndex(0);
        refreshTable();
        updateUserComboBox();
    } else {
        QMessageBox::critical(this, "Ошибка удаления", deleteQuery.lastError().text());
    }
}

// Кнопка 4: Сохранение изменений, внесенных вручную в ячейки tableView
void AdminFormUser::on_pushButton_4_clicked()
{
    QStandardItemModel *currentModel = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    if (!currentModel) {
        QMessageBox::warning(this, "Предупреждение", "Нет активной модели данных для сохранения.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Соединение с БД потеряно!");
        return;
    }

    // Карта привязки индексов изменяемых колонок интерфейса к физическим полям таблицы "Users"
    QMap<int, QString> fieldsMapping;
    fieldsMapping[1] = "Name";
    fieldsMapping[2] = "Password";

    db.transaction(); // Запускаем атомарную транзакцию
    bool allSaved = true;

    for (int row = 0; row < currentModel->rowCount(); ++row) {
        int userId = currentModel->item(row, 0)->text().toInt(); // Берем скрытый ID

        QStringList setClauses;
        QVariantMap valuesToBind;

        QMapIterator<int, QString> i(fieldsMapping);
        while (i.hasNext()) {
            i.next();
            QStandardItem *item = currentModel->item(row, i.key());
            if (!item) continue;

            QVariant currentValue = item->text().trimmed();
            QVariant originalValue = item->data(Qt::UserRole);

            // Если данные в ячейке были изменены пользователем
            if (currentValue != originalValue) {
                QString placeholder = QString(":field_%1").arg(i.key());
                setClauses << QString("\"%1\" = %2").arg(i.value(), placeholder);
                valuesToBind[placeholder] = currentValue;
            }
        }

        // Если изменения найдены — выполняем UPDATE для текущей строки
        if (!setClauses.isEmpty()) {
            QSqlQuery updateQuery(db);
            QString updateStr = QString("UPDATE \"Users\" SET %1 WHERE \"ID\" = :id")
                                    .arg(setClauses.join(", "));

            updateQuery.prepare(updateStr);

            QMapIterator<QString, QVariant> b(valuesToBind);
            while (b.hasNext()) {
                b.next();
                updateQuery.bindValue(b.key(), b.value());
            }
            updateQuery.bindValue(":id", userId);

            if (!updateQuery.exec()) {
                qDebug() << "Ошибка при обновлении пользователя ID" << userId << ":" << updateQuery.lastError().text();
                allSaved = false;
                break;
            }
        }
    }

    if (allSaved) {
        db.commit(); // Сохраняем окончательно
        QMessageBox::information(this, "Успех", "Все изменения успешно зафиксированы в базе данных!");
        refreshTable(); // Перезагружаем таблицу для обновления кэша Qt::UserRole
    } else {
        db.rollback(); // Откатываем операцию при любой ошибке
        QMessageBox::critical(this, "Ошибка сохранения", "Не удалось сохранить пакет изменений. База данных восстановлена.");
    }
}
