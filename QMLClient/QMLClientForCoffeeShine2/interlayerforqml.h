#ifndef INTERLAYERFORQML_H
#define INTERLAYERFORQML_H

#include <QObject>
#include <QQmlPropertyMap>
#include <QMessageBox>

#include "../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.h"
#include "../../Common/QUnNetProtokol/qunnetprotokolclient.h"

#include <QVector>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>

#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"

#include "DrinkImageProvider/drinkimageprovider.h"

class InterlayerForQML : public QObject
{
    Q_OBJECT
public:
    explicit InterlayerForQML(QObject *parent = nullptr);

    DrinkImageProvider *drinkImageProvider;
    QQmlPropertyMap *qmlProperty;

private:
    QCoffeeClientPlugin *currentPlugin;
    QUnNetProtokolCLient *client;
    QSettings *settings;

    QVector<HostInfo> vectorForSaveHosts;

    bool sendDisconectToExit = false;
signals:
    void createTab_Signal();
    void createMenu_Signal();
    void createVolume_Signal();
    void createVarietyOfPay_Signal();
    void createHostAndPort_Signal();
    void createReceipt_Signal();
    void createSoldPosition_Signal();
    void sendImageFromDB_Signal(QImage &image);

    void finishCreateTabs_Signal();
    void somePortNotValid_Signal();

    void fillOpenDate_Signal();
    void fillingSomePropertyInReportView_Signal();

    void getSomePropertyForSoldPosition_Signal(int index);

    void goodAuthorized_Signal();
    void badAuthorized_Signal();
    void sendMessageToSplashScreen_Signal(QString message);

public slots:
    bool getOSAndroidOrNot();
    void getImageFromDB_Slot(int);

    void getWarnings(QList<QQmlError>);
    void fillingListTabs_Slot();
    void fillingListMenu_Slot(int);
    void fillingListVolume_Slot(int);
    void fillingListVarietyOfPay_Slot();
    void fillingHostAndPortListFromQSettings_Slot();
    void fillingReceiptList_Slot();
    void fillingSoldPositionList_Slot(int);

    void createReceipt_Slot(int);

    bool getPrice(int,int,int);

    void saveIdPointSaleInQSettings_Slot();
    void getIdPointSaleFromQSettings_Slot();

    void saveLastLoginInQSettings_Slot();
    void getLastLoginFromQSettings_Slot();

    void saveHostAndPortInInterlayer_Slot();
    void saveHostAndPortInQSettings_Slot();

    void connectToHost_Slot();
    void disconnectFromHost_Slot();

    void exitApp();

    void slotBeginSynchronization();
    void slotEndSynchronization();
    void slotConnected();
    void slotAuthorized();
    void slotDisconnected();
    void slotUserInfoReceived();
    void slotError (UnErr err);
};

#endif // INTERLAYERFORQML_H

