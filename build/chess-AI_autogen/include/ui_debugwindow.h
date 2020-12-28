/********************************************************************************
** Form generated from reading UI file 'debugwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGWINDOW_H
#define UI_DEBUGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DebugWindow
{
public:
    QWidget *centralwidget;
    QLabel *whiteAttackTable;
    QLabel *blackAttackTable;
    QFrame *line;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DebugWindow)
    {
        if (DebugWindow->objectName().isEmpty())
            DebugWindow->setObjectName(QString::fromUtf8("DebugWindow"));
        DebugWindow->resize(500, 900);
        centralwidget = new QWidget(DebugWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        whiteAttackTable = new QLabel(centralwidget);
        whiteAttackTable->setObjectName(QString::fromUtf8("whiteAttackTable"));
        whiteAttackTable->setGeometry(QRect(50, 430, 400, 400));
        blackAttackTable = new QLabel(centralwidget);
        blackAttackTable->setObjectName(QString::fromUtf8("blackAttackTable"));
        blackAttackTable->setGeometry(QRect(50, 20, 400, 400));
        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(40, 420, 400, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        DebugWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DebugWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 500, 31));
        DebugWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DebugWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        DebugWindow->setStatusBar(statusbar);

        retranslateUi(DebugWindow);

        QMetaObject::connectSlotsByName(DebugWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DebugWindow)
    {
        DebugWindow->setWindowTitle(QApplication::translate("DebugWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DebugWindow: public Ui_DebugWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGWINDOW_H
