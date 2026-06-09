#ifndef ADMINFORMUSER_H
#define ADMINFORMUSER_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QComboBox>

namespace Ui {
class AdminFormUser;
}

class AdminFormUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminFormUser(QWidget *parent = nullptr);
    ~AdminFormUser();

private slots:
    void on_pushButton_clicked();   // Кнопка 1: Добавление
    void on_pushButton_2_clicked(); // Кнопка 2: Удаление
    void on_pushButton_4_clicked(); // Кнопка 4: Сохранение изменений из таблицы

private:
    Ui::AdminFormUser *ui;
    QStandardItemModel *model; // Модель для отображения пользователей

    // Вспомогательные методы автоматического обновления интерфейса
    void refreshTable();
    void updateTypeComboBox();
    void updateUserComboBox();
};

#endif // ADMINFORMUSER_H
