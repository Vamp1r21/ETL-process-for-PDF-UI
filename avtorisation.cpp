#include "avtorisation.h"
#include "ui_avtorisation.h"
#include "adminform.h"  // Добавьте эту строку, если ещё нет
#include <QMessageBox>
#include <warning.h>
#include "userform.h"
#include "EditorForm.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

Avtorisation::Avtorisation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Avtorisation)
    , adminForm(nullptr)  // Инициализируем указатель
{
    ui->setupUi(this);
}

Avtorisation::~Avtorisation()
{
    delete ui;
}

void Avtorisation::on_AvtorisationBTN_clicked()
{
    QString login = ui->LoginTB->text();
    QString password = ui->PasswordTB->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Введите логин и пароль");
        return;
    }

    // Подготавливаем запрос. Соединяем Users и TypeUser по внешнему ключу.
    QSqlQuery query;
    query.prepare("SELECT tu.\"Name\" FROM \"Users\" u "
                  "JOIN \"TypeUser\" tu ON u.\"IDTypeUser\" = tu.\"ID\" "
                  "WHERE u.\"Name\" = :login AND u.\"Password\" = :password");

    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qCritical() << "Ошибка выполнения запроса:" << query.lastError().text();
        QMessageBox::critical(this, "Ошибка БД", "Не удалось выполнить проверку пользователя");
        return;
    }

    if (query.next()) {
        QString role = query.value(0).toString();
        qDebug() << "Авторизация успешна. Роль:" << role;

        if (role == "Администратор") {
            if (!adminForm) {
                adminForm = new AdminForm(nullptr);
                adminForm->setAttribute(Qt::WA_DeleteOnClose);
                connect(adminForm, &AdminForm::destroyed, this, [this]() { adminForm = nullptr; });
            }
            adminForm->show();
            this->close();

        } else if (role == "Редактор") {
            editorForm = new EditorForm(nullptr);
            editorForm->setAttribute(Qt::WA_DeleteOnClose);
            connect(editorForm, &EditorForm::destroyed, this, [this]() { editorForm = nullptr; });
            editorForm->show();
            this->close();
        } else if (role == "Аналитик")
        {
            userForm = new UserForm(nullptr);
            userForm->setAttribute(Qt::WA_DeleteOnClose);
            connect(userForm, &UserForm::destroyed, this, [this]() { userForm = nullptr; });
            userForm->show();
            this->close();
        }

    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}
