#ifndef USERFORM_H
#define USERFORM_H

#include <QWidget>
#include <QComboBox>
#include <QSqlQueryModel>

namespace Ui {
class UserForm;
}

class UserForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserForm(QWidget *parent = nullptr);
    ~UserForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UserForm *ui;

    void updateShipsCount(const QString &whereClause);
    void displayQueryInTable(const QString &query);
    void setupComboBox(QComboBox *comboBox, const QString &tableName, const QString &columnName);
    // Новый метод для расчета и вывода аналитики
    void updateStatistics(const QString &whereClause);
};

#endif // USERFORM_H
