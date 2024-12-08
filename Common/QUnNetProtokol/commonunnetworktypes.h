#ifndef COMMONUNNETWORKTYPES_H
#define COMMONUNNETWORKTYPES_H

#include <QString>

struct HostInfo
{
    QString address;
    int port;
    QString description;
};

enum UnErr
{
    errAuthNone, //Нет ошибок авторизации
    errAuthLogin, //Неверный логин
    errAuthPassword, //Неверный пароль
    errAuthBlock, //Аккаунт заблокирован
    errAuthServWork, //Технические работы на сервере
    errNoRights, //Нет прав на выполнение данной команды
    errNoAuthorized //Пользователь не авторизирован
};

#endif // COMMONUNNETWORKTYPES_H
