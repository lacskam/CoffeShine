#ifndef QCOLORCONSOLE_H
#define QCOLORCONSOLE_H

#include <QObject>
#include <QDataStream>

class QColorConsole
{
public:
    QColorConsole();

    int red();
    int green();
    int blue();

    void setRed(int r);
    void setGreen(int g);
    void setBlue(int b);

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

private:
    int currentRed;
    int currentGreen;
    int currentBlue;
};

#endif // QCOLORCONSOLE_H
