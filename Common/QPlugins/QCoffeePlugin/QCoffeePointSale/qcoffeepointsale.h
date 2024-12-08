#ifndef QCOFFEEPOINTSALE_H
#define QCOFFEEPOINTSALE_H

#include <QObject>
#include <QVector>

class QCoffeePointSale
{
public:
    QCoffeePointSale();
    //void operator =(QCoffeePointSale input);

    int id;
    QString name;

    QVector<int> idDrinks;
    QVector<int> idCategories;

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEEPOINTSALE_H
