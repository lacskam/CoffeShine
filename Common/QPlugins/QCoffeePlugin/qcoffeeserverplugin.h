#ifndef QCOFFEESERVERPLUGIN_H
#define QCOFFEESERVERPLUGIN_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include "QColorConsole/qcolorconsole.h"

#include <QDebug>

#include "../qserverplugin.h"
#include "QCoffeeCategoryInfo/qcoffeecategoryinfo.h"
#include "QCoffeeDrinkInfo/qcoffeedrinkinfo.h"
#include "QCoffeePointSale/qcoffeepointsale.h"
#include "QCoffeePriceInfo/qcoffeepriceinfo.h"
#include "QCoffeeReceiptInfo/qcoffeereceiptinfo.h"
#include "QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.h"
#include "QCoffeeTypeDrink/qcoffeetypedrink.h"
#include "QCoffeeUserInfo/qcoffeeuserinfo.h"
#include "QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.h"
#include "QCoffeeShiftInfo/qcoffeeshiftinfo.h"
#include "QCoffeePictureInfo/qcoffeepictureinfo.h"
#include "QNnPredictionInfo/qnnpredictioninfo.h"
#include "../../../coffeeShineServer1/coffeeShineServer/include/QNn/nn.h"



class QCoffeeServerPlugin : public QServerPlugin
{
public:
    QCoffeeServerPlugin(QUnNetProtokolServer *server, quint16 business, QObject *parent = nullptr);
    ~QCoffeeServerPlugin();

    //QCoffeePointSale
    QVector<QCoffeePointSale> getListPointSale(); //Загруззить список точек продаже
    QVector<int> getDrinkIdForPointSale(int idPointSale); //Загрузить саисок id напитков для точки продаж
    QVector<int> getCategoryIdForPointSale(int idPointSale); //Загрузить саисок id категорий для точки продаж

    QCoffeePointSale getPointSaleInfo(int idPointSale); //Получить инфу по точке продажи по id
    bool editPointSaleInfo(QCoffeePointSale pointSaleInfo); //Редактировать информацию о точке продаж
    bool unlinkPointSaleAndDrink(int idPointSale); //Удалить все связи точки продаж и напитков
    bool unlinkPointSaleAndCategory(int idPointSale); //Удалить все связи точки продаж и категорий
    bool linkPointSaleAndDrink(int idPointSale,int idDrink); //Связать точку продаж и напиток
    bool linkPointSaleAndCategory(int idPointSale,int idCategory); //Связать точку продаж и категорию
    int getPointSaleId(QCoffeePointSale pointSale); //Получить id точки продаж
    int addPointSale(QCoffeePointSale pointSale); //Добавление новой точки продаж


    //QCoffeeCategoryInfo
    QVector<QCoffeeCategoryInfo> getListCategories(); //Загрузить список категорий
    QCoffeeCategoryInfo getCategoryInfo(int idCategory); //Получить информацию по категории
    QVector<QCoffeeCategoryInfo> getListCategories(int idPointSale); //Загрузить список категорий по id точки продаж
    QVector<int> getIdPointSaleForCatigory(int idCategory); //Получить список id точек продаж для категории
    bool linkCategoryAndPointSale(int idCategory, int idPointSale); //Связать категорию и точку продаж
    bool unlinkCategoryAndPointSale(int idCategory); //Удалить связи заданной категории и точек продаж
    int getIdCategory(QCoffeeCategoryInfo &category); //Получить id категории
    bool editCategory(QCoffeeCategoryInfo &category); //Редактировать категорию
    int addCategory(QCoffeeCategoryInfo &category); //Добавить категорию


    //QCoffeeUserInfo
    QVector<QCoffeeUserInfo> getListUser(); //Получить список пользователей
    QCoffeeUserInfo getUserInfo(int idUser); //Получить информацию о пользователе
    QCoffeeUserInfo getUserInfo(QString login); //Получить информацию о пользователе
    int getIdUser(QCoffeeUserInfo &userInfo); //Получить id пользователя
    int addUser(QCoffeeUserInfo &userInfo); //Добавить пользователя
    bool editUser(QCoffeeUserInfo &userInfo); //Редактировать данные пользователя


