#include "qcolorconsole.h"

QColorConsole::QColorConsole()
{

}

void QColorConsole::operator <<(QDataStream &stream)
{
    stream >> currentRed;
    stream >> currentGreen;
    stream >> currentBlue;
}

void QColorConsole::operator >>(QDataStream &stream)
{
    stream << currentRed;
    stream << currentGreen;
    stream << currentBlue;
}


int QColorConsole::red()
{
    return currentRed;
}

int QColorConsole::green()
{
    return currentGreen;
}

int QColorConsole::blue()
{
    return currentBlue;
}

void QColorConsole::setRed(int r)
{
    currentRed = r;
}

void QColorConsole::setGreen(int g)
{
    currentGreen = g;
}

void QColorConsole::setBlue(int b)
{
    currentBlue = b;
}
