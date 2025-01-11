#ifndef QCOFFEECLIENTPLUGIN_H
#define QCOFFEECLIENTPLUGIN_H

#include <QObject>
#include <QDataStream>
#include <QSqlError>
#include "../qclientplugin.h"

#include "QCoffeeCategoryInfo/qcoffeecategoryinfo.h"
#include "QCoffeeDrinkInfo/qcoffeedrinkinfo.h"
#include "QCoffeePointSale/qcoffeepointsale.h"
#include "QCoffeePriceInfo/qcoffeepriceinfo.h"
#include "QCoffeeReceiptInfo/qcoffeereceiptinfo.h"
#include "QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.h"
#include "QCoffeeTypeDrink/qcoffeetypedrink.h"
#include "QCoffeeUserInfo/qcoffeeuserinfo.h"
#include "QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.h"
#include "QCoffeeReceiptInfo/qcoffeereceiptinfo.h"
#include "QCoffeeShiftInfo/qcoffeeshiftinfo.h"
#include "QCoffeePictureInfo/qcoffeepictureinfo.h"


class QCoffeeClientPlugin : public QClientPlugin
{
    Q_OBJECT
public:
    QCoffeeClientPlugin(int idPointSale,QUnNetProtokolCLient *client,quint16 business, QObject *parent);
    QCoffeeClientPlugin(QUnNetProtokolCLient *client,quint16 business, QObject *parent);
    ~QCoffeeClientPlugin();

    void startSynchronization();

    void closeShift(); //Закрытие текущей смены
    void sale(QCoffeeReceiptInfo &receipt, QVector<QCoffeeSoldPositionInfo> &soldPositions);
    QCoffeeShiftInfo getCurrentShift();            //Получить информацию о текущей смене
    QCoffeeUserInfo getCurrentUserInfo();         //Получить информацию о текущем пользователе
    void sendPicture (QCoffeePictureInfo &info); //Отправить картинку на сервер

    // update date in database
    // cud - create, update, delete(0x01,0x02,0x03)


    bool tesst = false;



    //Database

    //QCoffeePointSale
    QVector<QCoffeePointSale> getListPointSale();
    QVector<int> getDrinkIdForPointSale(int idPointSale);
    QVector<int> getCategoryIdForPointSale(int idPointSale);
    QCoffeePointSale getPointSaleInfo(int idPointSale);
    bool editPointSaleInfo(QCoffeePointSale pointSaleInfo);
    bool  unlinkPointSaleAndDrink(int idPointSale);
    bool  unlinkPointSaleAndCategory(int idPointSale);
    bool linkPointSaleAndDrink(int idPointSale,int idDrink);
    bool linkPointSaleAndCategory(int idPointSale,int idCategory);
    int getPointSaleId(QCoffeePointSale pointSale);
    int addPointSale(QCoffeePointSale pointSale);

    //QCoffeeCategoryInfo
    QVector<QCoffeeCategoryInfo> getListCategories();
    QCoffeeCategoryInfo getCategoryInfo(int idCategory);
    QVector<QCoffeeCategoryInfo> getListCategories(int idPointSale);
    QVector<int> getIdPointSaleForCatigory(int idCategory);
    bool linkCategoryAndPointSale(int idCategory, int idPointSale);
    bool unlinkCategoryAndPointSale(int idCategory);
    int getIdCategory(QCoffeeCategoryInfo &category);
    bool editCategory(QCoffeeCategoryInfo &category);
    int addCategory(QCoffeeCategoryInfo &category);
    bool deleteCategoryInfo(QCoffeeCategoryInfo &categoryInfo);

    // QCoffeeUserInfo
    QVector<QCoffeeUserInfo> getListUser();
    QCoffeeUserInfo getUserInfo(int idUser);
    int getIdUser(QCoffeeUserInfo &userInfo);
    int addUser(QCoffeeUserInfo &userInfo);
    bool editUser(QCoffeeUserInfo &userInfo);

