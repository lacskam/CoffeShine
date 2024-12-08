#ifndef QCOFFEECATEGORYINFO_H
#define QCOFFEECATEGORYINFO_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include "../QColorConsole/qcolorconsole.h"

class QCoffeeCategoryInfo
{
public:
    QCoffeeCategoryInfo();
    //void operator =(QCoffeeCategoryInfo input);

    int id;
    QString name;
    QString description;
    QColorConsole color;

    QVector<int> idPointSale;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEECATEGORYINFO_H
