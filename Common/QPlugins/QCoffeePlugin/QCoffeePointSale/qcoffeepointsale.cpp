#include "qcoffeepointsale.h"

QCoffeePointSale::QCoffeePointSale()
{
    id = -1;
}

/*

void QCoffeePointSale::operator =(QCoffeePointSale input)
{
    id = input.id;
    name = input.name;


    idDrinks.clear();
    for (int i=0;i<input.idDrinks.length();i++)
        idDrinks.push_back(input.idDrinks.at(i));

    idCategories.clear();
    for (int i=0;i<input.idCategories.length();i++)
        idCategories.push_back(input.idCategories.at(i));
}
*/





QByteArray QCoffeePointSale::toByteArray()
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

    Output.push_back((quint8)idCategories.length());
    int currentCategory;
    ptr = (quint8*)(&currentCategory);
    for (int i=0;i<idCategories.length();i++){
        currentCategory = idCategories.at(i);
        Output.push_back(*(ptr));
        Output.push_back(*(ptr+1));
        Output.push_back(*(ptr+2));
        Output.push_back(*(ptr+3));
    }

    Output.push_back((quint8)idDrinks.length());
    int currentDrink;
    ptr = (quint8*)(&currentDrink);
    for (int i=0;i<idDrinks.length();i++){
        currentDrink = idDrinks.at(i);
        Output.push_back(*(ptr));
        Output.push_back(*(ptr+1));
        Output.push_back(*(ptr+2));
        Output.push_back(*(ptr+3));
    }

    return Output;
}

void QCoffeePointSale::fromByteArray(QByteArray data)
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

        idCategories.clear();
        quint8 currentCount = data.at(0);
        quint32 currentCategory;
        ptr = (quint8*)(&currentCategory);
        data.remove(0,1);
        for (int i=0;i<currentCount;i++) {
            *(ptr) = data.at(4*i);
            *(ptr+1) = data.at(4*i+1);
            *(ptr+2) = data.at(4*i+2);
            *(ptr+3) = data.at(4*i+3);

            idCategories.push_back(currentCategory);
        }
        data.remove(0,sizeof(currentCategory)*currentCount);

        idDrinks.clear();
        currentCount = data.at(0);
        quint32 currentDrink;
        ptr = (quint8*)(&currentDrink);
        data.remove(0,1);
        for (int i=0;i<currentCount;i++) {
            *(ptr) = data.at(4*i);
            *(ptr+1) = data.at(4*i+1);
            *(ptr+2) = data.at(4*i+2);
            *(ptr+3) = data.at(4*i+3);

            idDrinks.push_back(currentDrink);
        }
    }
}

QString QCoffeePointSale::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) + " name:" + name;

    Output = Output + "Categoryes: {";
    for (int i=0;i<idCategories.length();i++)
        Output = Output + QString::number(idCategories.at(i)) + " ";

    Output = Output + "}";

    Output = Output + "Drinks: {";
    for (int i=0;i<idDrinks.length();i++)
        Output = Output + QString::number(idDrinks.at(i)) + " ";

    Output = Output + "}";

    return Output;
}
