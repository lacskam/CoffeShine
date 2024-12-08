#include "qcoffeedrinkinfo.h"

QCoffeeDrinkInfo::QCoffeeDrinkInfo()
{

}

/*
void QCoffeeDrinkInfo::operator =(QCoffeeDrinkInfo &input)
{
    id = input.id;
    name = input.name;
    description = input.description;

    idCategories.clear();

    for (int i=0;i<input.idCategories.length();i++)
        idCategories.push_back(input.idCategories.at(i));
}
*/
bool QCoffeeDrinkInfo::operator ==(const QCoffeeDrinkInfo &other) {

    return this->id == other.id;
}

void QCoffeeDrinkInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>name;
    stream>>description;

    idCategories.clear();
    quint16 count;
    stream >> count;
    int currentId;
    for (int i=0;i<count;i++){
        stream>>currentId;
        idCategories.push_back(currentId);
    }

    color << stream;
    stream >> idPicture;
    stream >> pictureHash;
}

void QCoffeeDrinkInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << name;
    stream << description;

    stream << (quint16)idCategories.length();
    int currentId;
    for (int i=0;i<idCategories.length();i++) {
        currentId = idCategories.at(i);
        stream << currentId;
    }

    color >> stream;
    stream << idPicture;
    stream << pictureHash;
}

QByteArray QCoffeeDrinkInfo::toByteArray()
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

    return Output;
}

void QCoffeeDrinkInfo::fromByteArray(QByteArray data)
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

        idCategories.clear();
        quint8 currentCount = data.at(0);
        int currentCategory;
        ptr = (quint8*)(&currentCategory);
        data.remove(0,1);
        for (int i=0;i<currentCount;i++) {
            *(ptr) = data.at(4*i);
            *(ptr+1) = data.at(4*i+1);
            *(ptr+2) = data.at(4*i+2);
            *(ptr+3) = data.at(4*i+3);

            idCategories.push_back(currentCategory);
        }
    }
}

QString QCoffeeDrinkInfo::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) + " name:" + name + " description:" +description;

    Output = Output + "Categoryes: {";
    for (int i=0;i<idCategories.length();i++)
        Output = Output + QString::number(idCategories.at(i)) + " ";

    Output = Output + "}";

    Output = Output + "color: {" +
            QString::number(color.red()) + "," +
            QString::number(color.green()) + "," +
            QString::number(color.blue()) +"} ";

    Output = Output + "Picture hash: " + pictureHash;

    return Output;
}
