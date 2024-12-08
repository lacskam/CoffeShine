#include "qcoffeeuserinfo.h"

QCoffeeUserInfo::QCoffeeUserInfo()
{

}

/*
void QCoffeeUserInfo::operator =(QCoffeeUserInfo &input)
{
    id = input.id;
    login = input.login;
    password = input.password;
    name = input.name;
    patronymic = input.patronymic;
    surname = input.surname;
}
*/

void QCoffeeUserInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>login;
    stream>>password;
    stream>>name;
    stream>>patronymic;
    stream>>surname;
}

void QCoffeeUserInfo::operator >>(QDataStream &stream)
{
    stream << id;
    stream << login;
    stream << password;
    stream << name;
    stream << patronymic;
    stream << surname;
}

QByteArray QCoffeeUserInfo::toByteArray()
{
    QByteArray Output;

    quint8 *ptr;
    Output.push_back((quint8)sizeof(id));
    ptr = (quint8*)(&id);
    for (quint8 i=0;i<sizeof(id);i++)
        Output.push_back(*(ptr+i));

    if (login.length()>45) login = login.mid(0,45);
    Output.push_back((quint8)login.toUtf8().length());
    Output.push_back(login.toUtf8());

    if (password.length()>45) password = password.mid(0,45);
    Output.push_back((quint8)password.toUtf8().length());
    Output.push_back(password.toUtf8());

    if (name.length()>45) name = name.mid(0,45);
    Output.push_back((quint8)name.toUtf8().length());
    Output.push_back(name.toUtf8());

    if (patronymic.length()>45) patronymic = patronymic.mid(0,45);
    Output.push_back((quint8)patronymic.toUtf8().length());
    Output.push_back(patronymic.toUtf8());

    if (surname.length()>45) surname = surname.mid(0,45);
    Output.push_back((quint8)surname.toUtf8().length());
    Output.push_back(surname.toUtf8());

    return Output;
}

void QCoffeeUserInfo::fromByteArray(QByteArray data)
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
        login = QString(buf);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        password = QString(buf);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        name = QString(buf);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        patronymic = QString(buf);

        size = data.at(0);
        buf = data.mid(1,size);
        data.remove(0,size + 1);
        surname = QString(buf);
    }
}

QString QCoffeeUserInfo::toString()
{
    QString Output;

    Output = "id:" + QString::number(id) + " login:" + login +
             " password:" +password + " name:" + name +
             " patronymic:" + patronymic + " surname:" + surname;

    return Output;
}
