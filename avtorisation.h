#ifndef AVTORISATION_H
#define AVTORISATION_H

#include <QMainWindow>
#include "adminform.h"  // Добавьте эту строку
#include "userform.h"
#include "EditorForm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Avtorisation;
}
QT_END_NAMESPACE

class Avtorisation : public QMainWindow
{
    Q_OBJECT

public:
    Avtorisation(QWidget *parent = nullptr);
    ~Avtorisation();

private slots:
    void on_AvtorisationBTN_clicked();

private:
    Ui::Avtorisation *ui;
    AdminForm *adminForm;
    UserForm *userForm;
    EditorForm *editorForm;
};

#endif // AVTORISATION_H
