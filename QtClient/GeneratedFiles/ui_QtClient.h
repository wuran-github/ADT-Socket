/********************************************************************************
** Form generated from reading UI file 'QtClient.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTCLIENT_H
#define UI_QTCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtClientClass
{
public:
    QWidget *centralWidget;
    QLineEdit *FileText;
    QLabel *label_4;
    QLabel *label_3;
    QLabel *ConnectTxt;
    QLineEdit *FileSizeTxt;
    QPushButton *EndBtn;
    QPushButton *BuildBtn;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *ServerIPTxt;
    QLabel *label_5;
    QLineEdit *PortEdit;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtClientClass)
    {
        if (QtClientClass->objectName().isEmpty())
            QtClientClass->setObjectName(QStringLiteral("QtClientClass"));
        QtClientClass->resize(600, 400);
        centralWidget = new QWidget(QtClientClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FileText = new QLineEdit(centralWidget);
        FileText->setObjectName(QStringLiteral("FileText"));
        FileText->setGeometry(QRect(150, 40, 251, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(70, 120, 61, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(270, 80, 54, 12));
        ConnectTxt = new QLabel(centralWidget);
        ConnectTxt->setObjectName(QStringLiteral("ConnectTxt"));
        ConnectTxt->setGeometry(QRect(170, 170, 61, 16));
        FileSizeTxt = new QLineEdit(centralWidget);
        FileSizeTxt->setObjectName(QStringLiteral("FileSizeTxt"));
        FileSizeTxt->setGeometry(QRect(150, 80, 111, 20));
        EndBtn = new QPushButton(centralWidget);
        EndBtn->setObjectName(QStringLiteral("EndBtn"));
        EndBtn->setGeometry(QRect(314, 170, 81, 23));
        BuildBtn = new QPushButton(centralWidget);
        BuildBtn->setObjectName(QStringLiteral("BuildBtn"));
        BuildBtn->setGeometry(QRect(20, 170, 131, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 40, 81, 20));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 80, 121, 20));
        ServerIPTxt = new QLineEdit(centralWidget);
        ServerIPTxt->setObjectName(QStringLiteral("ServerIPTxt"));
        ServerIPTxt->setGeometry(QRect(150, 120, 113, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(280, 120, 71, 20));
        PortEdit = new QLineEdit(centralWidget);
        PortEdit->setObjectName(QStringLiteral("PortEdit"));
        PortEdit->setGeometry(QRect(360, 120, 71, 20));
        QtClientClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtClientClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtClientClass->setStatusBar(statusBar);

        retranslateUi(QtClientClass);

        QMetaObject::connectSlotsByName(QtClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtClientClass)
    {
        QtClientClass->setWindowTitle(QApplication::translate("QtClientClass", "QtClient", nullptr));
        FileText->setText(QApplication::translate("QtClientClass", "H:/Lession/code/newcpp/CPPSocket/", nullptr));
        label_4->setText(QApplication::translate("QtClientClass", "Server IP:", nullptr));
        label_3->setText(QApplication::translate("QtClientClass", "bytes", nullptr));
        ConnectTxt->setText(QApplication::translate("QtClientClass", "not start", nullptr));
        EndBtn->setText(QApplication::translate("QtClientClass", "End Receive", nullptr));
        BuildBtn->setText(QApplication::translate("QtClientClass", "Build TCP connection", nullptr));
        label->setText(QApplication::translate("QtClientClass", "Received File:", nullptr));
        label_2->setText(QApplication::translate("QtClientClass", "Received File Size:", nullptr));
        ServerIPTxt->setText(QApplication::translate("QtClientClass", "127.0.0.1", nullptr));
        label_5->setText(QApplication::translate("QtClientClass", "Server Port:", nullptr));
        PortEdit->setText(QApplication::translate("QtClientClass", "8099", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtClientClass: public Ui_QtClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTCLIENT_H
