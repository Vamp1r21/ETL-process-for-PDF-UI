#ifndef ADMINFORM_H
#define ADMINFORM_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPushButton>

// Подключаем заголовочные файлы ваших классов для обработки данных
#include "AdminformDB.h"
#include "LoadClass.h"

// Вставьте сюда корректные имена заголовочных файлов для extractClass и transformClass,
// если они называются иначе (например, "ExtractClass.h" и "TransformClass.h")
#include "ExtractClass.h"
#include "TransformClass.h"

namespace Ui {
class AdminForm;
}

class AdminForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminForm(QWidget *parent = nullptr);
    ~AdminForm();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::AdminForm *ui;
    QStandardItemModel *model;
    AdminFormDB *adminFormDB = nullptr;

    void setButtonGreen(QPushButton* button);
    void resetButtonColor(QPushButton* button);

    // Экземпляры классов для извлечения и трансформации данных
    ExtractClass extractClass;
    TransformClass transformClass;

    // Внутренние вспомогательные методы
    void createCSVHeader();
    void loadCSVToTableView(const QString &filePath);
};

#endif // ADMINFORM_H
