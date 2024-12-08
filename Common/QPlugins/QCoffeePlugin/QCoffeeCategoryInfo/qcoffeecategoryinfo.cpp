#include "qcoffeecategoryinfo.h"

QCoffeeCategoryInfo::QCoffeeCategoryInfo()
{

}

/*
void QCoffeeCategoryInfo::operator =(QCoffeeCategoryInfo input)
{
    id = input.id;
    name = input.name;
    description = input.description;

    idPointSale.clear();

    for (int i=0;i<input.idPointSale.length();i++)
         idPointSale.push_back(input.idPointSale.at(i));
}
*/

void QCoffeeCategoryInfo::operator <<(QDataStream &stream)
{
    stream >> id;
    stream >> name;
    stream >> description;
    color << stream;

    idPointSale.clear();
    quint16 count;
    stream >> count;
    int currentId;
    for (int i=0;i<count;i++){
        stream>>currentId;
        idPointSale.push_back(currentId);
    }
}

void QCoffeeCategoryInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << name;
    stream << description;
    color >> stream;

    stream << (quint16)idPointSale.length();
    int currentId;
    for (int i=0;i<idPointSale.length();i++) {
        currentId = idPointSale.at(i);
        stream << currentId;
    }
}

QByteArray QCoffeeCategoryInfo::toByteArray()
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

    Output.push_back((quint8)idPointSale.length());
    int currentPointSale;
    ptr = (quint8*)(&currentPointSale);
    for (int i=0;i<idPointSale.length();i++){
        currentPointSale = idPointSale.at(i);
        Output.push_back(*(ptr));
        Output.push_back(*(ptr+1));
        Output.push_back(*(ptr+2));
        Output.push_back(*(ptr+3));
    }

    return Output;
}

void QCoffeeCategoryInfo::fromByteArray(QByteArray data)
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

        idPointSale.clear();
        quint8 currentCount = data.at(0);
        int currentPointSale;
        ptr = (quint8*)(&currentPointSale);
        data.remove(0,1);
        for (int i=0;i<currentCount;i++) {
            *(ptr) = data.at(4*i);
            *(ptr+1) = data.at(4*i+1);
            *(ptr+2) = data.at(4*i+2);
            *(ptr+3) = data.at(4*i+3);

            idPointSale.push_back(currentPointSale);
        }
    }
}

QString QCoffeeCategoryInfo::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) + " name:" + name + " description:" +description;

    Output = Output + "Sale Point: {";
    for (int i=0;i<idPointSale.length();i++)
        Output = Output + QString::number(idPointSale.at(i)) + " ";

    Output = Output + "}";

    return Output;
}
