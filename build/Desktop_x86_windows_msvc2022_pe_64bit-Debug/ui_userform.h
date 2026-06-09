/********************************************************************************
** Form generated from reading UI file 'userform.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERFORM_H
#define UI_USERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserForm
{
public:
    QTableView *tableView;
    QPushButton *pushButton;
    QGroupBox *groupBox;
    QComboBox *comboBox;
    QCheckBox *checkBox;
    QComboBox *comboBox_2;
    QCheckBox *checkBox_2;
    QComboBox *comboBox_3;
    QCheckBox *checkBox_3;

    void setupUi(QWidget *UserForm)
    {
        if (UserForm->objectName().isEmpty())
            UserForm->setObjectName("UserForm");
        UserForm->resize(1920, 960);
        tableView = new QTableView(UserForm);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(10, 10, 1901, 821));
        QFont font;
        font.setPointSize(10);
        tableView->setFont(font);
        pushButton = new QPushButton(UserForm);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 840, 221, 111));
        QFont font1;
        font1.setPointSize(14);
        pushButton->setFont(font1);
        groupBox = new QGroupBox(UserForm);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(230, 840, 681, 121));
        groupBox->setFont(font1);
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(270, 30, 241, 22));
        QFont font2;
        font2.setPointSize(11);
        comboBox->setFont(font2);
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(10, 30, 281, 20));
        checkBox->setFont(font1);
        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(190, 60, 231, 22));
        comboBox_2->setFont(font2);
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(10, 60, 181, 20));
        checkBox_2->setFont(font1);
        comboBox_3 = new QComboBox(groupBox);
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(120, 90, 231, 22));
        comboBox_3->setFont(font2);
        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(10, 90, 171, 20));
        checkBox_3->setFont(font1);

        retranslateUi(UserForm);

        QMetaObject::connectSlotsByName(UserForm);
    } // setupUi

    void retranslateUi(QWidget *UserForm)
    {
        UserForm->setWindowTitle(QCoreApplication::translate("UserForm", "\320\220\320\275\320\260\320\273\320\270\321\202\320\270\320\272", nullptr));
        pushButton->setText(QCoreApplication::translate("UserForm", "\320\236\321\202\320\276\320\261\321\200\320\260\320\267\320\270\321\202\321\214 \320\221\320\224", nullptr));
        groupBox->setTitle(QCoreApplication::translate("UserForm", "\320\244\320\270\320\273\321\214\321\202\321\200\321\213", nullptr));
        checkBox->setText(QCoreApplication::translate("UserForm", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\270\321\202\320\265\320\273\321\214 \320\264\320\262\320\270\320\263\320\260\321\202\320\265\320\273\320\265\320\271", nullptr));
        checkBox_2->setText(QCoreApplication::translate("UserForm", "\320\234\320\276\320\264\320\265\320\273\321\214 \320\264\320\262\320\270\320\263\320\260\321\202\320\265\320\273\321\217", nullptr));
        checkBox_3->setText(QCoreApplication::translate("UserForm", "\320\242\320\270\320\277 \321\201\321\203\320\264\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserForm: public Ui_UserForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERFORM_H