    //QCoffeeTypeDrink
    QVector<QCoffeeTypeDrink> getListTypeDrink();
    QCoffeeTypeDrink getTypeDrinkInfo (int idTypeDrink);
    int getIdTypeDrink (QCoffeeTypeDrink &typeDrink);
    int addTypeDrink (QCoffeeTypeDrink &typeDrink);
    bool editTypeDrink (QCoffeeTypeDrink &typeDrink);

    //QCoffeeVolumeDrinkInfo
    QVector<QCoffeeVolumeDrinkInfo> getListVolume();
    QVector<QCoffeeVolumeDrinkInfo> getListVolume(int idDrink);
    QCoffeeVolumeDrinkInfo getVolumeInfo(int idVolume);
    int getVolumeId(QCoffeeVolumeDrinkInfo &volume);
    QVector<int> getIdDrinkForVolume(int idVolume);
    bool linkVolumeAndDrink(int idVolume, int idDrink);
    bool unlinkVolumeAndDrink(int idVolume);
    bool unlinkVolumeAndDrink2(int idVolume,int idDrink);
    int addVolumeDrink(QCoffeeVolumeDrinkInfo &volume);
    bool editVolume(QCoffeeVolumeDrinkInfo &volume);

    //QCoffeeDrinkInfo
    QVector<QCoffeeDrinkInfo> getListDrink();
    QVector<QCoffeeDrinkInfo> getListDrink(int idCategory);
    QVector<int> getListIdDrink();
    QVector<int> getIdDrinkForCategory(int idCategory);
    QVector<int> getIdVolumeForDrink(int idDrink);
    QVector<int> getIdCategoriesForDrink(int idDrink);
    QCoffeeDrinkInfo getDrinkInfo(int idDrink);
    int addDrinkInfo(QCoffeeDrinkInfo &drinkInfo);
    int getIdDrink (QCoffeeDrinkInfo &drinkInfo);
    bool editDrinkInfo(QCoffeeDrinkInfo &drinkInfo);
    bool deleteDrinkInfo(QCoffeeDrinkInfo &drinkInfo);
    bool unlinkDrinkAndCategory(int idDrink);
    bool linkDrinkAndCategory(int idDrink,int idCategory);

    bool unlinkCategoryAndDrink(int idCategory);

    //QCoffeePriceInfo
    QVector<QCoffeePriceInfo> getListPriceInfo();
    QCoffeePriceInfo getPriceInfo(int idPriceInfo);
    QCoffeePriceInfo getPricceInfo(int idCategory,int idDrink,int idVolume);
    int getIdPriceInfo(QCoffeePriceInfo &priceInfo);
    QVector<int> getIdPointSaleForPriceInfo(int idPriceinfo);
    bool unlinkPriceInfoAndPointSale(int idPriceInfo);
    bool linkPriceInfoAndPointSale(int idPriceInfo, int idPointSale);
    int addPriceInfo(QCoffeePriceInfo &priceInfo);
    bool editPriceInfo(QCoffeePriceInfo &priceInfo);

    //QCoffeeReceiptInfo
    QVector<QCoffeeReceiptInfo> getListReceiptInfo();
    QVector<QCoffeeReceiptInfo> getListReceiptInfoForShift(int idShift);
    QVector<QCoffeeReceiptInfo> getListReceiptInfoForPointSale(int idPointSale);
    QCoffeeReceiptInfo getReceiptInfo(int idReceiptInfo);
    int getIdReceiptInfo(QCoffeeReceiptInfo &receiptInfo);
    int addReceiptInfo(QCoffeeReceiptInfo &receiptInfo);
    bool editReceiptInfo(QCoffeeReceiptInfo &receiptInfo);

    //QCoffeeSoldPositionInfo
    QVector<QCoffeeSoldPositionInfo> getListSoldPosition();
    QVector<std::tuple<int, QString,int, double,QDate>> getListSoldPositionForIdAndDates(qint32 &id,QDate &firstDate,QDate &lastDate);
    QVector<QCoffeeSoldPositionInfo> getListSoldPositionForReceipt(int idReceipt);
    QCoffeeSoldPositionInfo getSoldPosition(int idSoldPosition);
    int getIdSoldPosition (QCoffeeSoldPositionInfo &soldPosition);
    int addSoldPosition(QCoffeeSoldPositionInfo &soldPosition);
    bool editSoldPosition(QCoffeeSoldPositionInfo &soldPosition);
    void deleteSolfPosition (int idSoldPosition);

