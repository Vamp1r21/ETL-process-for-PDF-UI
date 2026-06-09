/********************************************************************************
** Form generated from reading UI file 'adminform.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINFORM_H
#define UI_ADMINFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminForm
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QTableView *tableView;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;

    void setupUi(QWidget *AdminForm)
    {
        if (AdminForm->objectName().isEmpty())
            AdminForm->setObjectName("AdminForm");
        AdminForm->resize(1920, 960);
        pushButton = new QPushButton(AdminForm);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 840, 271, 111));
        QFont font;
        font.setPointSize(14);
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(AdminForm);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(300, 840, 221, 111));
        pushButton_2->setFont(font);
        pushButton_3 = new QPushButton(AdminForm);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(990, 840, 221, 111));
        pushButton_3->setFont(font);
        tableView = new QTableView(AdminForm);
        tableView->setObjectName("tableView");
        tableView->setEnabled(true);
        tableView->setGeometry(QRect(20, 10, 1871, 821));
        QFont font1;
        font1.setPointSize(10);
        tableView->setFont(font1);
        pushButton_4 = new QPushButton(AdminForm);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(1220, 840, 221, 111));
        pushButton_4->setFont(font);
        pushButton_5 = new QPushButton(AdminForm);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(760, 840, 221, 111));
        pushButton_5->setFont(font);
        pushButton_5->setCheckable(false);
        pushButton_5->setAutoDefault(false);
        pushButton_5->setFlat(false);
        pushButton_6 = new QPushButton(AdminForm);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(530, 840, 221, 111));
        pushButton_6->setFont(font);
        pushButton_6->setCheckable(false);
        pushButton_6->setAutoDefault(false);
        pushButton_6->setFlat(false);

        retranslateUi(AdminForm);

        pushButton_5->setDefault(false);
        pushButton_6->setDefault(false);


        QMetaObject::connectSlotsByName(AdminForm);
    } // setupUi

    void retranslateUi(QWidget *AdminForm)
    {
        AdminForm->setWindowTitle(QCoreApplication::translate("AdminForm", "\320\220\320\264\320\274\320\270\320\275\320\270\321\201\321\202\321\200\320\260\321\202\320\276\321\200", nullptr));
        pushButton->setText(QCoreApplication::translate("AdminForm", "\320\230\320\267\320\262\320\273\320\265\321\207\321\214 \320\270 \320\277\321\200\320\265\320\276\320\261\321\200\320\260\320\267\320\276\320\262\320\260\321\202\321\214", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AdminForm", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\320\260", nullptr));
        pushButton_3->setText(QCoreApplication::translate("AdminForm", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\262 \320\221\320\224", nullptr));
        pushButton_4->setText(QCoreApplication::translate("AdminForm", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\221\320\224", nullptr));
        pushButton_5->setText(QCoreApplication::translate("AdminForm", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", nullptr));
        pushButton_6->setText(QCoreApplication::translate("AdminForm", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\262 Excel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminForm: public Ui_AdminForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINFORM_H
