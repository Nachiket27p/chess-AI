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
    QAction *actionGreen;
    QAction *actionAI_vs_AI;
    QAction *action2_Player;
    QAction *actionPlay_AI;
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
        Board->resize(910, 955);
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
        actionGreen = new QAction(Board);
        actionGreen->setObjectName(QString::fromUtf8("actionGreen"));
        actionAI_vs_AI = new QAction(Board);
        actionAI_vs_AI->setObjectName(QString::fromUtf8("actionAI_vs_AI"));
        action2_Player = new QAction(Board);
        action2_Player->setObjectName(QString::fromUtf8("action2_Player"));
        actionPlay_AI = new QAction(Board);
        actionPlay_AI->setObjectName(QString::fromUtf8("actionPlay_AI"));
        centralwidget = new QWidget(Board);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Board->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Board);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 910, 31));
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
        menuFile->addAction(actionPlay_AI);
        menuFile->addAction(action2_Player);
        menuFile->addAction(actionAI_vs_AI);
        menuTheme->addAction(actionDefault);
        menuTheme->addAction(actionWood);
        menuTheme->addAction(actionGreen);
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
        actionGreen->setText(QApplication::translate("Board", "Green", nullptr));
        actionAI_vs_AI->setText(QApplication::translate("Board", "AI vs. AI", nullptr));
        action2_Player->setText(QApplication::translate("Board", "2-Player", nullptr));
        actionPlay_AI->setText(QApplication::translate("Board", "Vs. AI", nullptr));
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
