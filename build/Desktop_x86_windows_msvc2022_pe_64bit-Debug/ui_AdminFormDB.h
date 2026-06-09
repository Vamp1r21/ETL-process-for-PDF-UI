/********************************************************************************
** Form generated from reading UI file 'AdminFormDB.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINFORMDB_H
#define UI_ADMINFORMDB_H

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

class Ui_AdminFormDB
{
public:
    QGroupBox *groupBox;
    QComboBox *comboBox;
    QCheckBox *checkBox;
    QComboBox *comboBox_2;
    QCheckBox *checkBox_2;
    QComboBox *comboBox_3;
    QCheckBox *checkBox_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTableView *tableView;
    QPushButton *pushButton_3;

    void setupUi(QWidget *AdminFormDB)
    {
        if (AdminFormDB->objectName().isEmpty())
            AdminFormDB->setObjectName("AdminFormDB");
        AdminFormDB->resize(1920, 960);
        groupBox = new QGroupBox(AdminFormDB);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(700, 840, 681, 121));
        QFont font;
        font.setPointSize(14);
        groupBox->setFont(font);
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(270, 31, 211, 21));
        QFont font1;
        font1.setPointSize(11);
        comboBox->setFont(font1);
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(10, 30, 261, 20));
        checkBox->setFont(font);
        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(190, 61, 211, 21));
        comboBox_2->setFont(font1);
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(10, 60, 201, 20));
        checkBox_2->setFont(font);
        comboBox_3 = new QComboBox(groupBox);
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(120, 90, 201, 22));
        comboBox_3->setFont(font1);
        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(10, 90, 171, 20));
        checkBox_3->setFont(font);
        pushButton = new QPushButton(AdminFormDB);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 840, 221, 111));
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(AdminFormDB);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(240, 840, 221, 111));
        pushButton_2->setFont(font);
        tableView = new QTableView(AdminFormDB);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(10, 10, 1901, 821));
        QFont font2;
        font2.setPointSize(10);
        tableView->setFont(font2);
        pushButton_3 = new QPushButton(AdminFormDB);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(470, 840, 221, 111));
        pushButton_3->setFont(font);

        retranslateUi(AdminFormDB);

        QMetaObject::connectSlotsByName(AdminFormDB);
    } // setupUi

    void retranslateUi(QWidget *AdminFormDB)
    {
        AdminFormDB->setWindowTitle(QCoreApplication::translate("AdminFormDB", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("AdminFormDB", "\320\244\320\270\320\273\321\214\321\202\321\200\321\213", nullptr));
        checkBox->setText(QCoreApplication::translate("AdminFormDB", "\320\237\321\200\320\276\320\270\320\267\320\262\320\276\320\264\320\270\321\202\320\265\320\273\321\214 \320\264\320\262\320\270\320\263\320\260\321\202\320\265\320\273\320\265\320\271", nullptr));
        checkBox_2->setText(QCoreApplication::translate("AdminFormDB", "\320\234\320\276\320\264\320\265\320\273\321\214 \320\264\320\262\320\270\320\263\320\260\321\202\320\265\320\273\321\217", nullptr));
        checkBox_3->setText(QCoreApplication::translate("AdminFormDB", "\320\242\320\270\320\277 \321\201\321\203\320\264\320\275\320\260", nullptr));
        pushButton->setText(QCoreApplication::translate("AdminFormDB", "\320\236\321\202\320\276\320\261\321\200\320\260\320\267\320\270\321\202\321\214 \320\221\320\224", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AdminFormDB", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", nullptr));
        pushButton_3->setText(QCoreApplication::translate("AdminFormDB", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminFormDB: public Ui_AdminFormDB {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINFORMDB_H
