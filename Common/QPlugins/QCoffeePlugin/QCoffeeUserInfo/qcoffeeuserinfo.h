#ifndef QCOFFEEUSERINFO_H
#define QCOFFEEUSERINFO_H

#include <QObject>
#include <QDataStream>

class QCoffeeUserInfo
{
public:
    QCoffeeUserInfo();
    //void operator =(QCoffeeUserInfo &input);

    int id;
    QString login;
    QString password;
    QString name;
    QString patronymic;
    QString surname;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);

    QString toString();
};

#endif // QCOFFEEUSERINFO_H
