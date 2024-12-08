#ifndef QCOFFEESOLDPOSITIONINFO_H
#define QCOFFEESOLDPOSITIONINFO_H

#include <QObject>
#include <QDataStream>
#include <QDateTime>
#include <QVector>

class QCoffeeSoldPositionInfo
{
public:
    QCoffeeSoldPositionInfo();
    //void operator =(QCoffeeSoldPositionInfo &input);


    int id;
    int idUser;
    int count;
    int idDrink;
    int idTypeDrink;
    int idPrice;
    int idVolumeDrink;
    int idReceipt;
    int idPointSale;
    int numberInReceipt;
    QDateTime date;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEESOLDPOSITIONINFO_H
