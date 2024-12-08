#ifndef QCOFFEEPICTUREINFO_H
#define QCOFFEEPICTUREINFO_H

#include <QObject>
#include <QDataStream>
#include <QByteArray>

class QCoffeePictureInfo
{
public:
    QCoffeePictureInfo();

    int id;
    QString name;
    QByteArray data;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

};

#endif // QCOFFEEPICTUREINFO_H
