#ifndef EDITORFORM_H
#define EDITORFORM_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QComboBox>

namespace Ui {
class EditorForm;
}

class EditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditorForm(QWidget *parent = nullptr);
    ~EditorForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EditorForm *ui;

    void updateShipsCount(const QString &whereClause);
    void updateEngineModels();
    void setupComboBox(QComboBox *comboBox, const QString &tableName, const QString &columnName);
    // Вспомогательный метод для отображения модели в tableView
    void displayQueryInTable(const QString &query);
};

#endif // EDITORFORM_H