    //QCoffeeTypeDrink
    QVector<QCoffeeTypeDrink> getListTypeDrink(); //Загрузить список типов напитков
    QCoffeeTypeDrink getTypeDrinkInfo (int idTypeDrink); //Получить информацию о типе напитка
    int getIdTypeDrink (QCoffeeTypeDrink &typeDrink); //Получить id типа напитка
    int addTypeDrink (QCoffeeTypeDrink &typeDrink); //Добавить тип напитка
    bool editTypeDrink (QCoffeeTypeDrink &typeDrink); //Редактировать тип напитка


    //QCoffeeVolumeDrinkInfo
    QVector<QCoffeeVolumeDrinkInfo> getListVolume(); //Получить список объемов напитков
    QVector<QCoffeeVolumeDrinkInfo> getListVolume(int idDrink); //Получить список объемов наптка
    QCoffeeVolumeDrinkInfo getVolumeInfo(int idVolume); //Получить информацию об объеме
    int getVolumeId(QCoffeeVolumeDrinkInfo &volume); //Получить id объема
    QVector<int> getIdDrinkForVolume(int idVolume); //Получить id напитков для заданного объема
    bool linkVolumeAndDrink(int idVolume, int idDrink); //Свзять объем и напиткоу
    bool unlinkVolumeAndDrink(int idVolume); //Удалить все связи объема с напитками
    int addVolumeDrink(QCoffeeVolumeDrinkInfo &volume); //Добавить объем напитка
    bool editVolume(QCoffeeVolumeDrinkInfo &volume); //Редактировать объем напитка


    //QCoffeeDrinkInfo
    QVector<QCoffeeDrinkInfo> getListDrink(); //Получить список напитков
    QVector<int> getIdVolumeForDrink(int idDrink); //Получить id объемов для напитка
    QVector<int> getIdCategoriesForDrink(int idDrink); //Получить id категорий для напитка
    QCoffeeDrinkInfo getDrinkInfo(int idDrink); //Получить информацию о напитке по id
    int addDrinkInfo(QCoffeeDrinkInfo &drinkInfo); //Добавить напиток
    int getIdDrink (QCoffeeDrinkInfo &drinkInfo); //Получить id напитка
    bool editDrinkInfo(QCoffeeDrinkInfo &drinkInfo); //Редактировать информацию о напитке
    bool deleteDrinkInfo(QCoffeeDrinkInfo &drinkInfo); // удалить напиток
    bool unlinkDrinkAndCategory(int idDrink); //Удалить все связи напитка и категорий
    bool linkDrinkAndCategory(int idDrink,int idCategory); //Связать напиток и категорию

    //QCoffeePriceInfo
    QVector<QCoffeePriceInfo> getListPriceInfo(); //Получить список все похиций прайса
    QVector<QCoffeePriceInfo> getListPricePointSale(int idPointSale); //Получить позиции прайса для точки продаж
    QCoffeePriceInfo getPriceInfo(int idPriceInfo); //Получить информацию по позиции прайса
    int getIdPriceInfo(QCoffeePriceInfo &priceInfo); //Получить id позиции прайса
    QVector<int> getIdPointSaleForPriceInfo(int idPriceinfo); //Получить список id точек продаж для позиции прайса
    bool unlinkPriceInfoAndPointSale(int idPriceInfo); //Удалить все связи между позицией прайса и точками продаж
    bool linkPriceInfoAndPointSale(int idPriceInfo, int idPointSale); //Связать позицию прайса и точку продаж
    int addPriceInfo(QCoffeePriceInfo &priceInfo); //Добавить позицию прайса
    bool editPriceInfo(QCoffeePriceInfo &priceInfo); //Редактировать позицию прайса

    //QCoffeeReceiptInfo
    QVector<QCoffeeReceiptInfo> getListReceiptInfo(); //Получить список всех чеков
    QVector<QCoffeeReceiptInfo> getListReceiptInfo(int idShift); //Получение списка чеков для смены
    QCoffeeReceiptInfo getReceiptInfo(int idReceiptInfo); //Получить информацию о чеке по id
    int getIdReceiptInfo(QCoffeeReceiptInfo &receiptInfo); //Получить id чека
    int addReceiptInfo(QCoffeeReceiptInfo &receiptInfo); //Добавить чек
    bool editReceiptInfo(QCoffeeReceiptInfo &receiptInfo); //Редактировать чек
    void deleteSolfPosition (int idSoldPosition); //Удаление проданой позиции

