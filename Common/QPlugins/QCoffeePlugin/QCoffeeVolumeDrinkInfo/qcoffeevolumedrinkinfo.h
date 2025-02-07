#ifndef QCOFFEEVOLUMEDRINKINFO_H
#define QCOFFEEVOLUMEDRINKINFO_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include <QIODevice>
class QCoffeeVolumeDrinkInfo
{
public:
    QCoffeeVolumeDrinkInfo();
    //void operator =(QCoffeeVolumeDrinkInfo &input);

    int id;
    QString name;
    int volume;
    QString description;
    QString units;

    QVector<int> idDrink;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEEVOLUMEDRINKINFO_H
