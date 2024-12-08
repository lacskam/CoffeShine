#include "qcoffeepictureinfo.h"

QCoffeePictureInfo::QCoffeePictureInfo()
{
    id = -1;
}

void QCoffeePictureInfo::operator <<(QDataStream &stream)
{
    stream >> id;
    stream >> name;
    data.clear();
    stream >> data;
}

void QCoffeePictureInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << name;
    stream << data;
}
