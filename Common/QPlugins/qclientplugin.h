#ifndef QCLIENTPLUGIN_H
#define QCLIENTPLUGIN_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include "../QUnNetProtokol/qunnetprotokolclient.h"
#include "../QDataBaseSettings/qdatabasesettings.h"

class QClientPlugin : public QObject
{
    Q_OBJECT
public:
    explicit QClientPlugin(QUnNetProtokolCLient *client,quint16 business, QObject *parent = nullptr);
    virtual ~QClientPlugin();

    virtual void startSynchronization(); //Запуск процесса синхронизации

    quint8 pluginCode(); //Возвращает код плагина
    QString pluginName(); //Возвращает имя плагина
    quint16 businessId(); //Возвращает идентификатор деятельности
    quint32 version(); //Возвращает версию плагина

    bool initDataBase(); //Запускает инициализацию внутренней оперативной БД
    void deinitDataBase();

    QSqlQuery *execQuery(QString textQuery, bool *ok=NULL); //Выполняет запрос во внутреннюю БД
    int startId = 2147478647; //Начало временных id
    int getNextId(QString table, QString idName);


    QStringList getDebugTableist();
    QStringList getTableHeader(QString tableName);

protected:
    quint8 currentPluginCode = 0xFF; //Задается уникальный код плагина
    QString currentPluginName = "QServerPlugin"; //Уникальное имя плагина
    quint16 currentIdBusiness = 0xffff; //Уникальный идентификатор деятельности
    quint32 currentVersion = 0xffffffff; //Версия плагина

    QUnNetProtokolCLient *currentClient;
    QString connectionNameDB;
    QSqlDatabase *dataBase;
    QMutex *mutexDataBase;

    virtual void processCommand(quint8 command,QByteArray data); //Обработчик комманд плагина
    bool sendExtData(quint8 command,QByteArray &data); //Функция для отправки данных клиентам

    virtual void dataBaseIsInit(); //Вызывается в случае удачной инициализаци БД и в ней можно расположить код создания таблиц

    void debugMessage(QString message); //ОТправить отладочное сообщщение

signals:
    void signalPluginDebugMessage(quint8 plugCode,QString plugName, quint16 business, QString message);

    void signalBeginSynchronization(); //Сигнал начала синхронизации данных с сервером
    void signalEndSynchronization(); //Сигнал окончания синхронизации
    void signalMessageSplashScreen(QString message); //Сообщение процесса синхронизации

private slots:
    void slotNewExtPacket (quint8 plugin,quint8 command, quint16 idBusiness, QByteArray data);

};

#endif // QCLIENTPLUGIN_H
