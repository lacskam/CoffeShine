#ifndef QCOFFEETYPEDRINK_H
#define QCOFFEETYPEDRINK_H

#include <QObject>
#include <QDataStream>

class QCoffeeTypeDrink
{
public:
    QCoffeeTypeDrink();
    //void operator =(QCoffeeTypeDrink &input);

    int id;
    QString name;
    QString description;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEETYPEDRINK_H
