#include <QWidgetItem>
#include "debugwindow.h"
#include "ui_debugwindow.h"
#include "rules.h"

DebugWindow::DebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DebugWindow)
{
    ui->setupUi(this);
}

DebugWindow::~DebugWindow()
{
    delete ui;
}

void DebugWindow::updateWhiteValues()
{
    Rules *rules = Rules::getInstance();
    QString s = "";
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int x = rules->whiteAttacks[i][j];
            s += QString::number(x);
            s += "         ";
        }
        s += "\n\n";
    }
    ui->whiteAttackTable->setText(s);
}

void DebugWindow::updateBlackValues()
{
    Rules *rules = Rules::getInstance();
    QString s = "";
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int x = rules->blackAttacks[i][j];
            s += QString::number(x);
            s += "         ";
        }
        s += "\n\n";
    }
    ui->blackAttackTable->setText(s);
}

