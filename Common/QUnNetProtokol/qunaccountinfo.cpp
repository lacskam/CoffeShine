#include "qunaccountinfo.h"

QUNAccountInfo::QUNAccountInfo()
{

}


void QUNAccountInfo::operator <<(QDataStream &stream)
{
    stream>>id;
    stream>>login;
    stream>>password;
    stream>>name;
    stream>>patronymic;
    stream>>surname;
    stream>>email;
    stream>>number;
    stream>>enable;
    stream>>visible;
    stream>>rights;
    stream>>regdate;
    stream>>activate;
    stream>>changepassword;
    stream>>datepassword;
    stream>>activemail;
    stream>>activenumber;
    stream>>emailcode;
    stream>>numbercode;
}

void QUNAccountInfo::operator >>(QDataStream &stream)
{
    stream<<id;
    stream<<login;
    stream<<password;
    stream<<name;
    stream<<patronymic;
    stream<<surname;
    stream<<email;
    stream<<number;
    stream<<enable;
    stream<<visible;
    stream<<rights;
    stream<<regdate;
    stream<<activate;
    stream<<changepassword;
    stream<<datepassword;
    stream<<activemail;
    stream<<activenumber;
    stream<<emailcode;
    stream<<numbercode;
}


QByteArray QUNAccountInfo::toByteArray()
{
    QByteArray Output;

    quint8 *ptr;
    Output.push_back((quint8)sizeof(id));
    ptr = (quint8*)(&id);
    for (quint8 i=0;i<sizeof(id);i++)
        Output.push_back(*(ptr+i));

    Output.push_back((quint8)login.toUtf8().length());
    Output.push_back(login.toUtf8());

    Output.push_back((quint8)password.toUtf8().length());
    Output.push_back(password.toUtf8());

    Output.push_back((quint8)name.toUtf8().length());
    Output.push_back(name.toUtf8());

    Output.push_back((quint8)patronymic.toUtf8().length());
    Output.push_back(patronymic.toUtf8());

    Output.push_back((quint8)surname.toUtf8().length());
    Output.push_back(surname.toUtf8());

    Output.push_back((quint8)email.toUtf8().length());
    Output.push_back(email.toUtf8());

    Output.push_back((quint8)number.toUtf8().length());
    Output.push_back(number.toUtf8());

    Output.push_back(enable);
    Output.push_back(visible);
    Output.push_back(rights);

    QString textDate = regdate.toString("yyyy-MM-dd hh:mm:ss");
    Output.push_back((quint8)textDate.length());
    Output.push_back(textDate.toUtf8());

    Output.push_back(activate);
    Output.push_back(changepassword);

    textDate = datepassword.toString("yyyy-MM-dd hh:mm:ss");
    Output.push_back((quint8)textDate.length());
    Output.push_back(textDate.toUtf8());

    Output.push_back(activemail);
    Output.push_back(activenumber);

    Output.push_back((quint8)sizeof(emailcode));
    ptr = (quint8*)(&emailcode);
    for (quint8 i=0;i<sizeof(emailcode);i++)
        Output.push_back(*(ptr+i));

    Output.push_back((quint8)sizeof(numbercode));
    ptr = (quint8*)(&numbercode);
    for (quint8 i=0;i<sizeof(numbercode);i++)
        Output.push_back(*(ptr+i));

    return Output;
}

void QUNAccountInfo::fromByteArray(QByteArray data)
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
    //qDebug()<<"Name size="<<QString::number(size);
    name = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    patronymic = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    surname = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    email = QString(buf);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    number = QString(buf);

    enable = data.at(0);
    data.remove(0,1);

    visible = data.at(0);
    data.remove(0,1);

    rights = data.at(0);
    data.remove(0,1);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    regdate = QDateTime::fromString(QString(buf),"yyyy-MM-dd hh:mm:ss");

    activate = data.at(0);
    data.remove(0,1);

    changepassword = data.at(0);
    data.remove(0,1);

    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    datepassword = QDateTime::fromString(QString(buf),"yyyy-MM-dd hh:mm:ss");

    activemail = data.at(0);
    data.remove(0,1);

    activenumber = data.at(0);
    data.remove(0,1);

    ptr = (quint8*)(&emailcode);
    size = data.at(0);
    //qDebug()<<"Email code size="<<QString::number(size);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    for (int i=0;i<size;i++)
        *(ptr+i) = buf.at(i);

    ptr = (quint8*)(&numbercode);
    size = data.at(0);
    buf = data.mid(1,size);
    data.remove(0,size + 1);
    for (int i=0;i<size;i++)
        *(ptr+i) = buf.at(i);
}

QString QUNAccountInfo::toString(bool *ok)
{
    Q_UNUSED(ok)

    QString Output;
    Output = Output + "id: " + QString::number(id) + " ";
    Output = Output + "login: " + login + " ";
    Output = Output + "password: " + password + " ";
    Output = Output + "name: " + name + " ";
    Output = Output + "patronymic: " + patronymic + " ";
    Output = Output + "surname: " + surname + " ";
    Output = Output + "email: " + email + " ";
    Output = Output + "number: " + number + " ";

    if (enable) Output = Output + "enable: true" + " ";
        else Output = Output + "enable: false" + " ";

    if (visible) Output = Output + "visible: true" + " ";
        else Output = Output + "visible: false" + " ";

    Output = Output + "rights: " + QString::number(rights) + " ";
    Output = Output + "regdate: " + regdate.toString("yyyy-MM-dd hh:mm:ss") + " ";

    if (activate) Output = Output + "activate: true" + " ";
        else Output = Output + "activate: false" + " ";

    if (changepassword) Output = Output + "changepassword: true" + " ";
        else Output = Output + "changepassword: false" + " ";

    Output = Output + "datepassword: " + datepassword.toString("yyyy-MM-dd hh:mm:ss") + " ";

    if (activemail) Output = Output + "activemail: true" + " ";
        else Output = Output + "activemail: false" + " ";

    if (activenumber) Output = Output + "activenumber: true" + " ";
        else Output = Output + "activenumber: false" + " ";

    Output = Output + "emailcode: " + QString::number(emailcode) +  " ";
    Output = Output + "numbercode: " + QString::number(numbercode) + " ";


    return Output;
}
