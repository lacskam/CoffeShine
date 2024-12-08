#ifndef QSERVERPLUGIN_H
#define QSERVERPLUGIN_H

#include <QObject>
#include <QMutex>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QTimer>

#include <QDebug>

#include "../QUnNetProtokol/qunnetprotokolserver.h"
#include "../QDataBaseSettings/qdatabasesettings.h"

class QServerPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QServerPlugin(QUnNetProtokolServer *server,quint16 business, QObject *parent = nullptr);
    virtual ~QServerPlugin();

    quint8 pluginCode();
    QString pluginName();
    quint16 businessId();
    quint32 version();

    bool initExtDataBase(QDataBaseSettings &settingsDB);
    void deinitExtDataBase();

    QSqlQuery *execQuery(QString textQuery, bool *ok=NULL);
    QSqlQuery *execQuery(QString textQuery, QString bindName, QVariant bindValue, bool *ok=NULL);

    void setBusinessId(quint16 id);

protected:
    quint8 currentPluginCode = 0xFF; //Задается уникальный код плагина
    QString currentPluginName = "QServerPlugin"; //Уникальное имя плагина
    quint16 currentIdBusiness = 0xffff; //Уникальный идентификатор деятельности
    quint32 currentVersion = 0xffffffff; //Версия плагина

    QUnNetProtokolServer *currentServer;
    QVector<QUnClientHandler*> clientsLists;
    QDataBaseSettings currentSettingsDB;
    QString connectionNameDB;
    QSqlDatabase dataBase;
    QMutex *mutexDataBase;

    QTimer *timerDB;

    virtual void processCommand(quint8 command,QByteArray data, QUnClientHandler *client); //Обработчик комманд плагина
    bool sendExtData(quint8 command,QByteArray &data,QUnClientHandler *client); //Функция для отправки данных клиентам

    virtual void dataBaseIsInit();
    int getNextId(QString table, QString idName);

    void debugMessage(QString message); //ОТправить отладочное сообщщение

private:


signals:
    void signalPluginDebugMessage(quint8 plugCode,QString plugName, quint16 business, QString message);

public slots:
    void slotNewExtData (quint8 plugin, quint8 command, quint16 idBusiness, QByteArray data, QUnClientHandler *client);
    void slotNewConnection (QUnClientHandler *client);
    void slotClientDisconnected();

private slots:
    void slotTimerDB();
};

#endif // QSERVERPLUGIN_H
