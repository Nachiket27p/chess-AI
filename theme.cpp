#include "theme.h"

void Theme::setTheme(themes selection)
{
    switch (selection)
    {
    case def:
        darkBackgroundRGB[0] = 0;
        darkBackgroundRGB[1] = 0;
        darkBackgroundRGB[2] = 0;
        lightBackgroundRGB[0] = 255;
        lightBackgroundRGB[1] = 255;
        lightBackgroundRGB[2] = 255;
        hoverBackgroundRGB[0] = 0;
        hoverBackgroundRGB[1] = 150;
        hoverBackgroundRGB[2] = 0;
        hoverWhitePieceRGB[0] = 255;
        hoverWhitePieceRGB[1] = 255;
        hoverWhitePieceRGB[2] = 255;
        hoverBlackPieceRGB[0] = 0;
        hoverBlackPieceRGB[1] = 0;
        hoverBlackPieceRGB[2] = 0;
        attackRGB[0] = 150;
        attackRGB[1] = 0;
        attackRGB[2] = 0;
        break;
    case wood:
        darkBackgroundRGB[0] = 150;
        darkBackgroundRGB[1] = 65;
        darkBackgroundRGB[2] = 30;
        lightBackgroundRGB[0] = 240;
        lightBackgroundRGB[1] = 210;
        lightBackgroundRGB[2] = 160;
        hoverBackgroundRGB[0] = 0;
        hoverBackgroundRGB[1] = 150;
        hoverBackgroundRGB[2] = 0;
        hoverWhitePieceRGB[0] = 255;
        hoverWhitePieceRGB[1] = 255;
        hoverWhitePieceRGB[2] = 255;
        hoverBlackPieceRGB[0] = 0;
        hoverBlackPieceRGB[1] = 0;
        hoverBlackPieceRGB[2] = 0;
        attackRGB[0] = 150;
        attackRGB[1] = 0;
        attackRGB[2] = 0;
        break;
    case green:
        darkBackgroundRGB[0] = 0;
        darkBackgroundRGB[1] = 100;
        darkBackgroundRGB[2] = 0;
        lightBackgroundRGB[0] = 250;
        lightBackgroundRGB[1] = 250;
        lightBackgroundRGB[2] = 250;
        hoverBackgroundRGB[0] = 150;
        hoverBackgroundRGB[1] = 150;
        hoverBackgroundRGB[2] = 150;
        hoverWhitePieceRGB[0] = 250;
        hoverWhitePieceRGB[1] = 250;
        hoverWhitePieceRGB[2] = 250;
        hoverBlackPieceRGB[0] = 0;
        hoverBlackPieceRGB[1] = 0;
        hoverBlackPieceRGB[2] = 0;
        attackRGB[0] = 150;
        attackRGB[1] = 0;
        attackRGB[2] = 0;
        break;
    default:
        break;
    }
}

QString Theme::themeStyleString(int rgb[3])
{
    return QString("rgb(") + QString::number(rgb[0]) + QString(", ") + QString::number(rgb[1]) + QString(", ") + QString::number(rgb[2]) + QString(");");
}