    //QCoffeeShiftInfo
    QVector<QCoffeeShiftInfo> getListShift();                     //Получить список смен
    QCoffeeShiftInfo getShiftInfo(int idShift);                  //Получить информацию о смене
    QCoffeeShiftInfo getShiftInfo(int idUser, int idPointSale); //Получить информацию о смене по пользователю и точке продаж
    QCoffeeShiftInfo getOpenShift(int idUser,int idPointSale); //Получить открытую смену для пользователя и точки продажи
    int getIdShift(QCoffeeShiftInfo &shiftInfo);              //Получить id смены
    int addShift(QCoffeeShiftInfo &shiftInfo);               //Добавить смену
    bool editShift(QCoffeeShiftInfo &shiftInfo);            //Редактировать смену
    bool checkOpenShift(int idUser,int idPointSale);       //Проверить наличие открытой смены

    //QCoffeePictureInfo
    QVector<QCoffeePictureInfo> getListPicture();
    QCoffeePictureInfo getPicture (int idPicture);
    int addPicture (QCoffeePictureInfo &picture);
    int getIdPicture(QCoffeePictureInfo &picture);
    void editPicture(QCoffeePictureInfo &picture);

    void sendRelearnRequest();
    void sendPredictionRequest(QDate startDate, QDate endDate, quint32 id, qint32 idWg); // Отправление информации о дате и айди для прогноза
    void sendPredictionRequestFotStat(QDate startDate, QDate endDate, qint32 idstart, qint32 idend);

    void crudDrinkInfo(QCoffeeDrinkInfo &drinkInfo,quint32 idOperation);

    void crudCategoryInfo(QCoffeeCategoryInfo &categoryInfo,QVector<qint32> newDrinks,quint32 idOperation);

    void crudVolumeInfo(QCoffeeVolumeDrinkInfo &volumeInfo,quint32 idOperation);

    void crudPriceInfo(QCoffeePriceInfo &priceInfo,quint32 idOperation);



    void setIdCurrentPointSale(int id);
     int getIdCurrentPointSale();

    void getListNnVersions();
     void sendNnConfig(QMap<QString, QString> &config);


private:





    QMutex *mutexCurrentUserInfo;
    int idCurrentPointSale;
    bool desktopClient = false;
    void processCommand(quint8 command,QByteArray data);
    void dataBaseIsInit();

    QMutex *mutexDBPicture;
    QSqlDatabase dataBasePicture;
    void initDBPicture();
    QSqlQuery *execQueryPicture(QString textQuery, bool *ok=NULL);
    QSqlQuery *execQueryPicture(QString textQuery, QString bindName, QVariant bindValue, bool *ok=NULL);

    QCoffeePointSale currentPointSale; //Информация о текущей точке продаж
    QCoffeeShiftInfo currentShift; //Информация о текущей смене
    QCoffeeUserInfo currentUserInfo; //Информация о ткущем пользователе


    //Synchronization
    bool firstRun = true; //Флаг первого подключения
    int currentStep = 0; //Текущий шаг синхронизации
    void doStep (); //Выполнение текущего шага синхронизации
    void nextStep(); //Следующий шаг синхронизации
    void endSynchronization(); //Завершение синхронизации

