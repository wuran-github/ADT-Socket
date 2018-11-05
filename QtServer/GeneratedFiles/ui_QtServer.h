/********************************************************************************
** Form generated from reading UI file 'QtServer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSERVER_H
#define UI_QTSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtServerClass
{
public:
    QWidget *centralWidget;
    QLineEdit *FileText;
    QLabel *label;
    QPushButton *FileBtn;
    QLabel *label_2;
    QLineEdit *FileSizeTxt;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *IPTxt;
    QPushButton *ListenBtn;
    QPushButton *SendBtn;
    QLabel *ConnectTxt;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtServerClass)
    {
        if (QtServerClass->objectName().isEmpty())
            QtServerClass->setObjectName(QStringLiteral("QtServerClass"));
        QtServerClass->resize(600, 400);
        centralWidget = new QWidget(QtServerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FileText = new QLineEdit(centralWidget);
        FileText->setObjectName(QStringLiteral("FileText"));
        FileText->setGeometry(QRect(110, 40, 251, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 40, 71, 20));
        FileBtn = new QPushButton(centralWidget);
        FileBtn->setObjectName(QStringLiteral("FileBtn"));
        FileBtn->setGeometry(QRect(370, 40, 31, 23));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 80, 71, 16));
        FileSizeTxt = new QLineEdit(centralWidget);
        FileSizeTxt->setObjectName(QStringLiteral("FileSizeTxt"));
        FileSizeTxt->setGeometry(QRect(110, 80, 111, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(230, 80, 54, 12));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 120, 61, 20));
        IPTxt = new QLineEdit(centralWidget);
        IPTxt->setObjectName(QStringLiteral("IPTxt"));
        IPTxt->setGeometry(QRect(110, 120, 113, 20));
        ListenBtn = new QPushButton(centralWidget);
        ListenBtn->setObjectName(QStringLiteral("ListenBtn"));
        ListenBtn->setGeometry(QRect(30, 170, 75, 23));
        SendBtn = new QPushButton(centralWidget);
        SendBtn->setObjectName(QStringLiteral("SendBtn"));
        SendBtn->setGeometry(QRect(330, 170, 75, 23));
        ConnectTxt = new QLabel(centralWidget);
        ConnectTxt->setObjectName(QStringLiteral("ConnectTxt"));
        ConnectTxt->setGeometry(QRect(120, 175, 61, 16));
        QtServerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtServerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtServerClass->setStatusBar(statusBar);

        retranslateUi(QtServerClass);

        QMetaObject::connectSlotsByName(QtServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtServerClass)
    {
        QtServerClass->setWindowTitle(QApplication::translate("QtServerClass", "QtServer", nullptr));
        label->setText(QApplication::translate("QtServerClass", "Send File:", nullptr));
        FileBtn->setText(QApplication::translate("QtServerClass", "...", nullptr));
        label_2->setText(QApplication::translate("QtServerClass", "File Size:", nullptr));
        label_3->setText(QApplication::translate("QtServerClass", "bytes", nullptr));
        label_4->setText(QApplication::translate("QtServerClass", "local IP:", nullptr));
        ListenBtn->setText(QApplication::translate("QtServerClass", "StratListen", nullptr));
        SendBtn->setText(QApplication::translate("QtServerClass", "Send", nullptr));
        ConnectTxt->setText(QApplication::translate("QtServerClass", "not start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtServerClass: public Ui_QtServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSERVER_H
