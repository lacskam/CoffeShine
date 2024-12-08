#include "qcoffeetypedrink.h"

QCoffeeTypeDrink::QCoffeeTypeDrink()
{

}

/*
void QCoffeeTypeDrink::operator =(QCoffeeTypeDrink &input)
{
    id = input.id;
    name = input.name;
    description = input.description;
}
*/

void QCoffeeTypeDrink::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>name;
    stream>>description;
}

void QCoffeeTypeDrink::operator >>(QDataStream &stream)
{
    stream << id;
    stream << name;
    stream << description;
}

QByteArray QCoffeeTypeDrink::toByteArray()
{
    QByteArray Output;

    quint8 *ptr;
    Output.push_back((quint8)sizeof(id));
    ptr = (quint8*)(&id);
    for (quint8 i=0;i<sizeof(id);i++)
        Output.push_back(*(ptr+i));

    if (name.length()>45) name = name.mid(0,45);
    Output.push_back((quint8)name.toUtf8().length());
    Output.push_back(name.toUtf8());

    if (description.length()>256) description = description.mid(0,256);
    Output.push_back((quint8)description.toUtf8().length());
    Output.push_back(description.toUtf8());

    return Output;
}

void QCoffeeTypeDrink::fromByteArray(QByteArray data)
{
    if (data.length()>3)
    {
        quint8 *ptr = (quint8*)(&id);
        int size = data.at(0);
        QByteArray buf = data.mid(1,size);
        data.remove(0,size + 1);
        for (int i=0;i<size;i++)
            *(ptr+i) = buf.at(i);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        name = QString(buf);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        description = QString(buf);
    }
}

QString QCoffeeTypeDrink::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) + " name:" + name + " description:" +description;

    return Output;
}
