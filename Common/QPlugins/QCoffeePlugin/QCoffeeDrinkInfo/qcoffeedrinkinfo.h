#ifndef QCOFFEEDRINKINFO_H
#define QCOFFEEDRINKINFO_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include "../QColorConsole/qcolorconsole.h"

class QCoffeeDrinkInfo
{
public:
    explicit QCoffeeDrinkInfo();
    //void operator =(QCoffeeDrinkInfo &input);

    quint32 id;
    QString name;
    QString description;

    QVector<int> idCategories;

    QColorConsole color;
    int idPicture;
    QString pictureHash;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);
    bool operator ==(const QCoffeeDrinkInfo &other);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEEDRINKINFO_H
