#ifndef THEME_H
#define THEME_H

#include <QString>

class Theme
{
private:
    int darkBackgroundRGB[3];
    int lightBackgroundRGB[3];
    int hoverWhitePieceRGB[3];
    int hoverBlackPieceRGB[3];
    int hoverBackgroundRGB[3];
    int attackRGB[3];

    static QString themeStyleString(int [3]);

public:
    enum themes {def, wood};
    void setTheme(themes);
    QString getDarkBackground() {return themeStyleString(darkBackgroundRGB);}
    QString getLightBackground() {return themeStyleString(lightBackgroundRGB);}
    QString getHoverColorWhite() {return themeStyleString(hoverWhitePieceRGB);}
    QString getHoverColorBlack() {return themeStyleString(hoverBlackPieceRGB);}
    QString getHoverBackground() {return themeStyleString(hoverBackgroundRGB);}
    QString getAttackBackground() {return themeStyleString(attackRGB);}
};

#endif // THEME_H
