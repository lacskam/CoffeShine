#ifndef QNNPREDICTIONINFO_H
#define QNNPREDICTIONINFO_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QDate>
#include <QDateTime>
#include <QDataStream>
#include "../QColorConsole/qcolorconsole.h"
#include <QThread>
#include <QByteArray>
#include <QDataStream>
#include <QDate>
#include <QMap>
#include <QDateTime>
#include "../../../../coffeeShineServer1/coffeeShineServer/include/QNn/nn.h"



class Command22Worker : public QObject {
    Q_OBJECT

public:
    Command22Worker(QByteArray data);


signals:
    void finished();
    void step(qint32 step);

public slots:
    void process();

private:
    QByteArray data;
};


class Command18Worker : public QObject {
    Q_OBJECT

public:
    Command18Worker(QByteArray data,QMap<QString,QString> &config);

signals:
    void finished(QByteArray result);

public slots:
    void process();

private:
    QByteArray data;
    QMap<QString,QString> config;
};



class Command17Worker : public QObject {
    Q_OBJECT

public:
    Command17Worker(QByteArray data,QMap<QString,QString> &config);

signals:
    void finished(QByteArray result);

public slots:
    void process();

private:
    QByteArray data;
    QMap<QString,QString> config;
};

#endif // QNNPREDICTIONINFO_H
