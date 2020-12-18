/********************************************************************************
** Form generated from reading UI file 'board.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOARD_H
#define UI_BOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Board
{
public:
    QAction *actionNew_Game;
    QAction *actionExit;
    QAction *actionDefault;
    QAction *actionWood;
    QAction *actionAbout;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuTheme;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Board)
    {
        if (Board->objectName().isEmpty())
            Board->setObjectName(QString::fromUtf8("Board"));
        Board->resize(1000, 1000);
        actionNew_Game = new QAction(Board);
        actionNew_Game->setObjectName(QString::fromUtf8("actionNew_Game"));
        actionExit = new QAction(Board);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionDefault = new QAction(Board);
        actionDefault->setObjectName(QString::fromUtf8("actionDefault"));
        actionWood = new QAction(Board);
        actionWood->setObjectName(QString::fromUtf8("actionWood"));
        actionAbout = new QAction(Board);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralwidget = new QWidget(Board);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Board->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Board);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1000, 34));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTheme = new QMenu(menubar);
        menuTheme->setObjectName(QString::fromUtf8("menuTheme"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        Board->setMenuBar(menubar);
        statusbar = new QStatusBar(Board);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Board->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuTheme->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_Game);
        menuFile->addAction(actionExit);
        menuTheme->addAction(actionDefault);
        menuTheme->addAction(actionWood);
        menuHelp->addAction(actionAbout);

        retranslateUi(Board);

        QMetaObject::connectSlotsByName(Board);
    } // setupUi

    void retranslateUi(QMainWindow *Board)
    {
        Board->setWindowTitle(QApplication::translate("Board", "MainWindow", nullptr));
        actionNew_Game->setText(QApplication::translate("Board", "New Game", nullptr));
        actionExit->setText(QApplication::translate("Board", "Exit", nullptr));
        actionDefault->setText(QApplication::translate("Board", "Default", nullptr));
        actionWood->setText(QApplication::translate("Board", "Wood", nullptr));
        actionAbout->setText(QApplication::translate("Board", "About", nullptr));
        menuFile->setTitle(QApplication::translate("Board", "File", nullptr));
        menuTheme->setTitle(QApplication::translate("Board", "Theme", nullptr));
        menuHelp->setTitle(QApplication::translate("Board", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Board: public Ui_Board {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOARD_H
