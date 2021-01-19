#include <QWidgetItem>
#include "debugwindow.h"
#include "ui_debugwindow.h"
#include "rules.h"

DebugWindow::DebugWindow(QWidget *parent) : QMainWindow(parent),
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
    for (int i = 0; i < 64; i++)
    {
        int x = rules->whiteAttacks[i];
        s += QString::number(x);
        s += "         ";
        if (!((i + 1) % 8))
            s += "\n\n";
    }
    ui->whiteAttackTable->setText(s);
}

void DebugWindow::updateBlackValues()
{
    Rules *rules = Rules::getInstance();
    QString s = "";
    for (int i = 0; i < 64; i++)
    {
        int x = rules->blackAttacks[i];
        s += QString::number(x);
        s += "         ";
        if (!((i + 1) % 8))
            s += "\n\n";
    }
    ui->blackAttackTable->setText(s);
}
