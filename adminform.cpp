#include "adminform.h"
#include "ui_adminform.h"
#include "LoadClass.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardItemModel>
#include <QPushButton>
#include "xlsxdocument.h"
#include <QFileDialog> // Обязательный инклюд для работы с окном выбора файлов

AdminForm::AdminForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminForm)
    , model(nullptr)
{
    ui->setupUi(this);

    // Создаём модель для tableView
    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    // Настройка внешнего вида tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setAlternatingRowColors(true);
}

AdminForm::~AdminForm()
{
    delete ui;
}

void AdminForm::setButtonGreen(QPushButton* button)
{
    button->setStyleSheet(
        "background-color: green;"
        "color: white;"
        "font-weight: bold;"
        );
}

void AdminForm::resetButtonColor(QPushButton* button)
{
    button->setStyleSheet("");
}

// Вспомогательная функция для парсинга CSV с учётом кавычек
QStringList parseCSVLine(const QString &line)
{
    QStringList fields;
    QString field;
    bool inQuotes = false;
    int i = 0;

    while (i < line.length()) {
        QChar ch = line[i];
        if (ch == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.append(field);
            field.clear();
        } else {
            field += ch;
        }
        i++;
    }
    fields.append(field);
    return fields;
}

void AdminForm::createCSVHeader()
{
    QDir dir;
    dir.mkpath("CSVFiles");
    QFile file("../../CSVFiles/allShip.csv");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "FileName,VesselName,Builder,Designer,OwnerOperator,Country,DeliveryDate,"
               "Length(m),MaxSpeed(knots),ImoNumber,Gross,SisterShip,Displacement(kg),"
               "MainEngineDesigner,MainEngineModel,NumberOfEngines,ShipType\n";
        file.close();
        qDebug() << "CSV header created successfully";
    } else {
        qDebug() << "Failed to create CSV header";
    }
}

void AdminForm::loadCSVToTableView(const QString &filePath)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть CSV файл для предпросмотра!");
        return;
    }

    model->clear();
    QTextStream in(&file);
    int row = 0;
    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = parseCSVLine(line);

        if (isFirstLine) {
            model->setHorizontalHeaderLabels(fields);
            isFirstLine = false;
        } else {
            for (int col = 0; col < fields.size(); ++col) {
                QStandardItem *item = new QStandardItem(fields[col]);
                item->setEditable(true);
                model->setItem(row, col, item);
            }
            row++;
        }
    }
    file.close();
    ui->tableView->resizeColumnsToContents();
}

// Кнопка 1: Выбор одного или нескольких PDF файлов через диалоговое окно -> Обработка в CSV
void AdminForm::on_pushButton_clicked()
{
     setButtonGreen(ui->pushButton);

    // Открываем нативное окно ОС для выбора PDF файлов (можно выбрать один или зажать Ctrl/Shift для нескольких)
    QStringList selectedFiles = QFileDialog::getOpenFileNames(
        this,
        "Выберите PDF файлы для обработки",
        "../../PDFFiles", // Стартовая папка по умолчанию
        "PDF Files (*.pdf)"
        );

    // Если пользователь нажал "Отмена" или ничего не выбрал — прерываем работу слота
    if (selectedFiles.isEmpty()) {
        return;
    }

    ui->pushButton->setEnabled(false);

    try {
        // Создаем пустой CSV файл с заголовками
        createCSVHeader();

        // Циклом проходимся только по тем файлам, которые выбрал пользователь
        for (const QString& filePath : selectedFiles) {
            if (QFile::exists(filePath)) {
                extractClass.ProcessFile(filePath);
            }
        }

        // Финальная очистка и трансформация получившегося CSV
        if (transformClass.TransformCSVFile("../../CSVFiles/allShip.csv")) {
            QMessageBox::information(this, "Успех", "Обработка выбранных PDF и очистка CSV завершена!");
        } else {
            QMessageBox::warning(this, "Ошибка", "Ошибка при трансформации CSV.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", e.what());
    }

    ui->pushButton->setEnabled(true);

    resetButtonColor(ui->pushButton);
}

// Кнопка 2: Загрузить CSV в таблицу для просмотра
void AdminForm::on_pushButton_2_clicked()
{
    setButtonGreen(ui->pushButton_2);

    loadCSVToTableView("../../CSVFiles/allShip.csv");
}

// Кнопка 3: Загрузить данные из CSV в PostgreSQL
void AdminForm::on_pushButton_3_clicked()
{
    setButtonGreen(ui->pushButton_3);

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Нет соединения с базой данных!");
        return;
    }

    LoadClass loadClass(db);

    if (loadClass.loadFromCsv("../../CSVFiles/allShip.csv")) {
        QMessageBox::information(this, "Успех", "Данные загружены!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Ошибка загрузки. Проверьте консоль отладки!");
    }

    resetButtonColor(ui->pushButton_3);
}

void AdminForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Слот пустой
}

void AdminForm::on_pushButton_4_clicked()
{

    adminFormDB = new AdminFormDB(this);
    adminFormDB->setAttribute(Qt::WA_DeleteOnClose);
    connect(adminFormDB, &AdminFormDB::destroyed, this, [this]() {
        adminFormDB = nullptr;
    });
    ui->pushButton->close();
    ui->pushButton_2->close();
    ui->pushButton_3->close();
    ui->pushButton_4->close();
    ui->pushButton_5->close();
    ui->pushButton_6->close();
    ui->tableView->close();
    adminFormDB->show();
}

// Кнопка 5: Сохранить отредактированную таблицу обратно в CSV file
void AdminForm::on_pushButton_5_clicked()
{
    setButtonGreen(ui->pushButton_5);

    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Предупреждение", "Нет данных для сохранения!");

        resetButtonColor(ui->pushButton_5);

        return;
    }

    QString filePath = "../../CSVFiles/allShip.csv";
    QFile file(filePath); // Опечатка исправлена (было File вместо QFile)

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи!");

        resetButtonColor(ui->pushButton_5);

        return;
    }

    QTextStream out(&file);
    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    // 1. Записываем заголовки колонок
    QStringList headers;
    for (int i = 0; i < colCount; ++i) {
        headers << model->horizontalHeaderItem(i)->text();
    }
    out << headers.join(",") << "\n";

    // 2. Записываем строки с данными
    for (int row = 0; row < rowCount; ++row) {
        QStringList rowData;
        for (int col = 0; col < colCount; ++col) {
            QString cellText = model->item(row, col)->text();

            // Экранируем запятые и кавычки по спецификации CSV
            if (cellText.contains(',') || cellText.contains('"')) {
                cellText = cellText.replace("\"", "\"\"");
                cellText = "\"" + cellText + "\"";
            }
            rowData << cellText;
        }
        out << rowData.join(",") << "\n";
    }

    file.close();

    resetButtonColor(ui->pushButton_5);

    QMessageBox::information(this, "Успех", "Изменения успешно сохранены в CSV!");
}

void AdminForm::on_pushButton_6_clicked()
{

    setButtonGreen(ui->pushButton_6);

    QString csvFile = "../../CSVFiles/allShip.csv";

    QFile file(csvFile);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть CSV файл!");
        resetButtonColor(ui->pushButton_6);
        return;
    }

    QXlsx::Document xlsx;

    QTextStream in(&file);

    int row = 1;

    while (!in.atEnd()) {

        QString line = in.readLine();

        QStringList fields = parseCSVLine(line);

        for (int col = 0; col < fields.size(); ++col) {

            xlsx.write(row, col + 1, fields[col]);
        }

        row++;
    }

    file.close();

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить Excel файл",
        "../../CSVFiles/allShip.xlsx",
        "Excel Files (*.xlsx)"
        );

    if (savePath.isEmpty()) {
        resetButtonColor(ui->pushButton_6);
        return;
    }

    if (xlsx.saveAs(savePath)) {

        QMessageBox::information(this,
                                 "Успех",
                                 "Excel файл успешно создан!");

    } else {

        QMessageBox::warning(this,
                             "Ошибка",
                             "Не удалось сохранить Excel файл!");
    }

    resetButtonColor(ui->pushButton_6);
}

