/********************************************************************************
** Form generated from reading UI file 'avtorisation.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVTORISATION_H
#define UI_AVTORISATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Avtorisation
{
public:
    QWidget *centralwidget;
    QPushButton *AvtorisationBTN;
    QLineEdit *PasswordTB;
    QLabel *PasswordLb;
    QLabel *LoginLb;
    QLineEdit *LoginTB;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Avtorisation)
    {
        if (Avtorisation->objectName().isEmpty())
            Avtorisation->setObjectName("Avtorisation");
        Avtorisation->resize(379, 337);
        centralwidget = new QWidget(Avtorisation);
        centralwidget->setObjectName("centralwidget");
        AvtorisationBTN = new QPushButton(centralwidget);
        AvtorisationBTN->setObjectName("AvtorisationBTN");
        AvtorisationBTN->setGeometry(QRect(70, 240, 231, 51));
        QFont font;
        font.setPointSize(14);
        AvtorisationBTN->setFont(font);
        PasswordTB = new QLineEdit(centralwidget);
        PasswordTB->setObjectName("PasswordTB");
        PasswordTB->setGeometry(QRect(10, 170, 351, 41));
        PasswordTB->setFont(font);
        PasswordTB->setAutoFillBackground(false);
        PasswordTB->setEchoMode(QLineEdit::EchoMode::Password);
        PasswordLb = new QLabel(centralwidget);
        PasswordLb->setObjectName("PasswordLb");
        PasswordLb->setGeometry(QRect(140, 130, 101, 31));
        QFont font1;
        font1.setPointSize(16);
        PasswordLb->setFont(font1);
        LoginLb = new QLabel(centralwidget);
        LoginLb->setObjectName("LoginLb");
        LoginLb->setGeometry(QRect(150, 30, 101, 31));
        LoginLb->setFont(font1);
        LoginTB = new QLineEdit(centralwidget);
        LoginTB->setObjectName("LoginTB");
        LoginTB->setGeometry(QRect(10, 70, 351, 41));
        LoginTB->setFont(font);
        Avtorisation->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Avtorisation);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 379, 22));
        Avtorisation->setMenuBar(menubar);
        statusbar = new QStatusBar(Avtorisation);
        statusbar->setObjectName("statusbar");
        Avtorisation->setStatusBar(statusbar);

        retranslateUi(Avtorisation);

        QMetaObject::connectSlotsByName(Avtorisation);
    } // setupUi

    void retranslateUi(QMainWindow *Avtorisation)
    {
        Avtorisation->setWindowTitle(QCoreApplication::translate("Avtorisation", "\320\220\320\262\321\202\320\276\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        AvtorisationBTN->setText(QCoreApplication::translate("Avtorisation", "\320\222\320\276\320\271\321\202\320\270", nullptr));
        PasswordLb->setText(QCoreApplication::translate("Avtorisation", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        LoginLb->setText(QCoreApplication::translate("Avtorisation", "\320\233\320\276\320\263\320\270\320\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Avtorisation: public Ui_Avtorisation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVTORISATION_H
