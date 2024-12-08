#ifndef QCOFFEERECEIPTINFO_H
#define QCOFFEERECEIPTINFO_H

#include <QObject>
#include <QDateTime>
#include <QDataStream>

class QCoffeeReceiptInfo
{
public:
    QCoffeeReceiptInfo();

    int id;
    QDateTime dateTime;
    double sum;
    int idPointSale;
    int idShift;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEERECEIPTINFO_H
