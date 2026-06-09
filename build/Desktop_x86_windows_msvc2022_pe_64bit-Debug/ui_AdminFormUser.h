/********************************************************************************
** Form generated from reading UI file 'AdminFormUser.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINFORMUSER_H
#define UI_ADMINFORMUSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminFormUser
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QComboBox *comboBox;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_2;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QPushButton *pushButton_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *AdminFormUser)
    {
        if (AdminFormUser->objectName().isEmpty())
            AdminFormUser->setObjectName("AdminFormUser");
        AdminFormUser->resize(481, 799);
        QFont font;
        font.setPointSize(14);
        AdminFormUser->setFont(font);
        centralwidget = new QWidget(AdminFormUser);
        centralwidget->setObjectName("centralwidget");
        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(20, 10, 441, 421));
        QFont font1;
        font1.setPointSize(12);
        tableView->setFont(font1);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 440, 221, 291));
        groupBox->setFont(font);
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(20, 230, 161, 61));
        pushButton->setFont(font);
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(10, 70, 201, 31));
        lineEdit->setFont(font);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 50, 49, 16));
        label->setFont(font);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 100, 91, 31));
        label_2->setFont(font);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 170, 49, 16));
        label_3->setFont(font);
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(10, 130, 201, 31));
        lineEdit_2->setFont(font);
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(10, 190, 201, 31));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(240, 440, 221, 181));
        groupBox_2->setFont(font);
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(30, 110, 161, 61));
        pushButton_2->setFont(font);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 35, 81, 31));
        label_4->setFont(font);
        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(10, 70, 201, 31));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(260, 640, 181, 71));
        pushButton_4->setFont(font);
        AdminFormUser->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AdminFormUser);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 481, 32));
        AdminFormUser->setMenuBar(menubar);
        statusbar = new QStatusBar(AdminFormUser);
        statusbar->setObjectName("statusbar");
        AdminFormUser->setStatusBar(statusbar);

        retranslateUi(AdminFormUser);

        QMetaObject::connectSlotsByName(AdminFormUser);
    } // setupUi

    void retranslateUi(QMainWindow *AdminFormUser)
    {
        AdminFormUser->setWindowTitle(QCoreApplication::translate("AdminFormUser", "\320\240\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\265\320\271", nullptr));
        groupBox->setTitle(QCoreApplication::translate("AdminFormUser", "\320\224\320\276\320\261\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        pushButton->setText(QCoreApplication::translate("AdminFormUser", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        label->setText(QCoreApplication::translate("AdminFormUser", "\320\230\320\274\321\217", nullptr));
        label_2->setText(QCoreApplication::translate("AdminFormUser", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        label_3->setText(QCoreApplication::translate("AdminFormUser", "\320\240\320\276\320\273\321\214", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("AdminFormUser", "\320\243\320\264\320\260\320\273\320\265\320\275\320\270\320\265", nullptr));
        pushButton_2->setText(QCoreApplication::translate("AdminFormUser", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        label_4->setText(QCoreApplication::translate("AdminFormUser", "\320\230\320\275\320\264\320\265\320\272\321\201", nullptr));
        pushButton_4->setText(QCoreApplication::translate("AdminFormUser", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminFormUser: public Ui_AdminFormUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINFORMUSER_H
