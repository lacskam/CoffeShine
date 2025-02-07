#ifndef QCOFFEESHIFTINFO_H
#define QCOFFEESHIFTINFO_H

#include <QObject>
#include <QDataStream>
#include <QDateTime>
#include <QIODevice>
class QCoffeeShiftInfo
{
public:
    QCoffeeShiftInfo();

    int id;
    QDateTime openTime;
    QDateTime closeTime;
    bool open;
    bool close;
    int idUser;
    int idPointSale;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEESHIFTINFO_H
