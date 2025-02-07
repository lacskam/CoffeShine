#ifndef STARTAPP_H
#define STARTAPP_H

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "interlayerforqml.h"
#include "DrinkImageProvider/drinkimageprovider.h"

class StartApp : public QObject
{
    Q_OBJECT
public:
    explicit StartApp(QObject *parent = nullptr);
    ~StartApp();
    QQmlApplicationEngine *engine;
    InterlayerForQML *interlayerForQML;
signals:

public slots:
    void saveQmlEngineWarnings(QList<QQmlError>);
};

#endif // STARTAPP_H
