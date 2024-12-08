#ifndef QUNACCOUNTINFO_H
#define QUNACCOUNTINFO_H

#include <QObject>
#include <QDateTime>
#include <QDataStream>

class QUNAccountInfo
{
public:
    QUNAccountInfo();

    int id;
    QString login;
    QString password;
    QString name;
    QString patronymic;
    QString surname;
    QString email;
    QString number;
    bool enable;
    bool visible;
    quint8 rights;
    QDateTime regdate;
    bool activate;
    bool changepassword;
    QDateTime datepassword;
    bool activemail;
    bool activenumber;
    int emailcode;
    int numbercode;

    void operator <<(QDataStream &stream);
    void operator >>(QDataStream &stream);

    QByteArray toByteArray();
    void fromByteArray(QByteArray data);
    QString toString(bool *ok=NULL);
};

#endif // QUNACCOUNTINFO_H
