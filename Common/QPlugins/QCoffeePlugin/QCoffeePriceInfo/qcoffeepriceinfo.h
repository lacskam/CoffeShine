#ifndef QCOFFEEPRICEINFO_H
#define QCOFFEEPRICEINFO_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include <QDateTime>


class QCoffeePriceInfo
{
public:
    QCoffeePriceInfo();
    //void operator =(QCoffeePriceInfo &input);

    int id;
    double value;
    QDateTime startDate;
    QString description;
    int idDrink;
    int idVolume;
    int idDrinkCategory;

    QVector<int> idPointSale;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEEPRICEINFO_H