    //Commands
    QVector<QCoffeeReceiptInfo> receipt; //Список чеков
    QVector<int> idPictures; //id картинок напитков
    void loadAllPrices();
    void loadCurrentPintSaleInfo(int idPointSale);
    void command01(QByteArray &data); //Плучение информации о точке продаж
    void loadCategoriesForPointSale(); //Запросить список категорий для точки продаж
    void command02(QByteArray &data); //Получение списка категорий
    void loadDrinkInfo(); //Запросить ифнормацию о напитках
    void command03(QByteArray &data); //Получение информации о напитках
    void loadVolumeDrink(); //Запрос списка объемов
    void command04(QByteArray &data); //Получение списка объемов
    void loadTypeDrink(); //Запрос списка типов напитков
    void command05(QByteArray &data); //Получение списка типов напитков
    void loadPriceInfo(int idPointSale); //Запрос позиций прайса для точки продаж
    void command06(QByteArray &data); //Получение позиций прайса
    void loadUserInfo(); //Запрос информации о пользователе
    void command07(QByteArray &data); //Получение информации о пользователе
    void openShift(int idPointSale); //Запрос на открытие смены
    void command08(QByteArray &data); //Результат открытия смены
    void closeShift(QCoffeeShiftInfo &shift); //Закрыть смену
    void command09(QByteArray &data); //Результат закрытия смены
    void loadReceipt (QCoffeeShiftInfo &shift); //Запрос чеков для смены
    void command0A(QByteArray &data); //Получение списка чеков
    void loadSoldPosition(QVector<QCoffeeReceiptInfo> &receipt); //Запрос проданных позций
    void command0B(QByteArray &data); //Получение проданных позиций
    void sendSale(QCoffeeReceiptInfo &receipt, QVector<QCoffeeSoldPositionInfo> &soldPositions); //Отправить информацию о продаже
    void command0C(QByteArray &data); //Возвращение информации о продаже
    void uploadPicture(QCoffeePictureInfo &picture); //Выгрузить фотографию на сервер
    void command0D(QByteArray &data); //Результат выгрузки картинки на сервер
    void updateDrinkPicture(); //Обновить картинки напитков
    void getPictureInformation(int idPicture); //Запросить информацию о картинке
    void command0E(QByteArray &data); //Получение информации о картинке
    void loadPointSales();  //Получение id точек продаж
    void command0F(QByteArray &data);
    void loadPointSale(int idPointSale);  // Запрос на точку продаж
    void command10(QByteArray &data); //Получение точки продаж
    void loadAllCategoryes();           // Запрос на все категории
    void command11(QByteArray &data);  // Получение категорий
    void loadAllDrinks();               // Запрос на все напитки
    void command12(QByteArray &data);   // Получение напитков
    void loadAllgetCategoryInfoPrices();               // Запрос на все цены
    void command13(QByteArray &data);   // Получение цен
    void loadAllShifts();               // Запрос на все смены
    void command14(QByteArray &data);   // Получение смен
    void loadAllReceipt();              // Запрос на все чеки
    void command15(QByteArray &data);   // Получение чеков
    void loadAllSoldPositions();        // Запрос на все проданные позиции
    void command16(QByteArray &data);   // Получение проданных позиций

    void crudOpDrinkInfo(QCoffeeDrinkInfo &categoryInfo,quint32 idOperation); // Отправление информации о напитке
    void commandA0(QByteArray &data);   // Возвращение информации о напитке


    void command19(QByteArray &data);     //получение категории

    void command17(QByteArray &data);    //получение прогноза нейронки
    void command18(QByteArray &data);    //получение прогноза для диапазона товаров
    void command20(QByteArray &data);   //получение обьема
     void command21(QByteArray &data);  //получение цены
    void command22(QByteArray &data); // получение статуса
    void command23(QByteArray &data); //получение версий нейронки




signals:
    void signalNewPointInfo(QCoffeePointSale info);
    void signalNewCategories();
    void signalNewDrink();
    void signalNewVOlume();
    void signalNewDrinkType();
    void signalNewPrice();
    void signalNewUserInfo();
    void signalNewShift();
    void signalNewReceipt();
    void signalNewSoldPosition();
    void signalCloseShift();
    void signalNewPicture();
    void signalImageUploaded(bool result);
    void predictionDataReceived(QMap<QDateTime, float> predictionResults);
    void predictionDataReceivedToStat(QMap<QDateTime, float> predictionResults,qint32 id);
    void predictionDataReceivedForStart(QMap<qint32, float> predictionResults);
    void signalNewCategory();
    void signalVolumeGetted();
    void signalNnVersiomsGetted(QList<QString> &versions,QMap<QString,QString> &config,qint32 &progressLearn);
    void signalSendStatus(qint32 &status);

};

#endif // QCOFFEECLIENTPLUGIN_H
