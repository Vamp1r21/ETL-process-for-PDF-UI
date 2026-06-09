#ifndef ADMINFORMDB_H
#define ADMINFORMDB_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QComboBox>
#include "AdminFormUser.h"


namespace Ui {
class AdminFormDB;
}

class AdminFormDB : public QWidget
{
    Q_OBJECT

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:
    explicit AdminFormDB(QWidget *parent = nullptr);
    ~AdminFormDB();

private:
    AdminFormUser *adminFormUser;
    Ui::AdminFormDB *ui;

    void updateShipsCount(const QString &whereClause);
    void updateEngineModels();
    void setupComboBox(QComboBox *comboBox, const QString &tableName, const QString &columnName);
    // Вспомогательный метод для отображения модели в tableView
    void displayQueryInTable(const QString &queryStr);
};

#endif // ADMINFORMDB_H