    //QCoffeeSoldPositionInfo
    QVector<QCoffeeSoldPositionInfo> getListSoldPosition(); //Получить список всех проданных позиций
    QVector<QCoffeeSoldPositionInfo> getSoldPositionsForReceipt(int idReceipt); //Получить список проданных позиций для чека
    QCoffeeSoldPositionInfo getSoldPosition(int idSoldPosition); //Получить информацию о проданной позиции
    int getIdSolfPosition (QCoffeeSoldPositionInfo &soldPosition); //Полцчить id проданной позиции
    int addSoldPosition(QCoffeeSoldPositionInfo &soldPosition); //Добавить проданную позицию
    bool editSoldPosition(QCoffeeSoldPositionInfo &soldPosition); //Редактировать проданную позицию

    //QCoffeeShiftInfo
    QVector<QCoffeeShiftInfo> getListShift(); //Получить список смен
    QCoffeeShiftInfo getShiftInfo(int idShift); //Получить информацию о смене
    QCoffeeShiftInfo getShiftInfo(int idUser, int idPointSale); //Получить информацию о смене по пользователю и точке продаж
    QCoffeeShiftInfo getOpenShift(int idUser,int idPointSale); //Получить открытую смену для пользователя и точки продажи
    int getIdShift(QCoffeeShiftInfo &shiftInfo); //Получить id смены
    int addShift(QCoffeeShiftInfo &shiftInfo); //Добавить смену
    bool editShift(QCoffeeShiftInfo &shiftInfo); //Редактировать смену
    bool checkOpenShift(int idUser,int idPointSale); //Проверить наличие открытой смены
    QCoffeeShiftInfo openShift(int idUser, int idPointSale, bool *newShift = NULL); //Открыть смену
    bool closeShift(QCoffeeShiftInfo &shift); //Закрыть смену


    //QCoffeePriceInfo
    QCoffeePictureInfo getPicture (int idPicture); //Получить картинку напитка
    int addPicture (QCoffeePictureInfo &picture); //Добавить картинку
    int getIdPicture(QCoffeePictureInfo &picture); //Получить id картинки
    void editPicture(QCoffeePictureInfo &picture); //Редактировать картинку

    //Commands
    void command01 (QByteArray data, QUnClientHandler *client); //Получение информации о точке продаж
    void command02 (QByteArray data, QUnClientHandler *client); //Получение информации о категориях
    void command03(QByteArray data, QUnClientHandler *client); //Получение списка напитков
    void command04(QByteArray data, QUnClientHandler *client); //Получение списка объемов напитков
    void command05(QByteArray data, QUnClientHandler *client); //Получение списка типов напитков
    void command06(QByteArray data, QUnClientHandler *client); //Получение прайса
    void command07(QByteArray data, QUnClientHandler *client); //Получение информации о пользователе
    void command08(QByteArray data, QUnClientHandler *client); //Открыть смену
    void command09(QByteArray data, QUnClientHandler *client); //Закрыть смену
    void command0A(QByteArray data, QUnClientHandler *client); //Получение чеков для смены
    void command0B(QByteArray data, QUnClientHandler *client); //Получение проданных позиций
    void command0C(QByteArray data, QUnClientHandler *client); //Получение продажи
    void command0D(QByteArray data, QUnClientHandler *client); //Выгрузка картинки
    void command0E(QByteArray data, QUnClientHandler *client); //Запрос информации о картинке
    void command0F(QByteArray data, QUnClientHandler *client); //Получение id всех точек продаж
    void command10(QByteArray data, QUnClientHandler *client); //Получение точки продаж
    void command11(QByteArray data, QUnClientHandler *client); //Получение всех категорий
    void command12(QByteArray data, QUnClientHandler *client); //Получение всех напитков
    void command13(QByteArray data, QUnClientHandler *client); //Получение всех цен
    void command14(QByteArray data, QUnClientHandler *client); //Получение всех смен
    void command15(QByteArray data, QUnClientHandler *client); //Получение всех чеков
    void command16(QByteArray data, QUnClientHandler *client); //Получение всех проданых позиций

    void commandA0(QByteArray data, QUnClientHandler *client); // получение информации о напитке
    void command19(QByteArray data,QUnClientHandler *client);

    void command17(QByteArray data, QUnClientHandler *client); //получение прогноза нейронки
    void command18(QByteArray data, QUnClientHandler *client); //получение прогноза среднего значения продаж для диапазона товаров
private:


    void processCommand(quint8 command,QByteArray data, QUnClientHandler *client);

    void dataBaseIsInit();
};

#endif // QCOFFEESERVERPLUGIN_H
