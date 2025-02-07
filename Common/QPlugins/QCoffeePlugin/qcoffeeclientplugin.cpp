#include "qcoffeeclientplugin.h"

QCoffeeClientPlugin::QCoffeeClientPlugin(int idPointSale, QUnNetProtokolCLient *client, quint16 business, QObject *parent)
    :   QClientPlugin(client,business,parent)
{
    idCurrentPointSale = idPointSale;
    currentPluginCode = 0x04;
    currentPluginName = "QCoffeeClientPlugin";
    mutexCurrentUserInfo = new QMutex;
    mutexDBPicture = new QMutex;
}

QCoffeeClientPlugin::QCoffeeClientPlugin(QUnNetProtokolCLient *client, quint16 business, QObject *parent) :   QClientPlugin(client,business,parent)
{
    desktopClient = true;
    currentPluginCode = 0x04;
    currentPluginName = "QCoffeeClientPlugin";
    mutexCurrentUserInfo = new QMutex;
    mutexDBPicture = new QMutex;
}


void QCoffeeClientPlugin::setIdCurrentPointSale(int id) {
    idCurrentPointSale = id;
}

int QCoffeeClientPlugin::getIdCurrentPointSale() {
    return idCurrentPointSale;
}

QCoffeeClientPlugin::~QCoffeeClientPlugin()
{
    delete mutexCurrentUserInfo;
    delete mutexDBPicture;
}

void QCoffeeClientPlugin::startSynchronization()
{
    emit signalBeginSynchronization();
    initDataBase();
}

void QCoffeeClientPlugin::closeShift()
{
    closeShift(currentShift);
}

void QCoffeeClientPlugin::sale(QCoffeeReceiptInfo &receipt, QVector<QCoffeeSoldPositionInfo> &soldPositions)
{
    sendSale(receipt,soldPositions);
}



QCoffeeShiftInfo QCoffeeClientPlugin::getCurrentShift()
{
    return currentShift;
}

QCoffeeUserInfo QCoffeeClientPlugin::getCurrentUserInfo()
{
    QCoffeeUserInfo Output;

    mutexCurrentUserInfo->lock();

    Output = currentUserInfo;

    mutexCurrentUserInfo->unlock();

    return Output;
}

void QCoffeeClientPlugin::sendPicture (QCoffeePictureInfo &info)
{
    uploadPicture(info);
}

void QCoffeeClientPlugin::crudDrinkInfo(QCoffeeDrinkInfo &drinkInfo, quint32 idOperation)
{
    crudOpDrinkInfo(drinkInfo,idOperation);
}

void QCoffeeClientPlugin::processCommand(quint8 command,QByteArray data)
{
    switch (command) {
    case 0x01: command01(data);
        break;
    case 0x02: command02(data);
        break;
    case 0x03: command03(data);
        break;
    case 0x04: command04(data);
        break;
    case 0x05: command05(data);
        break;
    case 0x06: command06(data);
        break;
    case 0x07: command07(data);
        break;
    case 0x08: command08(data);
        break;
    case 0x09: command09(data);
        break;
    case 0x0A: command0A(data);
        break;
    case 0x0B: command0B(data);
        break;
    case 0x0C: command0C(data);
        break;
    case 0x0D: command0D(data);
        break;
    case 0x0E: command0E(data);
        break;
    case 0x0F: command0F(data);
        break;
    case 0x10: command10(data);
        break;
    case 0x11: command11(data);
        break;
    case 0x12: command12(data);
        break;
    case 0x13: command13(data);
        break;
    case 0x14: command14(data);
        break;
    case 0x15: command15(data);
        break;
    case 0x16: command16(data);
        break;
    case 0xA0: commandA0(data);
        break;
    case 0x17: command17(data);
        break;
    case 0x18: command18(data);
        break;
    case 0x19: command19(data);
        break;
    case 0x20: command20(data);
        break;
    case 0x21: command21(data);
        break;
    case 0x22: command22(data);
        break;
    case 0x23: command23(data);
        break;
    case 0x25: command25(data);
        break;
    }
}

void QCoffeeClientPlugin::dataBaseIsInit()
{
    //Создание таблиц внутренней БД

    bool result = false;
    QString textQuery;
    QSqlQuery *queryCreateTable;

    //tbl_drink
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_drink ("
                "id_drink INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "name_drink text NOT NULL,"
                "description_drink text NOT NULL,"
                "red_drink INTEGER NOT NULL,"
                "green_drink INTEGER NOT NULL,"
                "blue_drink INTEGER NOT NULL,"
                "drink_id_pictureDrink INTEGER NOT NULL,"
                "hash_drink text NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_drink:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_drink");
    } else debugMessage("Table tbl_drink is created");

    delete queryCreateTable;



    //tbl_drink_foundation
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_drink_foundation ("
                "tbl_drink_id_drink INTEGER NOT NULL,"
                "tbl_foundation_id_foundation INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_drink_foundation:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_drink_foundation");
    } else debugMessage("Table tbl_drink_foundation is created");

    delete queryCreateTable;



    //tbl_drink_pointSale
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_drink_pointSale ("
                "tbl_drink_id_drink INTEGER NOT NULL,"
                "tbl_pointSale_id_pointSale INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_drink_pointSale:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_drink_pointSale");
    } else debugMessage("Table tbl_drink_pointSale is created");

    delete queryCreateTable;



    //tbl_drink_volumeDrink
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_drink_volumeDrink ("
                "tbl_drink_id_drink INTEGER NOT NULL,"
                "tbl_volumeDrink_id_volumeDrink INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_drink_volumeDrink:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_drink_volumeDrink");
    } else debugMessage("Table tbl_drink_volumeDrink is created");

    delete queryCreateTable;




    //tbl_drinkСategory
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_drinkСategory ("
                "id_drinkСategory INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "name_drinkСategory text NOT NULL,"
                "description_drinkСategory text NOT NULL,"
                "red_drinkСategory INTEGER NOT NULL,"
                "green_drinkСategory INTEGER NOT NULL,"
                "blue_drinkСategory INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_drinkСategory:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_drinkСategory");
    } else debugMessage("Table tbl_drinkСategory is created");

    delete queryCreateTable;




    //tbl_pointSale
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_pointSale ("
                "id_pointSale INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "name_pointSalecol text NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_pointSale:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_pointSale");
    } else debugMessage("Table tbl_pointSale is created");

    delete queryCreateTable;



    //tbl_pointSale_drinkСategory
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_pointSale_drinkСategory ("
                "tbl_pointSale_id_pointSale INTEGER NOT NULL,"
                "tbl_drinkСategory_id_drinkСategory INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_pointSale_drinkСategory:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_pointSale_drinkСategory");
    } else debugMessage("Table tbl_pointSale_drinkСategory is created");

    delete queryCreateTable;



    //tbl_pointSale_price
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_pointSale_price ("
                "tbl_pointSale_id_pointSale INTEGER NOT NULL,"
                "tbl_price_id_price INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_pointSale_price:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_pointSale_price");
    } else debugMessage("Table tbl_pointSale_price is created");

    delete queryCreateTable;



    //tbl_pointSale_price
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_price ("
                "id_price INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "value_price DECIMAL(10,2) NOT NULL,"
                "startTime_price DATETIME NOT NULL,"
                "description_price text NOT NULL,"
                "price_id_drink INTEGER  NOT NULL,"
                "price_id_volumeDrink INTEGER  NOT NULL,"
                "price_id_drinkСategory INTEGER  NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_price:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_price");
    } else debugMessage("Table tbl_price is created");

    delete queryCreateTable;



    //tbl_receipt
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_receipt ("
                "id_receipt INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "date_receipt DATETIME NOT NULL,"
                "sum_receipt DECIMAL(10,2) NOT NULL,"
                "receipt_id_pointSale INTEGER  NOT NULL,"
                "receipt_id_shift INTEGER  NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_receipt:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_receipt");
    } else debugMessage("Table tbl_receipt is created");

    delete queryCreateTable;



    //tbl_soldPosition
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_soldPosition ("
                "id_soldPosition INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "soldPosition_id_user INTEGER NOT NULL,"
                "count_soldPosition INTEGER NOT NULL,"
                "soldPosition_id_drink INTEGER  NOT NULL,"
                "soldPosition_id_typeDrink INTEGER  NOT NULL,"
                "soldPosition_id_price INTEGER  NOT NULL,"
                "soldPosition_id_volumeDrink INTEGER  NOT NULL,"
                "soldPosition_id_receipt INTEGER  NOT NULL,"
                "soldPosition_id_pointSale INTEGER  NOT NULL,"
                "soldPosition_numberInReceipt INTEGER  NOT NULL,"
                "date_soldPosition DATETIME NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_soldPosition:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_soldPosition");
    } else debugMessage("Table tbl_soldPosition is created");

    delete queryCreateTable;



    //tbl_typeDrink
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_typeDrink ("
                "id_typeDrink INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "name_typeDrink text NOT NULL,"
                "description_typeDrink  text NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_typeDrink:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_typeDrink");
    } else debugMessage("Table tbl_typeDrink is created");

    delete queryCreateTable;



    //tbl_user
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_user ("
                "id_user INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "login_user text NOT NULL,"
                "password_user text,"
                "name_user text NOT NULL,"
                "patronymic_user text NOT NULL,"
                "surname_user text NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_user:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_user");
    } else debugMessage("Table tbl_user is created");

    delete queryCreateTable;




    //tbl_volumeDrink
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_volumeDrink ("
                "id_volumeDrink INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "name_volumeDrink text NOT NULL,"
                "volume_volumeDrink INTEGER NOT NULL,"
                "description_volumeDrink text NOT NULL,"
                "units_volumeDrink INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table tbl_volumeDrink:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_volumeDrink");
    } else debugMessage("Table tbl_volumeDrink is created");

    delete queryCreateTable;



    //tbl_shift
    textQuery = "CREATE TABLE IF NOT EXISTS tbl_shift ("
                "id_shift INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                "openTime_shift DATETIME NOT NULL,"
                "closeTime_shift DATETIME NOT NULL,"
                "open_shift INTEGER NOT NULL,"
                "close_shift INTEGER NOT NULL,"
                "shift_id_user INTEGER NOT NULL,"
                "shift_id_pointSale INTEGER NOT NULL);";

    queryCreateTable = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table id_shift:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table id_shift");
    } else debugMessage("Table id_shift is created");

    delete queryCreateTable;


    initDBPicture();
}

void QCoffeeClientPlugin::initDBPicture()
{
    dataBasePicture = QSqlDatabase::addDatabase("QSQLITE","connectionPicture");
    dataBasePicture.setDatabaseName("picture.db");

    QString textQuery = "CREATE TABLE IF NOT EXISTS tbl_pictureDrink ("
                        "id_pictureDrink INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                        "name_pictureDrink text NOT NULL,"
                        "data_pictureDrink blob NOT NULL);";

    bool result;
    QSqlQuery *queryCreateTable = execQueryPicture(textQuery,&result);

    if (!result) {
        qDebug()<<"Error create table picture:"<<queryCreateTable->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
        debugMessage("Error create table tbl_pictureDrink");
    } else debugMessage("Table tbl_pictureDrink is created");

    delete queryCreateTable;
    emit startSynchronization();
    nextStep();
}

QSqlQuery *QCoffeeClientPlugin::execQueryPicture(QString textQuery, bool *ok)
{
    QSqlQuery *Output;

    mutexDBPicture->lock();

    Output = new QSqlQuery(dataBasePicture);


        if (dataBasePicture.isValid()) {
            if (!dataBasePicture.isOpen())
                dataBasePicture.open();

            if (dataBasePicture.isOpen()) {

                if (Output->exec(textQuery)) {
                    if (ok != NULL) *ok = true;

                } else if (ok != NULL) *ok = false;

            } else if (ok != NULL) *ok = false;

        } else if (ok != NULL) *ok = false;



    mutexDBPicture->unlock();

    return Output;
}

QSqlQuery *QCoffeeClientPlugin::execQueryPicture(QString textQuery, QString bindName, QVariant bindValue, bool *ok)
{
    QSqlQuery *Output;

    mutexDBPicture->lock();

    Output = new QSqlQuery(dataBasePicture);
    Output->prepare(textQuery);
    Output->bindValue(bindName,bindValue);

    if (dataBasePicture.isValid()) {
        if (!dataBasePicture.isOpen())
            dataBasePicture.open();

        if (dataBasePicture.isOpen()) {

            if (Output->exec()) {
                if (ok != NULL) *ok = true;

            } else if (ok != NULL) *ok = false;

        } else if (ok != NULL) *ok = false;

    } else if (ok != NULL) *ok = false;


    mutexDBPicture->unlock();

    return Output;
}

//Database

//QCoffeePointSale

QVector<QCoffeePointSale> QCoffeeClientPlugin::getListPointSale()
{
    QVector<QCoffeePointSale>  Output;

    QString textQuery = "select id_pointSale,name_pointSalecol from tbl_pointSale;";

    bool ok = false;
    QSqlQuery *queryPointSale = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeePointSale currentPointSale;

        while (queryPointSale->next()) {
            currentPointSale.id = queryPointSale->value(0).toInt();
            currentPointSale.name = queryPointSale->value(1).toString();

            currentPointSale.idDrinks = getDrinkIdForPointSale(currentPointSale.id);
            currentPointSale.idCategories = getCategoryIdForPointSale(currentPointSale.id);

            Output.push_back(currentPointSale);
        }
    } else {
        qDebug()<<"Error get point sale list:"<<queryPointSale->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPointSale;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getDrinkIdForPointSale(int idPointSale)
{
    QVector<int> Output;

    QString textQuery = "select tbl_drink_id_drink from tbl_drink_pointSale "
                        "where tbl_pointSale_id_pointSale = '" + QString::number(idPointSale) + "';";

    bool ok = false;
    QSqlQuery *queryDrinkId = execQuery(textQuery,&ok);

    if (ok) {
        int currentId;
        while(queryDrinkId->next()) {
            currentId = queryDrinkId->value(0).toInt();

            Output.push_back(currentId);
        }

    } else {
        qDebug()<<"Error get drinks id:"<<queryDrinkId->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryDrinkId;

    return Output;
}





QVector<int> QCoffeeClientPlugin::getCategoryIdForPointSale(int idPointSale)
{
    QVector<int> Output;




    QString textQuery = "select tbl_drinkСategory_id_drinkСategory from tbl_pointSale_drinkСategory "
                        "where tbl_pointSale_id_pointSale = '" + QString::number(idPointSale) + "';";

    bool ok = false;
    QSqlQuery *queryCategoryId = execQuery(textQuery,&ok);

    if (ok) {

        int currentId;
        while (queryCategoryId->next()) {

            currentId = queryCategoryId->value(0).toInt();
            Output.push_back(currentId);
        }
    } else {
        qDebug()<<"Error get id category:"<<queryCategoryId->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryCategoryId;

    return Output;
}

QCoffeePointSale QCoffeeClientPlugin::getPointSaleInfo(int idPointSale)
{
    QCoffeePointSale Output;

    QString textQuery = "select id_pointSale,name_pointSalecol from tbl_pointSale "
                        "where id_pointSale = '" + QString::number(idPointSale) + "';";

    bool ok = false;
    QSqlQuery *queryPointSale = execQuery(textQuery,&ok);

    if (ok) {
        if (queryPointSale->next()) {
            Output.id = queryPointSale->value(0).toInt();
            Output.name = queryPointSale->value(1).toString();

            Output.idDrinks = getDrinkIdForPointSale(Output.id);
            Output.idCategories = getCategoryIdForPointSale(Output.id);
        }

    } else {
        qDebug()<<"Error get point sale info:"<<queryPointSale->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPointSale;

    return Output;
}

bool QCoffeeClientPlugin::editPointSaleInfo(QCoffeePointSale pointSaleInfo)
{
    bool Output = false;

    unlinkPointSaleAndDrink(pointSaleInfo.id);
    unlinkPointSaleAndCategory(pointSaleInfo.id);

    int currentId;
    for (int i=0;i<pointSaleInfo.idCategories.length();i++) {
        currentId = pointSaleInfo.idCategories.at(i);
        linkPointSaleAndCategory(pointSaleInfo.id,currentId);
    }

    for (int i=0;i<pointSaleInfo.idDrinks.length();i++) {
        currentId = pointSaleInfo.idDrinks.at(i);
        linkPointSaleAndDrink(pointSaleInfo.id,currentId);
    }

    QString textQuery = "update tbl_pointSale set name_pointSalecol = '" + pointSaleInfo.name + "' "
                        "where id_pointSale = '" + QString::number(pointSaleInfo.id) + "';";

    QSqlQuery *queryEditPointSale = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit point sale:"<<queryEditPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryEditPointSale;

    return Output;
}

bool  QCoffeeClientPlugin::unlinkPointSaleAndDrink(int idPointSale)
{
    bool Output = false;

    QString textQuery = "delete from tbl_drink_pointSale "
                        "where tbl_pointSale_id_pointSale = '" + QString::number(idPointSale) + "';";

    QSqlQuery *queryUnlinkPointSale = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink point sale and drink:"<<queryUnlinkPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryUnlinkPointSale;

    return Output;
}

bool  QCoffeeClientPlugin::unlinkPointSaleAndCategory(int idPointSale)
{
    bool Output = false;

    QString textQuery = "delete from tbl_pointSale_drinkСategory "
                        "where tbl_pointSale_id_pointSale = '" + QString::number(idPointSale) + "';";

    QSqlQuery *queryUnlinkPointSale = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink point sale and category:"<<queryUnlinkPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryUnlinkPointSale;

    return Output;
}

bool QCoffeeClientPlugin::linkPointSaleAndDrink(int idPointSale,int idDrink)
{
    bool Output = false;

    QString textQuery = "insert into tbl_drink_pointSale (tbl_drink_id_drink,tbl_pointSale_id_pointSale) "
                        "values ('" + QString::number(idDrink)+ "','" + QString::number(idPointSale) + "');";

    QSqlQuery *queryLinkPointSale = execQuery(textQuery,&Output);
    if (!Output) {
        qDebug()<<"Error link point sale and drink:"<<queryLinkPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryLinkPointSale;

    return Output;
}

bool QCoffeeClientPlugin::linkPointSaleAndCategory(int idPointSale,int idCategory)
{
    bool Output = false;

    QString textQuery = "insert into tbl_pointSale_drinkСategory (tbl_pointSale_id_pointSale,tbl_drinkСategory_id_drinkСategory) "
                        "values ('" + QString::number(idPointSale)+ "','" + QString::number(idCategory) + "');";

    QSqlQuery *queryLinkPointSale = execQuery(textQuery,&Output);
    if (!Output) {
        qDebug()<<"Error link point sale and Category:"<<queryLinkPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryLinkPointSale;

    return Output;
}

int QCoffeeClientPlugin::getPointSaleId(QCoffeePointSale pointSale)
{
    int Output = -1;

    QString textQuery = "select id_pointSale from tbl_pointSale "
                        "where name_pointSalecol = '" + pointSale.name + "';";

    bool ok = false;
    QSqlQuery *queryPointSaleId = execQuery(textQuery,&ok);

    if (ok) {
        queryPointSaleId->next();
        Output = queryPointSaleId->value(0).toInt();

    } else {
        qDebug()<<"Error get id point sale:"<<queryPointSaleId->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryPointSaleId;

    return Output;
}

int QCoffeeClientPlugin::addPointSale(QCoffeePointSale pointSale)
{
    int Output = -1;

    QString textQuery = "insert into tbl_pointSale (id_pointSale,name_pointSalecol) "
                        "values ('" + QString::number(pointSale.id) + "','" + pointSale.name + "');";

    bool ok = false;
    QSqlQuery *queryAddPointSale = execQuery(textQuery,&ok);

    if (ok) {
        int currentPointId = getPointSaleId(pointSale);
        unlinkPointSaleAndCategory(pointSale.id);
        if (currentPointId >=0) {
            for (int i=0;i<pointSale.idCategories.length();i++)
                linkPointSaleAndCategory(currentPointId,pointSale.idCategories.at(i));

            for (int i=0;i<pointSale.idDrinks.length();i++)
                linkPointSaleAndDrink(currentPointId,pointSale.idDrinks.at(i));

            Output = currentPointId;
        }


    } else {
        qDebug()<<"Error add point sale:"<<queryAddPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryAddPointSale;

    return Output;
}

//QCoffeeCategoryInfo

QVector<QCoffeeCategoryInfo> QCoffeeClientPlugin::getListCategories()
{
    QVector<QCoffeeCategoryInfo> Output;

    QString textQuery = "select id_drinkСategory,"
                        "name_drinkСategory,"
                        "description_drinkСategory,"
                        "red_drinkСategory,"
                        "green_drinkСategory,"
                        "blue_drinkСategory "
                        "from tbl_drinkСategory;";

    bool ok = false;
    QSqlQuery *queryCategories = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeCategoryInfo currentCategory;
        while (queryCategories->next()) {
            currentCategory.id = queryCategories->value(0).toInt();
            currentCategory.name = queryCategories->value(1).toString();
            currentCategory.description = queryCategories->value(2).toString();

            currentCategory.color.setRed(queryCategories->value(3).toInt());
            currentCategory.color.setGreen(queryCategories->value(4).toInt());
            currentCategory.color.setBlue(queryCategories->value(5).toInt());

            currentCategory.idPointSale = getIdPointSaleForCatigory(currentCategory.id);

            Output.push_back(currentCategory);
        }

    } else {
        qDebug()<<"Error load categories:"<<queryCategories->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryCategories;

    return Output;
}

QCoffeeCategoryInfo QCoffeeClientPlugin::getCategoryInfo(int idCategory)
{
    QCoffeeCategoryInfo Output;

    QString textQuery = "select id_drinkСategory,"
                        "name_drinkСategory,"
                        "description_drinkСategory,"
                        "red_drinkСategory,"
                        "green_drinkСategory,"
                        "blue_drinkСategory "
                        "from tbl_drinkСategory where id_drinkСategory = '" + QString::number(idCategory) + "';";

    bool ok = false;
    QSqlQuery *queryCategory = execQuery(textQuery,&ok);

    if (ok) {
        queryCategory->next();
        Output.id = queryCategory->value(0).toInt();
        Output.name = queryCategory->value(1).toString();
        Output.description = queryCategory->value(2).toString();

        Output.color.setRed(queryCategory->value(3).toInt());
        Output.color.setGreen(queryCategory->value(4).toInt());
        Output.color.setBlue(queryCategory->value(5).toInt());

        Output.idPointSale = getIdPointSaleForCatigory(Output.id);

    } else {
        qDebug()<<"Error get category info:"<<queryCategory->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryCategory;

    return Output;
}

QVector<QCoffeeCategoryInfo> QCoffeeClientPlugin::getListCategories(int idPointSale)
{
    QVector<QCoffeeCategoryInfo> Output;
    QVector<int> idCategories = getCategoryIdForPointSale(idPointSale);

    int currentIdCategory;
    QCoffeeCategoryInfo currentCategory;
    for (int i=0;i<idCategories.length();i++) {
        currentIdCategory = idCategories.at(i);
        currentCategory = getCategoryInfo(currentIdCategory);

        Output.push_back(currentCategory);
    }

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdPointSaleForCatigory(int idCategory)
{
   QVector<int> Output;

   QString textQuery = "select tbl_pointSale_id_pointSale from tbl_pointSale_drinkСategory "
                       "where tbl_drinkСategory_id_drinkСategory = '" + QString::number(idCategory) + "';";

   bool ok = false;
   QSqlQuery *queryIdPointSale = execQuery(textQuery,&ok);

   if (ok) {
       while (queryIdPointSale->next()) {
            Output.push_back(queryIdPointSale->value(0).toInt());
       }
   } else {
       qDebug()<<"Error load point sale id for category:"<<queryIdPointSale->lastError().text();
       qDebug()<<"textQuery ="<<textQuery;
   }

   delete queryIdPointSale;

   return Output;
}

bool QCoffeeClientPlugin::linkCategoryAndPointSale(int idCategory, int idPointSale)
{
    bool Output = false;

    QString textQuery = "insert into tbl_pointSale_drinkСategory (tbl_pointSale_id_pointSale,tbl_drinkСategory_id_drinkСategory) "
                        "values ('" + QString::number(idPointSale) +"','" + QString::number(idCategory) +"');";

    QSqlQuery *queryLinkCategoryAndPointSale = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error link category and point sale:"<<queryLinkCategoryAndPointSale->lastError().text();
        qDebug()<<"textQuery ="<<textQuery;
    }

    delete queryLinkCategoryAndPointSale;

    return Output;
}

bool QCoffeeClientPlugin::unlinkCategoryAndPointSale(int idCategory)
{
    bool Output = false;

    QString textQuery = "delete from tbl_pointSale_drinkСategory "
                        "where tbl_drinkСategory_id_drinkСategory = '" + QString::number(idCategory) + "';";

    QSqlQuery *queryUnlinkCtegory = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink catgory and point sale:"<<queryUnlinkCtegory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkCtegory;

    return Output;
}

int QCoffeeClientPlugin::getIdCategory(QCoffeeCategoryInfo &category)
{
    int Output = -1;

    QString textQuery = "select id_drinkСategory from tbl_drinkСategory "
                        "where name_drinkСategory = '" + category.name +  "';";

    bool ok = false;
    QSqlQuery *queryCategoryId = execQuery(textQuery,&ok);

    if (ok) {
        if (queryCategoryId->next())
            Output = queryCategoryId->value(0).toInt();

    } else {
        qDebug()<<"Error get category id:"<<queryCategoryId->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryCategoryId;

    return Output;
}

bool QCoffeeClientPlugin::editCategory(QCoffeeCategoryInfo &category)
{
    bool Output = false;

    unlinkCategoryAndPointSale(category.id);

    QString textQuery = "update tbl_drinkСategory set "
                        "name_drinkСategory = '" + category.name + "',"
                        "description_drinkСategory = '" + category.description + "',"
                        "red_drinkСategory = '" + QString::number(category.color.red()) + "',"
                        "green_drinkСategory = '" + QString::number(category.color.green()) + "',"
                        "blue_drinkСategory = '" + QString::number(category.color.blue()) + "' "
                        "where id_drinkСategory = '" + QString::number(category.id) + "';";

    QSqlQuery *queryUpdateCategory = execQuery(textQuery,&Output);

    if (Output){
        for (int i=0;i<category.idPointSale.length();i++)
            linkCategoryAndPointSale(category.id,category.idPointSale.at(i));

    } else {
        qDebug()<<"Error update category:"<<queryUpdateCategory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUpdateCategory;

    return Output;
}

int QCoffeeClientPlugin::addCategory(QCoffeeCategoryInfo &category)
{
    int Output = -1;

    QString textQuery = "insert into tbl_drinkСategory ("
                        "id_drinkСategory,"
                        "name_drinkСategory,"
                        "description_drinkСategory,"
                        "red_drinkСategory,"
                        "green_drinkСategory,"
                        "blue_drinkСategory"
                        ") values ("
                        "'" + QString::number(category.id) + "',"
                        "'" + category.name + "',"
                        "'" + category.description + "',"
                        "'" + QString::number(category.color.red()) + "',"
                        "'" + QString::number(category.color.green()) + "',"
                        "'" + QString::number(category.color.blue()) + "'"
                        ");";

    bool ok = false;
    QSqlQuery *queryAddCategory = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdCategory(category);
        unlinkCategoryAndPointSale(category.id);
        for (int i=0;i<category.idPointSale.length();i++)
            linkCategoryAndPointSale(Output,category.idPointSale.at(i));

    } else {
        qDebug()<<"Error add category:"<<queryAddCategory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddCategory;

    return Output;
}

//QCoffeeUserInfo

QVector<QCoffeeUserInfo> QCoffeeClientPlugin::getListUser()
{
    QVector<QCoffeeUserInfo> Output;

    QString textQuery = "select id_user,login_user,password_user,name_user,patronymic_user,surname_user "
                        "from tbl_user;";

    bool ok = false;
    QSqlQuery *queryListUserInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeUserInfo currentUser;
        while (queryListUserInfo->next()) {
            currentUser.id = queryListUserInfo->value(0).toInt();
            currentUser.login = queryListUserInfo->value(1).toString();
            currentUser.password = queryListUserInfo->value(2).toString();
            currentUser.name = queryListUserInfo->value(3).toString();
            currentUser.patronymic = queryListUserInfo->value(4).toString();
            currentUser.surname = queryListUserInfo->value(5).toString();

            Output.push_back(currentUser);
        }

    } else {
        qDebug()<<"Error get list user info:"<<queryListUserInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListUserInfo;

    return Output;
}

QCoffeeUserInfo QCoffeeClientPlugin::getUserInfo(int idUser)
{
    QCoffeeUserInfo Output;

    QString textQuery = "select id_user,login_user,password_user,name_user,patronymic_user,surname_user "
                        "from tbl_user where id_user = '" + QString::number(idUser) + "';";

    bool ok = false;
    QSqlQuery *queryUserInfo = execQuery(textQuery,&ok);
    if (ok) {
        if (queryUserInfo->next()) {
            Output.id = queryUserInfo->value(0).toInt();
            Output.login = queryUserInfo->value(1).toString();
            Output.password = queryUserInfo->value(2).toString();
            Output.name = queryUserInfo->value(3).toString();
            Output.patronymic = queryUserInfo->value(4).toString();
            Output.surname = queryUserInfo->value(5).toString();
        }
    } else {
        qDebug()<<"Error get user info:"<<queryUserInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    return Output;
}

int QCoffeeClientPlugin::getIdUser(QCoffeeUserInfo &userInfo)
{
    int Output = -1;

    QString textQuery = "select id_user from tbl_user "
                        "where login_user = '" + userInfo.login + "' "
                        "and password_user = '" + userInfo.password +"' "
                        "and name_user = '" + userInfo.name + "' "
                        "and patronymic_user = '" + userInfo.patronymic + "' "
                        "and surname_user = '" + userInfo.surname + "';";

    bool ok = false;
    QSqlQuery *queryIdUser = execQuery(textQuery,&ok);

    if (ok) {
        if (queryIdUser->next())
            Output = queryIdUser->value(0).toInt();
    } else {
        qDebug()<<"Error get id user:"<<queryIdUser->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdUser;

    return Output;
}

int QCoffeeClientPlugin::addUser(QCoffeeUserInfo &userInfo)
{
    int Output = -1;

    QString textQuery = "insert into tbl_user (id_user,"
                                              "login_user,"
                                              "password_user,"
                                              "name_user,"
                                              "patronymic_user,"
                                              "surname_user) "
                        "values ('" + QString::number(userInfo.id) + "',"
                                "'" + userInfo.login + "',"
                                "'" + userInfo.password + "',"
                                "'" + userInfo.name + "',"
                                "'" + userInfo.patronymic + "',"
                                "'" + userInfo.surname + "');";

    bool ok = false;
    QSqlQuery *queryAddUser = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdUser(userInfo);

    } else {
        qDebug()<<"Error insert user:"<<queryAddUser->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddUser;

    return Output;
}

bool QCoffeeClientPlugin::editUser(QCoffeeUserInfo &userInfo)
{
    bool Output = false;

    QString textQuery = "update tbl_user set "
                        "login_user = '" + userInfo.login + "',"
                        "password_user = '" + userInfo.password + "',"
                        "name_user = '" + userInfo.name + "',"
                        "patronymic_user = '" + userInfo.patronymic + "',"
                        "surname_user = '" + userInfo.surname + "' "
                        "where id_user = '" + QString::number(userInfo.id) + "';";

    QSqlQuery *queryEditUser = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit user:"<<queryEditUser->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditUser;

    return Output;
}

//QCoffeeTypeDrink

QVector<QCoffeeTypeDrink> QCoffeeClientPlugin::getListTypeDrink()
{
    QVector<QCoffeeTypeDrink> Output;

    QString textQuery = "select id_typeDrink,name_typeDrink,description_typeDrink from tbl_typeDrink;";

    bool ok = false;
    QSqlQuery *queryTypeDrink = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeTypeDrink currentType;
        while (queryTypeDrink->next()) {
            currentType.id = queryTypeDrink->value(0).toInt();
            currentType.name = queryTypeDrink->value(1).toString();
            currentType.description = queryTypeDrink->value(2).toString();

            Output.push_back(currentType);
        }

    } else {
        qDebug()<<"Error get list type drink:"<<queryTypeDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryTypeDrink;

    return Output;
}

QCoffeeTypeDrink QCoffeeClientPlugin::getTypeDrinkInfo (int idTypeDrink)
{
    QCoffeeTypeDrink Output;

    QString textQuery = "select id_typeDrink,name_typeDrink,description_typeDrink from tbl_typeDrink "
                        "where id_typeDrink = '" + QString::number(idTypeDrink) + "';";

    bool ok = false;
    QSqlQuery *queryTypeDrink = execQuery(textQuery,&ok);

    if (ok) {
        if (queryTypeDrink->next()) {
            Output.id = queryTypeDrink->value(0).toInt();
            Output.name = queryTypeDrink->value(1).toString();
            Output.description = queryTypeDrink->value(2).toString();
        }

    } else {
        qDebug()<<"Error get type drink:"<<queryTypeDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryTypeDrink;

    return Output;
}

int QCoffeeClientPlugin::getIdTypeDrink (QCoffeeTypeDrink &typeDrink)
{
    int Output = -1;

    QString textQuery = "select id_typeDrink from tbl_typeDrink "
                        "where name_typeDrink = '" + typeDrink.name + "' "
                        "and description_typeDrink = '" + typeDrink.description + "';";

    bool ok = false;
    QSqlQuery *queryIdTypeDrink = execQuery(textQuery,&ok);

    if (ok) {
        if (queryIdTypeDrink->next())
            Output = queryIdTypeDrink->value(0).toInt();

    } else {
        qDebug()<<"Error get id drink:"<<queryIdTypeDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdTypeDrink;

    return Output;
}

int QCoffeeClientPlugin::addTypeDrink (QCoffeeTypeDrink &typeDrink)
{
    int Output = -1;

    QString textQuery = "insert into tbl_typeDrink (id_typeDrink,"
                                                   "name_typeDrink,"
                                                   "description_typeDrink) "
                        "values ('" + QString::number(typeDrink.id) + "',"
                                "'" + typeDrink.name + "',"
                                "'" + typeDrink.description + "');";

    bool ok = false;
    QSqlQuery *queryAddTypeDrink = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdTypeDrink(typeDrink);

    } else {
        qDebug()<<"Error add type drink:"<<queryAddTypeDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddTypeDrink;

    return Output;
}

bool QCoffeeClientPlugin::editTypeDrink (QCoffeeTypeDrink &typeDrink)
{
    bool Output = false;

    QString textQuery = "update tbl_typeDrink set name_typeDrink = '" + typeDrink.name + "', "
                        "description_typeDrink = '" + typeDrink.description + "' "
                        "where id_typeDrink = '" + QString::number(typeDrink.id) + "';";

    QSqlQuery *queryEditTypeDrink = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit type drink:"<<queryEditTypeDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditTypeDrink;

    return Output;
}

//QCoffeeVolumeDrinkInfo

QVector<QCoffeeVolumeDrinkInfo> QCoffeeClientPlugin::getListVolume()
{
    QVector<QCoffeeVolumeDrinkInfo> Output;

    QString textQuery = "select id_volumeDrink,"
                        "name_volumeDrink,"
                        "volume_volumeDrink,"
                        "description_volumeDrink,"
                        "units_volumeDrink "
                        "from tbl_volumeDrink;";

    bool ok = false;
    QSqlQuery *queryListVolume = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeVolumeDrinkInfo currentVolume;

        while (queryListVolume->next()) {
            currentVolume.id = queryListVolume->value(0).toInt();
            currentVolume.name = queryListVolume->value(1).toString();
            currentVolume.volume = queryListVolume->value(2).toDouble();
            currentVolume.description = queryListVolume->value(3).toString();
            currentVolume.units = queryListVolume->value(4).toString();

            currentVolume.idDrink = getIdDrinkForVolume(currentVolume.id);

            Output.push_back(currentVolume);
        }

    } else {
        qDebug()<<"Error get list volume:"<<queryListVolume->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListVolume;


    return Output;
}

QVector<QCoffeeVolumeDrinkInfo> QCoffeeClientPlugin::getListVolume(int idDrink)
{
    QVector<QCoffeeVolumeDrinkInfo> Output;
    QVector<int> idVolume = getIdVolumeForDrink(idDrink);

    for (int i=0;i<idVolume.length();i++)
        Output.push_back(getVolumeInfo(idVolume.at(i)));


    return Output;
}

QCoffeeVolumeDrinkInfo QCoffeeClientPlugin::getVolumeInfo(int idVolume)
{
    QCoffeeVolumeDrinkInfo Output;

    QString textQuery = "select id_volumeDrink,"
                        "name_volumeDrink,"
                        "volume_volumeDrink,"
                        "description_volumeDrink,"
                        "units_volumeDrink "
                        "from tbl_volumeDrink where id_volumeDrink = '" + QString::number(idVolume) + "';";

    bool ok = false;
    QSqlQuery *queryListVolume = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeVolumeDrinkInfo currentVolume;

        if (queryListVolume->next()) {
            Output.id = queryListVolume->value(0).toInt();
            Output.name = queryListVolume->value(1).toString();
            Output.volume = queryListVolume->value(2).toDouble();
            Output.description = queryListVolume->value(3).toString();
            Output.units = queryListVolume->value(4).toString();

            Output.idDrink = getIdDrinkForVolume(currentVolume.id);
        }

    } else {
        qDebug()<<"Error get list volume:"<<queryListVolume->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListVolume;

    return Output;
}

int QCoffeeClientPlugin::getVolumeId(QCoffeeVolumeDrinkInfo &volume)
{
    int Output = -1;

    QString textQury = "select id_volumeDrink from tbl_volumeDrink where "
                       "name_volumeDrink = '" + volume.name + "' "
                       "and volume_volumeDrink = '" + QString::number(volume.volume) + "' "
                       "and description_volumeDrink = '" + volume.description + "' "
                       "and units_volumeDrink = '" + volume.units + "';";

    bool ok = false;
    QSqlQuery *queryIdVolume = execQuery(textQury,&ok);

    if (ok) {
        if (queryIdVolume->next())
            Output = queryIdVolume->value(0).toInt();

    } else {
        qDebug()<<"Error get id volume:"<<queryIdVolume->lastError().text();
        qDebug()<<"textQury = "<<textQury;
    }

    delete queryIdVolume;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdDrinkForVolume(int idVolume)
{
    QVector<int> Output;

    QString textQuery = "select tbl_drink_id_drink from tbl_drink_volumeDrink where "
                        "tbl_volumeDrink_id_volumeDrink = '" + QString::number(idVolume) + "';";

    bool ok = false;
    QSqlQuery *queryIdDrink = execQuery(textQuery,&ok);

    if (ok) {
        while (queryIdDrink->next())
            Output.push_back(queryIdDrink->value(0).toInt());

    } else {
        qDebug()<<"Error get id drink for volume:"<<queryIdDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdDrink;

    return Output;
}

bool QCoffeeClientPlugin::linkVolumeAndDrink(int idVolume, int idDrink)
{
    bool Output = false;

    QString textQuery = "insert into tbl_drink_volumeDrink (tbl_drink_id_drink,tbl_volumeDrink_id_volumeDrink) "
                        "values ('" + QString::number(idDrink) + "','" + QString::number(idVolume) + "');";

    QSqlQuery *queryLinkColumeAndDrink = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"1488";
        qDebug()<<"Error link volume and drink: "<<queryLinkColumeAndDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryLinkColumeAndDrink;

    return Output;
}

bool QCoffeeClientPlugin::unlinkVolumeAndDrink(int idVolume)
{
    bool Output = false;

    QString textQuery = "delete from tbl_drink_volumeDrink "
                        "where tbl_volumeDrink_id_volumeDrink = '" + QString::number(idVolume) + "';";

    QSqlQuery *queryUnlinkVolumeAndDrink = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink volume and drink:"<<queryUnlinkVolumeAndDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkVolumeAndDrink;

    return Output;
}

bool QCoffeeClientPlugin::unlinkVolumeAndDrink2(int idVolume,int idDrink)
{
    bool Output = false;

    QString textQuery = "delete from tbl_drink_volumeDrink "
                        "where tbl_volumeDrink_id_volumeDrink = '" + QString::number(idVolume) + "' and tbl_drink_id_drink = '"+QString::number(idDrink)+ "';";

    QSqlQuery *queryUnlinkVolumeAndDrink = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink volume and drink:"<<queryUnlinkVolumeAndDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkVolumeAndDrink;

    return Output;
}

int QCoffeeClientPlugin::addVolumeDrink(QCoffeeVolumeDrinkInfo &volume)
{
    int Output = -1;

    QString textQuery = "insert into tbl_volumeDrink (id_volumeDrink,"
                                                    "name_volumeDrink,"
                                                    "volume_volumeDrink,"
                                                    "description_volumeDrink,"
                                                    "units_volumeDrink) "
                        "values ('" + QString::number(volume.id) + "',"
                                "'" + volume.name + "',"
                                "'" + QString::number(volume.volume) + "',"
                                "'" + volume.description +"',"
                                "'" + volume.units + "');";

    bool ok = false;
    QSqlQuery *queryAddVolume = execQuery(textQuery,&ok);

    if (ok) {
        Output = getVolumeId(volume);
        unlinkVolumeAndDrink(Output);
        for (int i=0;i<volume.idDrink.length();i++)
            linkVolumeAndDrink(Output,volume.idDrink.at(i));

    } else {
        qDebug()<<"Error add volume:"<<queryAddVolume->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddVolume;

    return Output;
}

bool QCoffeeClientPlugin::editVolume(QCoffeeVolumeDrinkInfo &volume)
{
    bool Output = false;

    QString textQUery = "update tbl_volumeDrink set "
                        "name_volumeDrink = '" + volume.name + "',"
                        "volume_volumeDrink = '" + QString::number(volume.volume) + "',"
                        "description_volumeDrink = '" + volume.description + "',"
                        "units_volumeDrink = '" + volume.units + "' "
                        "where id_volumeDrink = '" + QString::number(volume.id) + "';";

    QSqlQuery *queryEditVolume = execQuery(textQUery,&Output);

    if (Output) {
        unlinkVolumeAndDrink(volume.id);
        for (int i=0;i<volume.idDrink.length();i++)
            linkVolumeAndDrink(volume.id,volume.idDrink.at(i));

    } else {
        qDebug()<<"Error edit volume:"<<queryEditVolume->lastError().text();
        qDebug()<<"textQUery = "<<textQUery;
    }

    delete queryEditVolume;

    return Output;
}

//QCoffeeDrinkInfo

QVector<QCoffeeDrinkInfo> QCoffeeClientPlugin::getListDrink()
{
    QVector<QCoffeeDrinkInfo> Output;

    QString textQUery = "select id_drink,"
                        "name_drink,"
                        "description_drink,"
                        "red_drink,"
                        "green_drink,"
                        "blue_drink,"
                        "drink_id_pictureDrink,"
                        "hash_drink "
                        "from tbl_drink;";

    bool ok = false;
    QSqlQuery *queryListDrinks = execQuery(textQUery,&ok);

    if (ok) {
        QCoffeeDrinkInfo currentDrink;
        while (queryListDrinks->next()) {
            currentDrink.id = queryListDrinks->value(0).toInt();
            currentDrink.name = queryListDrinks->value(1).toString();
            currentDrink.description = queryListDrinks->value(2).toString();

            currentDrink.color.setRed(queryListDrinks->value(3).toInt());
            currentDrink.color.setGreen(queryListDrinks->value(4).toInt());
            currentDrink.color.setBlue(queryListDrinks->value(5).toInt());

            currentDrink.idPicture = queryListDrinks->value(6).toInt();
            currentDrink.pictureHash = queryListDrinks->value(7).toString();

            currentDrink.idCategories = getIdCategoriesForDrink(currentDrink.id);

            Output.push_back(currentDrink);
        }

    } else {
        qDebug()<<"Error get list drink:"<<queryListDrinks->lastError().text();
        qDebug()<<"textQUery = "<<textQUery;
    }

    delete queryListDrinks;

    return Output;
}

QVector<QCoffeeDrinkInfo> QCoffeeClientPlugin::getListDrink(int idCategory)
{
    QVector<QCoffeeDrinkInfo> Output;

    QVector<int> idDrinks;
    idDrinks = getIdDrinkForCategory(idCategory);

    for (int i=0;i<idDrinks.length();i++)
        Output.push_back(getDrinkInfo(idDrinks.at(i)));

    return Output;
}

QVector<int> QCoffeeClientPlugin::getListIdDrink()
{
    QVector<int> Output;

    QString textQUery = "select id_drink from tbl_drink;";

    bool ok = false;
    QSqlQuery *queryListDrinks = execQuery(textQUery,&ok);

    if (ok) {
        while (queryListDrinks->next()) {
            Output.push_back(queryListDrinks->value(0).toInt());
        }

    } else {
        qDebug()<<"Error get list id drink:"<<queryListDrinks->lastError().text();
        qDebug()<<"textQUery = "<<textQUery;
    }

    delete queryListDrinks;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdDrinkForCategory(int idCategory)
{
    QVector<int> Output;


    QString textQuery = "select tbl_drink_id_drink from tbl_drink_foundation where "
                        "tbl_foundation_id_foundation = '" + QString::number(idCategory) + "';";

    bool ok;
    QSqlQuery *queryIdDrink = execQuery(textQuery,&ok);

    if (ok) {
        int currentId;
        while (queryIdDrink->next()) {
            currentId = queryIdDrink->value(0).toInt();
            Output.push_back(currentId);
        }
    } else {
        qDebug()<<"Error get id drink for category:"<<queryIdDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdDrink;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdVolumeForDrink(int idDrink)
{
    QVector<int> Output;

    QString textQuery = "select tbl_volumeDrink_id_volumeDrink from tbl_drink_volumeDrink "
                        "where tbl_drink_id_drink = '" + QString::number(idDrink) + "';";

    bool ok = false;
    QSqlQuery *queryIdVolumeForDrink = execQuery(textQuery,&ok);

    if (ok) {
        while (queryIdVolumeForDrink->next()) {
            Output.push_back(queryIdVolumeForDrink->value(0).toInt());
        }

    } else {
        qDebug()<<"Error get id volume for drink:"<<queryIdVolumeForDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdVolumeForDrink;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdCategoriesForDrink(int idDrink)
{
    QVector<int> Output;

    QString textQuery = "select tbl_foundation_id_foundation from tbl_drink_foundation "
                        "where tbl_drink_id_drink = '" + QString::number(idDrink) + "';";

    bool ok = false;
    QSqlQuery *queryIdCategoryForDrink = execQuery(textQuery,&ok);

    if (ok) {
        while (queryIdCategoryForDrink->next())
            Output.push_back(queryIdCategoryForDrink->value(0).toInt());

    } else {
        qDebug()<<"Error get id category for drink:"<<queryIdCategoryForDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdCategoryForDrink;

    return Output;
}

QCoffeeDrinkInfo QCoffeeClientPlugin::getDrinkInfo(int idDrink)
{
    QCoffeeDrinkInfo Output;

    QString textQuery = "select id_drink,"
                        "name_drink,"
                        "description_drink,"
                        "red_drink,"
                        "green_drink,"
                        "blue_drink,"
                        "drink_id_pictureDrink,"
                        "hash_drink "
                        "from tbl_drink where id_drink = '" + QString::number(idDrink) + "';";

    bool ok = false;
    QSqlQuery *queryDrinkInfo = execQuery(textQuery,&ok);

    if (ok) {
        if (queryDrinkInfo->next()) {
            Output.id = queryDrinkInfo->value(0).toInt();
            Output.name = queryDrinkInfo->value(1).toString();
            Output.description = queryDrinkInfo->value(2).toString();

            Output.color.setRed(queryDrinkInfo->value(3).toInt());
            Output.color.setGreen(queryDrinkInfo->value(4).toInt());
            Output.color.setBlue(queryDrinkInfo->value(5).toInt());

            Output.idPicture = queryDrinkInfo->value(6).toInt();
            Output.pictureHash = queryDrinkInfo->value(7).toString();

            Output.idCategories = getIdCategoriesForDrink(Output.id);
        }

    } else {
        qDebug()<<"Error get drink info:"<<queryDrinkInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryDrinkInfo;

    return Output;
}

int QCoffeeClientPlugin::addDrinkInfo(QCoffeeDrinkInfo &drinkInfo)
{
    int Output = -1;

    QString textQuery = "insert into tbl_drink (id_drink,"
                                               "name_drink,"
                                               "description_drink,"
                                               "red_drink,"
                                               "green_drink,"
                                               "blue_drink,"
                                               "drink_id_pictureDrink,"
                                               "hash_drink) "
                        "values ('" + QString::number(drinkInfo.id) + "',"
                                "'" + drinkInfo.name + "',"
                                "'" + drinkInfo.description + "',"
                                "'" + QString::number(drinkInfo.color.red()) + "',"
                                "'" + QString::number(drinkInfo.color.green()) + "',"
                                "'" + QString::number(drinkInfo.color.blue()) + "',"
                                "'" + QString::number(drinkInfo.idPicture) + "',"
                                "'" + drinkInfo.pictureHash + "');";

    bool ok = false;
    QSqlQuery *queryDrinkInfo = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdDrink(drinkInfo);
        unlinkDrinkAndCategory(Output);
        for (int i=0;i<drinkInfo.idCategories.length();i++)
            linkDrinkAndCategory(Output,drinkInfo.idCategories.at(i));

    } else {
        qDebug()<<"Error get drink info:"<<queryDrinkInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryDrinkInfo;

    return Output;
}

int QCoffeeClientPlugin::getIdDrink (QCoffeeDrinkInfo &drinkInfo)
{
    int Output = -1;

    QString textQuery = "select id_drink from tbl_drink where "
                        "name_drink = '" + drinkInfo.name + "' "
                        "and description_drink = '" + drinkInfo.description + "' "
                        "and red_drink = '" + QString::number(drinkInfo.color.red()) + "' "
                        "and green_drink = '" + QString::number(drinkInfo.color.green()) + "' "
                        "and blue_drink = '" + QString::number(drinkInfo.color.blue()) + "' "
                        "and drink_id_pictureDrink = '" + QString::number(drinkInfo.idPicture) + "' "
                        "and hash_drink = '" + drinkInfo.pictureHash + "';";

    bool ok = false;
    QSqlQuery *queryIdDrink = execQuery(textQuery,&ok);

    if (ok) {
        if (queryIdDrink->next())
            Output = queryIdDrink->value(0).toInt();

    } else {
        qDebug()<<"Error get id drink:"<<queryIdDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdDrink;

    return Output;
}

bool QCoffeeClientPlugin::editDrinkInfo(QCoffeeDrinkInfo &drinkInfo)
{
    bool Output = false;

    QString textQuery = "update tbl_drink set "
                        "name_drink = '" + drinkInfo.name + "', "
                        "description_drink = '" + drinkInfo.description + "', "
                        "red_drink = '" + QString::number(drinkInfo.color.red()) + "', "
                        "green_drink = '" + QString::number(drinkInfo.color.green()) + "', "
                        "blue_drink = '" + QString::number(drinkInfo.color.blue()) + "', "
                        "drink_id_pictureDrink = '" + QString::number(drinkInfo.idPicture) + "', "
                        "hash_drink = '" + drinkInfo.pictureHash + "' "
                        "where id_drink = '" + QString::number(drinkInfo.id) + "';";

    QSqlQuery *queryEditDrink = execQuery(textQuery,&Output);
    qDebug()<< Output;
    if (Output) {
        unlinkDrinkAndCategory(drinkInfo.id);

        for (int i=0;i<drinkInfo.idCategories.length();i++)
            linkDrinkAndCategory(drinkInfo.id,drinkInfo.idCategories.at(i));

    } else {
        qDebug()<<"Error edit drink:"<<queryEditDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditDrink;

    return Output;
}

bool QCoffeeClientPlugin::deleteDrinkInfo(QCoffeeDrinkInfo &drinkInfo) {
    bool Output = false;

    QString textQuery = "delete from tbl_drink where id_drink = '" + QString::number(drinkInfo.id) + "';";

    QSqlQuery *queryDeleteDrink = execQuery(textQuery,&Output);

    if (Output) {

    } else {
        qDebug()<<"Error edit drink:"<<queryDeleteDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryDeleteDrink;

    return Output;
}


bool QCoffeeClientPlugin::deleteCategoryInfo(QCoffeeCategoryInfo &categoryInfo) {
    bool Output = false;

    QString textQuery = "delete from tbl_drinkСategory where id_drinkСategory = '" + QString::number(categoryInfo.id) + "';";

    unlinkCategoryAndDrink(categoryInfo.id);
    unlinkCategoryAndPointSale(categoryInfo.id);

    QSqlQuery *queryDeleteDrink = execQuery(textQuery,&Output);

    if (Output) {

    } else {
        qDebug()<<"Error edit drink:"<<queryDeleteDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryDeleteDrink;

    return Output;
}

bool QCoffeeClientPlugin::unlinkDrinkAndCategory(int idDrink)
{
    bool Output = false;

    QString textQuery = "delete from tbl_drink_foundation "
                        "where tbl_drink_id_drink = '" + QString::number(idDrink) + "';";

    QSqlQuery *queryUnlinkDrinkAndCategory = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink drink and category:"<<queryUnlinkDrinkAndCategory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkDrinkAndCategory;

    return Output;
}

bool QCoffeeClientPlugin::linkDrinkAndCategory(int idDrink,int idCategory)
{
    bool Output = false;

    QString textQuery = "insert into tbl_drink_foundation (tbl_drink_id_drink,tbl_foundation_id_foundation) "
                        "values('" + QString::number(idDrink) + "','" + QString::number(idCategory) + "');";

    QSqlQuery *queryLinkDrinkAndCategory = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error link drink and category:"<<queryLinkDrinkAndCategory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryLinkDrinkAndCategory;

    return Output;
}


bool QCoffeeClientPlugin::unlinkCategoryAndDrink(int idCategory)
{
    bool Output = false;

    QString textQuery = "delete from tbl_drink_foundation "
                        "where tbl_foundation_id_foundation = '" + QString::number(idCategory) + "';";

    QSqlQuery *queryUnlinkDrinkAndCategory = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink drink and category:"<<queryUnlinkDrinkAndCategory->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkDrinkAndCategory;

    return Output;
}


//QCoffeePriceInfo

QVector<QCoffeePriceInfo> QCoffeeClientPlugin::getListPriceInfo()
{
    QVector<QCoffeePriceInfo> Output;

    QString textQuery = "select id_price,value_price,startTime_price,description_price,"
                        "price_id_drink,price_id_volumeDrink,price_id_drinkСategory "
                        "from tbl_price;";

    bool ok = false;
    QSqlQuery *queryListPrice = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeePriceInfo currentPriceInfo;
        while (queryListPrice->next()) {
            currentPriceInfo.id = queryListPrice->value(0).toInt();
            currentPriceInfo.value = queryListPrice->value(1).toDouble();
            currentPriceInfo.startDate = queryListPrice->value(2).toDateTime();
            currentPriceInfo.description = queryListPrice->value(3).toString();
            currentPriceInfo.idDrink = queryListPrice->value(4).toInt();
            currentPriceInfo.idVolume = queryListPrice->value(5).toInt();
            currentPriceInfo.idDrinkCategory = queryListPrice->value(6).toInt();

            currentPriceInfo.idPointSale = getIdPointSaleForPriceInfo(currentPriceInfo.id);

            Output.push_back(currentPriceInfo);
        }

    } else {
        qDebug()<<"Error get list price info:"<<queryListPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListPrice;

    return Output;
}

QCoffeePriceInfo QCoffeeClientPlugin::getPriceInfo(int idPriceInfo)
{
    QCoffeePriceInfo Output;

    QString textQuery = "select id_price,value_price,startTime_price,description_price,"
                        "price_id_drink,price_id_volumeDrink,price_id_drinkСategory "
                        "from tbl_price where id_price = '" + QString::number(idPriceInfo) + "';";

    bool ok = false;
    QSqlQuery *queryPriceInfo = execQuery(textQuery,&ok);

    if (ok) {
        if (queryPriceInfo->next()) {
            Output.id = queryPriceInfo->value(0).toInt();
            Output.value = queryPriceInfo->value(1).toDouble();
            Output.startDate = queryPriceInfo->value(2).toDateTime();
            Output.description = queryPriceInfo->value(3).toString();
            Output.idDrink = queryPriceInfo->value(4).toInt();
            Output.idVolume = queryPriceInfo->value(5).toInt();
            Output.idDrinkCategory = queryPriceInfo->value(6).toInt();

            Output.idPointSale = getIdPointSaleForPriceInfo(Output.id);
        }

    } else {
        qDebug()<<"Error get price info:"<<queryPriceInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPriceInfo;

    return Output;
}

QCoffeePriceInfo QCoffeeClientPlugin::getPricceInfo(int idCategory,int idDrink,int idVolume)
{
    QCoffeePriceInfo Output;

    QString textQuery = "select id_price,value_price,startTime_price,description_price,"
                        "price_id_drink,price_id_volumeDrink,price_id_drinkСategory "
                        "from tbl_price where "
                        "price_id_drinkСategory = '" + QString::number(idCategory) + "' "
                        "and price_id_drink = '" + QString::number(idDrink) + "' "
                        "and price_id_volumeDrink = '" + QString::number(idVolume) + "' "
                        "and startTime_price <= '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "' "
                        "order by startTime_price desc;";

    bool ok = false;
    QSqlQuery *queryPriceInfo = execQuery(textQuery,&ok);

    if (ok) {
        if (queryPriceInfo->next()) {
            Output.id = queryPriceInfo->value(0).toInt();
            Output.value = queryPriceInfo->value(1).toDouble();
            Output.startDate = queryPriceInfo->value(2).toDateTime();
            Output.description = queryPriceInfo->value(3).toString();
            Output.idDrink = queryPriceInfo->value(4).toInt();
            Output.idVolume = queryPriceInfo->value(5).toInt();
            Output.idDrinkCategory = queryPriceInfo->value(6).toInt();

            Output.idPointSale = getIdPointSaleForPriceInfo(Output.id);
        }

    } else {
        qDebug()<<"Error get price info:"<<queryPriceInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPriceInfo;

    return Output;
}

int QCoffeeClientPlugin::getIdPriceInfo(QCoffeePriceInfo &priceInfo)
{
    int Output = -1;

    QString textQuery = "select id_price from tbl_price where "
                        "value_price = '" + QString::number(priceInfo.value) + "' "
                        "and startTime_price = '" + priceInfo.startDate.toString("yyyy-MM-dd hh:mm:ss") + "' "
                        "and description_price = '" + priceInfo.description + "' "
                        "and price_id_drink = '" + QString::number(priceInfo.idDrink) + "' "
                        "and price_id_volumeDrink = '" + QString::number(priceInfo.idVolume) + "'"
                        "and price_id_drinkСategory = '" + QString::number(priceInfo.idDrinkCategory) + "';";

    bool ok = false;
    QSqlQuery *queryPrice = execQuery(textQuery,&ok);

    if (ok) {
        if (queryPrice->next())
            Output = queryPrice->value(0).toInt();
    } else {
        qDebug()<<"Error get id price info:"<<queryPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPrice;

    return Output;
}

QVector<int> QCoffeeClientPlugin::getIdPointSaleForPriceInfo(int idPriceinfo)
{
    QVector<int> Output;

    QString textQuery = "select tbl_pointSale_id_pointSale from tbl_pointSale_price "
                        "where tbl_price_id_price = '" + QString::number(idPriceinfo) +"';";

    bool ok = false;
    QSqlQuery *queryPointSale = execQuery(textQuery,&ok);

    if (ok) {
        while (queryPointSale->next())
            Output.push_back(queryPointSale->value(0).toInt());

    } else {
        qDebug()<<"Error get list id point sale for price:"<<queryPointSale->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPointSale;

    return Output;
}

bool QCoffeeClientPlugin::unlinkPriceInfoAndPointSale(int idPriceInfo)
{
    bool Output = false;

    QString textQuery = "delete from tbl_pointSale_price "
                        "where tbl_price_id_price = '" + QString::number(idPriceInfo) + "';";

    QSqlQuery *queryUnlinkPointSaleAndPrice = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error unlink point sale and price info:"<<queryUnlinkPointSaleAndPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryUnlinkPointSaleAndPrice;

    return Output;
}

bool QCoffeeClientPlugin::linkPriceInfoAndPointSale(int idPriceInfo, int idPointSale)
{
    bool Output = false;

    QString textQuery = "insert into tbl_pointSale_price (tbl_pointSale_id_pointSale,tbl_price_id_price) "
                        "values ('" + QString::number(idPointSale) + "','" + QString::number(idPriceInfo) + "');";

    QSqlQuery *queryLinkPointSaleAndPrice = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error link price info and point sale:"<<queryLinkPointSaleAndPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryLinkPointSaleAndPrice;

    return Output;
}

int QCoffeeClientPlugin::addPriceInfo(QCoffeePriceInfo &priceInfo)
{
    int Output = -1;

    QString textQuery = "insert into tbl_price (id_price,"
                        "value_price,"
                        "startTime_price,"
                        "description_price,"
                        "price_id_drink,"
                        "price_id_volumeDrink,"
                        "price_id_drinkСategory) "
                        "values ('" + QString::number(priceInfo.id) + "',"
                        "'" + QString::number(priceInfo.value) + "',"
                        "'" + priceInfo.startDate.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "'" + priceInfo.description + "',"
                        "'" + QString::number(priceInfo.idDrink) + "',"
                        "'" + QString::number(priceInfo.idVolume) + "',"
                        "'" + QString::number(priceInfo.idDrinkCategory) + "');";

    bool ok = false;
    QSqlQuery *queryAddPrice = execQuery(textQuery,&ok);
    if (ok) {
        Output = getIdPriceInfo(priceInfo);
        unlinkPriceInfoAndPointSale(Output);
        for (int i=0;i<priceInfo.idPointSale.length();i++)
            linkPriceInfoAndPointSale(Output,priceInfo.idPointSale.at(i));

    } else {
        qDebug()<<"Error add price:"<<queryAddPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddPrice;

    return Output;
}

bool QCoffeeClientPlugin::editPriceInfo(QCoffeePriceInfo &priceInfo)
{
    bool Output = false;

    QString textQuery = "update tbl_price set "
                        "value_price = '" + QString::number(priceInfo.value) + "',"
                        "startTime_price = '" + priceInfo.startDate.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "description_price = '" + priceInfo.description + "',"
                        "price_id_drink = '" + QString::number(priceInfo.idDrink) + "',"
                        "price_id_volumeDrink = '" + QString::number(priceInfo.idVolume) + "',"
                        "price_id_drinkСategory = '" + QString::number(priceInfo.idDrinkCategory) + "' "
                        "where id_price = '" + QString::number(priceInfo.id) + "';";

    QSqlQuery *queryEditPrice = execQuery(textQuery,&Output);

    if (Output) {

        unlinkPriceInfoAndPointSale(priceInfo.id);
       for (int i=0;i<priceInfo.idPointSale.length();i++)
           linkPriceInfoAndPointSale(priceInfo.id,priceInfo.idPointSale.at(i));

    } else {
        qDebug()<<"Error edit price info:"<<queryEditPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditPrice;

    return Output;
}

//QCoffeeReceiptInfo

QVector<QCoffeeReceiptInfo> QCoffeeClientPlugin::getListReceiptInfo()
{
    QVector<QCoffeeReceiptInfo> Output;

    QString textQuery = "select id_receipt,"
                        "date_receipt,"
                        "sum_receipt,"
                        "receipt_id_pointSale,"
                        "receipt_id_shift "
                        "from tbl_receipt;";

    bool ok = false;
    QSqlQuery *queryListReceiptInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeReceiptInfo currentReceiptInfo;
        while (queryListReceiptInfo->next()) {
            currentReceiptInfo.id = queryListReceiptInfo->value(0).toInt();
            currentReceiptInfo.dateTime = queryListReceiptInfo->value(1).toDateTime();
            currentReceiptInfo.sum = queryListReceiptInfo->value(2).toDouble();
            currentReceiptInfo.idPointSale = queryListReceiptInfo->value(3).toInt();
            currentReceiptInfo.idShift = queryListReceiptInfo->value(4).toInt();

            Output.push_back(currentReceiptInfo);
        }

    } else {
        qDebug()<<"Error get list Receipt InfoL"<<queryListReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListReceiptInfo;

    return Output;
}

QVector<QCoffeeReceiptInfo> QCoffeeClientPlugin::getListReceiptInfoForShift(int idShift)
{
    QVector<QCoffeeReceiptInfo> Output;

    QString textQuery = "select id_receipt,"
                        "date_receipt,"
                        "sum_receipt,"
                        "receipt_id_pointSale,"
                        "receipt_id_shift "
                        "from tbl_receipt "
                        "where receipt_id_shift = '" + QString::number(idShift) + "';";

    bool ok = false;
    QSqlQuery *queryListReceiptInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeReceiptInfo currentReceiptInfo;
        while (queryListReceiptInfo->next()) {
            currentReceiptInfo.id = queryListReceiptInfo->value(0).toInt();
            currentReceiptInfo.dateTime = queryListReceiptInfo->value(1).toDateTime();
            currentReceiptInfo.sum = queryListReceiptInfo->value(2).toDouble();
            currentReceiptInfo.idPointSale = queryListReceiptInfo->value(3).toInt();
            currentReceiptInfo.idShift = queryListReceiptInfo->value(4).toInt();

            Output.push_back(currentReceiptInfo);
        }

    } else {
        qDebug()<<"Error get list Receipt InfoL"<<queryListReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListReceiptInfo;

    return Output;
}

QVector<QCoffeeReceiptInfo> QCoffeeClientPlugin::getListReceiptInfoForPointSale(int idPointSale)
{
    QVector<QCoffeeReceiptInfo> Output;

    QString textQuery = "select id_receipt,"
                        "date_receipt,"
                        "sum_receipt,"
                        "receipt_id_pointSale,"
                        "receipt_id_shift "
                        "from tbl_receipt "
                        "where receipt_id_pointSale = '" + QString::number(idPointSale) + "';";

    bool ok = false;
    QSqlQuery *queryListReceiptInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeReceiptInfo currentReceiptInfo;
        while (queryListReceiptInfo->next()) {
            currentReceiptInfo.id = queryListReceiptInfo->value(0).toInt();
            currentReceiptInfo.dateTime = queryListReceiptInfo->value(1).toDateTime();
            currentReceiptInfo.sum = queryListReceiptInfo->value(2).toDouble();
            currentReceiptInfo.idPointSale = queryListReceiptInfo->value(3).toInt();
            currentReceiptInfo.idShift = queryListReceiptInfo->value(4).toInt();

            Output.push_back(currentReceiptInfo);
        }

    } else {
        qDebug()<<"Error get list Receipt InfoL"<<queryListReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListReceiptInfo;

    return Output;
}

QCoffeeReceiptInfo QCoffeeClientPlugin::getReceiptInfo(int idReceiptInfo)
{
    QCoffeeReceiptInfo Output;

    QString textQuery = "select id_receipt,date_receipt,sum_receipt,sum_receipt from tbl_receipt,receipt_id_shift "
                        "where id_receipt = '" + QString::number(idReceiptInfo) + "';";

    bool ok = false;
    QSqlQuery *queryReceiptInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeReceiptInfo currentReceiptInfo;
        if (queryReceiptInfo->next()) {
            Output.id = queryReceiptInfo->value(0).toInt();
            Output.dateTime = queryReceiptInfo->value(1).toDateTime();
            Output.sum = queryReceiptInfo->value(2).toDouble();
            Output.idPointSale = queryReceiptInfo->value(3).toInt();
            Output.idShift = queryReceiptInfo->value(4).toInt();

        }

    } else {
        qDebug()<<"Error get Receipt Info:"<<queryReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryReceiptInfo;

    return Output;
}

int QCoffeeClientPlugin::getIdReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
{
    int Output = -1;

    QString textQuery = "select id_receipt from tbl_receipt where "
                        "date_receipt = '" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "' "
                        "and sum_receipt = '" + QString::number(receiptInfo.sum) + "' "
                        "and receipt_id_pointSale = '" + QString::number(receiptInfo.idPointSale) + "' "
                        "and receipt_id_shift = '" + QString::number(receiptInfo.idShift) + "';";

    bool ok = false;
    QSqlQuery *queryIdReceiptInfo = execQuery(textQuery,&ok);

    if (ok) {
        if (queryIdReceiptInfo->next())
            Output = queryIdReceiptInfo->value(0).toInt();

    } else {
        qDebug()<<"Error get id receipt info:"<<queryIdReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdReceiptInfo;

    return Output;
}

int QCoffeeClientPlugin::addReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
{
    int Output = -1;

    QString textQuery;

    if (receiptInfo.id == -1) {

        textQuery = "insert into tbl_receipt ("
                    "id_receipt,"
                    "date_receipt,"
                    "sum_receipt,"
                    "receipt_id_pointSale,"
                    "receipt_id_shift"
                    ") values ("
                    "'" + QString::number(getNextId("tbl_receipt","id_receipt")) +"',"
                    "'" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                    "'" + QString::number(receiptInfo.sum) + "',"
                    "'" + QString::number(receiptInfo.idPointSale) + "',"
                    "'" + QString::number(receiptInfo.idShift) + "');";
    } else {
        textQuery = "insert into tbl_receipt (id_receipt,date_receipt,sum_receipt,receipt_id_pointSale,receipt_id_shift) "
                    "values ('" + QString::number(receiptInfo.id) + "',"
                    "'" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                    "'" + QString::number(receiptInfo.sum) + "',"
                    "'" + QString::number(receiptInfo.idPointSale) + "',"
                    "'" + QString::number(receiptInfo.idShift) + "');";
    }


    bool ok = false;
    QSqlQuery *queryAddReceiptInfo = execQuery(textQuery,&ok);
    if (ok) {
        Output = getIdReceiptInfo(receiptInfo);

    } else {
        qDebug()<<"Error add receipt info:"<<queryAddReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddReceiptInfo;

    return Output;
}

bool QCoffeeClientPlugin::editReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
{
    bool Output = false;

    int currentId = getIdReceiptInfo(receiptInfo);
    QString textQuery = "update tbl_receipt set "
                        "id_receipt = '" + QString::number(receiptInfo.id) + "', "
                        "date_receipt = '" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "', "
                        "sum_receipt = '" + QString::number(receiptInfo.sum) + "', "
                        "receipt_id_pointSale = '" + QString::number(receiptInfo.idPointSale) + "', "
                        "receipt_id_shift = '" + QString::number(receiptInfo.idShift) + "' "
                        "where id_receipt = '" + QString::number(currentId) + "';";

    QSqlQuery *queryEditReceiptInfo = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit receipt info:"<<queryEditReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditReceiptInfo;

    return Output;
}

//QCoffeeSoldPositionInfo

QVector<QCoffeeSoldPositionInfo> QCoffeeClientPlugin::getListSoldPosition()
{
    QVector<QCoffeeSoldPositionInfo> Output;

    QString textQuery = "select id_soldPosition,soldPosition_id_user,count_soldPosition,soldPosition_id_drink,"
                        "soldPosition_id_typeDrink,soldPosition_id_price,soldPosition_id_volumeDrink,"
                        "soldPosition_id_receipt,soldPosition_id_pointSale,soldPosition_numberInReceipt,date_soldPosition "
                        "from tbl_soldPosition;";

    bool ok = false;
    QSqlQuery *queryListSoldPosition = execQuery(textQuery,&ok);

    if (ok) {
       QCoffeeSoldPositionInfo currentSoldPosition;
       while (queryListSoldPosition->next()) {
           currentSoldPosition.id = queryListSoldPosition->value(0).toInt();
           currentSoldPosition.idUser = queryListSoldPosition->value(1).toInt();
           currentSoldPosition.count = queryListSoldPosition->value(2).toInt();
           currentSoldPosition.idDrink = queryListSoldPosition->value(3).toInt();
           currentSoldPosition.idTypeDrink = queryListSoldPosition->value(4).toInt();
           currentSoldPosition.idPrice = queryListSoldPosition->value(5).toInt();
           currentSoldPosition.idVolumeDrink = queryListSoldPosition->value(6).toInt();
           currentSoldPosition.idReceipt = queryListSoldPosition->value(7).toInt();
           currentSoldPosition.idPointSale = queryListSoldPosition->value(8).toInt();
           currentSoldPosition.numberInReceipt = queryListSoldPosition->value(9).toInt();
           currentSoldPosition.date = queryListSoldPosition->value(10).toDateTime();

           Output.push_back(currentSoldPosition);
       }

    } else {
        qDebug()<<"Error load list sold position:"<<queryListSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListSoldPosition;

    return Output;
}



QVector<std::tuple<int, QString,int, double,QDate>> QCoffeeClientPlugin::getListSoldPositionForIdAndDates(qint32 &id,QDate &firstDate,QDate &lastDate) {
    QVector<std::tuple<int, QString,int, double,QDate>> Output;

    QString textQuery = "SELECT s.soldPosition_id_drink AS drink_id, d.name_drink AS drink_name, "
        "SUM(s.count_soldPosition) AS total_quantity,SUM(s.count_soldPosition * p.value_price) AS total_sales, "
        "DATE(s.date_soldPosition) AS sale_date FROM tbl_soldPosition s JOIN "
        "tbl_price p ON s.soldPosition_id_price = p.id_price JOIN "
        "tbl_drink d ON s.soldPosition_id_drink = d.id_drink WHERE "
        "s.soldPosition_id_drink = "+QString::number(id)+" AND DATE(s.date_soldPosition) >= \'"+firstDate.toString("yyyy-MM-dd")+"\' AND DATE(s.date_soldPosition) <= \'"+lastDate.toString("yyyy-MM-dd")+"\' "
        "GROUP BY s.soldPosition_id_drink, DATE(s.date_soldPosition) ORDER BY sale_date ASC;";
    qDebug()<<"textQuery = "<<textQuery;

    bool ok = false;
    QSqlQuery *queryListSoldPosition = execQuery(textQuery,&ok);

    if (ok) {

        while (queryListSoldPosition->next()) {


            Output.push_back(std::make_tuple(queryListSoldPosition->value(0).toInt(),queryListSoldPosition->value(1).toString(),
               queryListSoldPosition->value(2).toInt(),queryListSoldPosition->value(3).toDouble(),queryListSoldPosition->value(4).toDate()));
             qDebug()<<"asd "<<queryListSoldPosition->value(1).toString();
        }

    } else {
        qDebug()<<"Error load list sold position:"<<queryListSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListSoldPosition;

    return Output;

}





QVector<QCoffeeSoldPositionInfo> QCoffeeClientPlugin::getListSoldPositionForReceipt(int idReceipt)
{
    QVector<QCoffeeSoldPositionInfo> Output;

    QString textQuery = "select id_soldPosition,soldPosition_id_user,count_soldPosition,soldPosition_id_drink,"
                        "soldPosition_id_typeDrink,soldPosition_id_price,soldPosition_id_volumeDrink,"
                        "soldPosition_id_receipt,soldPosition_id_pointSale,soldPosition_numberInReceipt,date_soldPosition "
                        "from tbl_soldPosition "
                        "where soldPosition_id_receipt = '" + QString::number(idReceipt) + "';";

    bool ok = false;
    QSqlQuery *queryListSoldPosition = execQuery(textQuery,&ok);

    if (ok) {
       QCoffeeSoldPositionInfo currentSoldPosition;
       while (queryListSoldPosition->next()) {
           currentSoldPosition.id = queryListSoldPosition->value(0).toInt();
           currentSoldPosition.idUser = queryListSoldPosition->value(1).toInt();
           currentSoldPosition.count = queryListSoldPosition->value(2).toInt();
           currentSoldPosition.idDrink = queryListSoldPosition->value(3).toInt();
           currentSoldPosition.idTypeDrink = queryListSoldPosition->value(4).toInt();
           currentSoldPosition.idPrice = queryListSoldPosition->value(5).toInt();
           currentSoldPosition.idVolumeDrink = queryListSoldPosition->value(6).toInt();
           currentSoldPosition.idReceipt = queryListSoldPosition->value(7).toInt();
           currentSoldPosition.idPointSale = queryListSoldPosition->value(8).toInt();
           currentSoldPosition.numberInReceipt = queryListSoldPosition->value(9).toInt();
           currentSoldPosition.date = queryListSoldPosition->value(10).toDateTime();

           Output.push_back(currentSoldPosition);
       }

    } else {
        qDebug()<<"Error load list sold position:"<<queryListSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListSoldPosition;

    return Output;
}

QCoffeeSoldPositionInfo QCoffeeClientPlugin::getSoldPosition(int idSoldPosition)
{
    QCoffeeSoldPositionInfo Output;

    QString textQuery = "select id_soldPosition,soldPosition_id_user,count_soldPosition,soldPosition_id_drink,"
                        "soldPosition_id_typeDrink,soldPosition_id_price,soldPosition_id_volumeDrink,"
                        "soldPosition_id_receipt,soldPosition_id_pointSale,soldPosition_numberInReceipt,date_soldPosition "
                        "from tbl_soldPosition "
                        "where id_soldPosition = '" + QString::number(idSoldPosition) + "';";

    bool ok = false;
    QSqlQuery *querySoldPosition = execQuery(textQuery,&ok);

    if (ok) {
       if (querySoldPosition->next()) {
           Output.id = querySoldPosition->value(0).toInt();
           Output.idUser = querySoldPosition->value(1).toInt();
           Output.count = querySoldPosition->value(2).toInt();
           Output.idDrink = querySoldPosition->value(3).toInt();
           Output.idTypeDrink = querySoldPosition->value(4).toInt();
           Output.idPrice = querySoldPosition->value(5).toInt();
           Output.idVolumeDrink = querySoldPosition->value(6).toInt();
           Output.idReceipt = querySoldPosition->value(7).toInt();
           Output.idPointSale = querySoldPosition->value(8).toInt();
           Output.numberInReceipt = querySoldPosition->value(9).toInt();
           Output.date = querySoldPosition->value(10).toDateTime();
       }

    } else {
        qDebug()<<"Error load sold position:"<<querySoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete querySoldPosition;

    return Output;
}

int QCoffeeClientPlugin::getIdSoldPosition (QCoffeeSoldPositionInfo &soldPosition)
{
    int Output = -1;

    QString textQuery = "select id_soldPosition from tbl_soldPosition where "
                        "soldPosition_id_user = '" + QString::number(soldPosition.idUser) + "' "
                        "and count_soldPosition = '" + QString::number(soldPosition.count) + "' "
                        "and soldPosition_id_drink = '" + QString::number(soldPosition.idDrink) + "' "
                        "and soldPosition_id_typeDrink = '" + QString::number(soldPosition.idTypeDrink) + "' "
                        "and soldPosition_id_price = '" + QString::number(soldPosition.idPrice) + "' "
                        "and soldPosition_id_volumeDrink = '" + QString::number(soldPosition.idVolumeDrink) + "' "
                        //"and soldPosition_id_receipt = '" + QString::number(soldPosition.idReceipt) + "' "
                        "and soldPosition_id_pointSale = '" + QString::number(soldPosition.idPointSale) + "' "
                        "and soldPosition_numberInReceipt = '" + QString::number(soldPosition.numberInReceipt) + "' "
                        "and date_soldPosition = '" + soldPosition.date.toString("yyyy-MM-dd hh:mm:ss") + "';";

    bool ok = false;
    QSqlQuery *queryIdSoldPosition = execQuery(textQuery,&ok);

    if (ok) {
        if (queryIdSoldPosition->next())
            Output = queryIdSoldPosition->value(0).toInt();

    } else {
        qDebug()<<"Error get id sold position:"<<queryIdSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryIdSoldPosition;

    return Output;
}

int QCoffeeClientPlugin::addSoldPosition(QCoffeeSoldPositionInfo &soldPosition)
{
    int Output = -1;

    QString textQuery;

    if (soldPosition.id == -1) {
        textQuery = "insert into tbl_soldPosition ("
                    "id_soldPosition,"
                    "soldPosition_id_user,"
                    "count_soldPosition,"
                    "soldPosition_id_drink,"
                    "soldPosition_id_typeDrink,"
                    "soldPosition_id_price,"
                    "soldPosition_id_volumeDrink,"
                    "soldPosition_id_receipt,"
                    "soldPosition_id_pointSale,"
                    "soldPosition_numberInReceipt,"
                    "date_soldPosition"
                    ") values ("
                    "'" + QString::number(getNextId("tbl_soldPosition","id_soldPosition")) + "',"
                    "'" + QString::number(soldPosition.idUser) + "',"
                    "'" + QString::number(soldPosition.count) + "',"
                    "'" + QString::number(soldPosition.idDrink) + "',"
                    "'" + QString::number(soldPosition.idTypeDrink) + "',"
                    "'" + QString::number(soldPosition.idPrice) + "',"
                    "'" + QString::number(soldPosition.idVolumeDrink) + "',"
                    "'" + QString::number(soldPosition.idReceipt) + "',"
                    "'" + QString::number(soldPosition.idPointSale) + "',"
                    "'" + QString::number(soldPosition.numberInReceipt) + "',"
                    "'" + soldPosition.date.toString("yyyy-MM-dd hh:mm:ss") + "'"
                    ");";
    } else {
        textQuery = "insert into tbl_soldPosition ("
                    "id_soldPosition,"
                    "soldPosition_id_user,"
                    "count_soldPosition,"
                    "soldPosition_id_drink,"
                    "soldPosition_id_typeDrink,"
                    "soldPosition_id_price,"
                    "soldPosition_id_volumeDrink,"
                    "soldPosition_id_receipt,"
                    "soldPosition_id_pointSale,"
                    "soldPosition_numberInReceipt,"
                    "date_soldPosition"
                    ") values ("
                    "'" + QString::number(soldPosition.id) + "',"
                    "'" + QString::number(soldPosition.idUser) + "',"
                    "'" + QString::number(soldPosition.count) + "',"
                    "'" + QString::number(soldPosition.idDrink) + "',"
                    "'" + QString::number(soldPosition.idTypeDrink) + "',"
                    "'" + QString::number(soldPosition.idPrice) + "',"
                    "'" + QString::number(soldPosition.idVolumeDrink) + "',"
                    "'" + QString::number(soldPosition.idReceipt) + "',"
                    "'" + QString::number(soldPosition.idPointSale) + "',"
                    "'" + QString::number(soldPosition.numberInReceipt) + "',"
                    "'" + soldPosition.date.toString("yyyy-MM-dd hh:mm:ss") +"'"
                    ");";
    }



    bool ok = false;
    QSqlQuery *queryAddSoldPosition = execQuery(textQuery,&ok);
    if (ok) {
        Output = getIdSoldPosition(soldPosition);

    } else {
        qDebug()<<"Error add sold position:"<<queryAddSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddSoldPosition;

    return Output;
}

bool QCoffeeClientPlugin::editSoldPosition(QCoffeeSoldPositionInfo &soldPosition)
{
    bool Output = false;

    int currentId = getIdSoldPosition(soldPosition);
    QString textQuery;

    if (currentId == soldPosition.id)
    {
        textQuery = "update tbl_soldPosition set "
                    "soldPosition_id_user = '" + QString::number(soldPosition.idUser) + "',"
                    "count_soldPosition = '" + QString::number(soldPosition.count) + "',"
                    "soldPosition_id_drink = '" + QString::number(soldPosition.idDrink) + "',"
                    "soldPosition_id_typeDrink = '" + QString::number(soldPosition.idTypeDrink) + "',"
                    "soldPosition_id_price = '" + QString::number(soldPosition.idPrice) + "',"
                    "soldPosition_id_volumeDrink = '" + QString::number(soldPosition.idVolumeDrink) + "',"
                    "soldPosition_id_receipt = '" + QString::number(soldPosition.idReceipt) + "',"
                    "soldPosition_id_pointSale = '" + QString::number(soldPosition.idPointSale) + "',"
                    "soldPosition_numberInReceipt = '" + QString::number(soldPosition.numberInReceipt) + "',"
                    "date_soldPosition = '" + soldPosition.date.toString("yyyy-MM-dd hh:mm:ss") + "' "
                    " where id_soldPosition = '" + QString::number(currentId) + "';";

        QSqlQuery *queryEditSoldPosition = execQuery(textQuery,&Output);

        if (!Output) {
            qDebug()<<"Error edit sold position:"<<queryEditSoldPosition->lastError().text();
            qDebug()<<"textQuery = "<<textQuery;
        }

        delete queryEditSoldPosition;

    } else {
        deleteSolfPosition(currentId);
        addSoldPosition(soldPosition);
    }


    return Output;
}

void QCoffeeClientPlugin::deleteSolfPosition (int idSoldPosition)
{
    QString textQuery = "delete from tbl_soldPosition where id_soldPosition = '" + QString::number(idSoldPosition) + "';";

    bool result;
    QSqlQuery *querydeleteSoldPosition = execQuery(textQuery,&result);

    if (!result) {
        qDebug()<<"Error delete sold position:"<<querydeleteSoldPosition->lastError().text();
        qDebug()<<"textQuery="<<textQuery;
    }

    delete querydeleteSoldPosition;
}


//QCoffeeShiftInfo
QVector<QCoffeeShiftInfo> QCoffeeClientPlugin::getListShift()
{
    QVector<QCoffeeShiftInfo> Output;

    QString textQuery = "select id_shift,openTime_shift,"
                        "closeTime_shift,open_shift,"
                        "close_shift,shift_id_user,"
                        "shift_id_pointSale "
                        "from tbl_shift;";

    bool ok;
    QSqlQuery *queryShift = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeShiftInfo currentShift;
        while (queryShift->next()) {
            currentShift.id = queryShift->value(0).toInt();
            currentShift.openTime = queryShift->value(1).toDateTime();
            currentShift.closeTime = queryShift->value(2).toDateTime();
            currentShift.open = queryShift->value(3).toBool();
            currentShift.close = queryShift->value(4).toBool();
            currentShift.idUser = queryShift->value(5).toInt();
            currentShift.idPointSale = queryShift->value(6).toInt();

            Output.push_back(currentShift);
        }
    } else {
        qDebug()<<"Error get shift list:"<<queryShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryShift;

    return Output;
}

QCoffeeShiftInfo QCoffeeClientPlugin::getShiftInfo(int idShift)
{
    QCoffeeShiftInfo Output;

    QString textQuery = "select id_shift,openTime_shift,"
                        "closeTime_shift,open_shift,"
                        "close_shift,shift_id_user,"
                        "shift_id_pointSale "
                        "from tbl_shift where id_shift = '" + QString::number(idShift) + "';";

    bool ok;
    QSqlQuery *queryShift = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeShiftInfo currentShift;
        if (queryShift->next()) {
            Output.id = queryShift->value(0).toInt();
            Output.openTime = queryShift->value(1).toDateTime();
            Output.closeTime = queryShift->value(2).toDateTime();
            Output.open = queryShift->value(3).toBool();
            Output.close = queryShift->value(4).toBool();
            Output.idUser = queryShift->value(5).toInt();
            Output.idPointSale = queryShift->value(6).toInt();
        }
    } else {
        qDebug()<<"Error get shift info:"<<queryShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryShift;

    return Output;
}

QCoffeeShiftInfo QCoffeeClientPlugin::getShiftInfo(int idUser, int idPointSale)
{
    QCoffeeShiftInfo Output;

    QString textQuery = "select id_shift,openTime_shift,"
                        "closeTime_shift,open_shift,"
                        "close_shift,shift_id_user,"
                        "shift_id_pointSale "
                        "from tbl_shift where shift_id_user = '" + QString::number(idUser) + "' "
                        "and shift_id_pointSale = '" + QString::number(idPointSale) + "';";

    bool ok;
    QSqlQuery *queryShift = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeShiftInfo currentShift;
        if (queryShift->next()) {
            Output.id = queryShift->value(0).toInt();
            Output.openTime = queryShift->value(1).toDateTime();
            Output.closeTime = queryShift->value(2).toDateTime();
            Output.open = queryShift->value(3).toBool();
            Output.close = queryShift->value(4).toBool();
            Output.idUser = queryShift->value(5).toInt();
            Output.idPointSale = queryShift->value(6).toInt();
        }
    } else {
        qDebug()<<"Error get shift info:"<<queryShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryShift;

    return Output;
}

QCoffeeShiftInfo QCoffeeClientPlugin::getOpenShift(int idUser,int idPointSale)
{
    QCoffeeShiftInfo Output;

    QString textQuery = "select id_shift,openTime_shift,"
                        "closeTime_shift,open_shift,"
                        "close_shift,shift_id_user,"
                        "shift_id_pointSale "
                        "from tbl_shift where shift_id_user = '" + QString::number(idUser) + "' "
                        "and shift_id_pointSale = '" + QString::number(idPointSale) + "' "
                        "and open_shift = '" + QString::number(true) + "' "
                        "and close_shift = '" + QString::number(false) + "';";

    bool ok;
    QSqlQuery *queryShift = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeShiftInfo currentShift;
        if (queryShift->next()) {
            Output.id = queryShift->value(0).toInt();
            Output.openTime = queryShift->value(1).toDateTime();
            Output.closeTime = queryShift->value(2).toDateTime();
            Output.open = queryShift->value(3).toBool();
            Output.close = queryShift->value(4).toBool();
            Output.idUser = queryShift->value(5).toInt();
            Output.idPointSale = queryShift->value(6).toInt();
        }
    } else {
        qDebug()<<"Error get shift info:"<<queryShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryShift;

    return Output;
}

int QCoffeeClientPlugin::getIdShift(QCoffeeShiftInfo &shiftInfo)
{
    int Output = -1;

    QString textQuery = "select id_shift from tbl_shift "
                        "where openTime_shift = '" + shiftInfo.openTime.toString("yyyy-MM-dd hh:mm:ss") + "' "
                        "and closeTime_shift = '" + shiftInfo.closeTime.toString("yyyy-MM-dd hh:mm:ss") + "' "
                        "and open_shift = '" + QString::number(shiftInfo.open) + "' "
                        "and close_shift = '" + QString::number(shiftInfo.close) + "' "
                        "and shift_id_user = '" + QString::number(shiftInfo.idUser) + "' "
                        "and shift_id_pointSale = '" + QString::number(shiftInfo.idPointSale) + "';";
    bool ok;
    QSqlQuery *queryShift = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeShiftInfo currentShift;
        if (queryShift->next()) {
            Output = queryShift->value(0).toInt();
        }
    } else {
        qDebug()<<"Error get id shift:"<<queryShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryShift;

    return Output;
}

int QCoffeeClientPlugin::addShift(QCoffeeShiftInfo &shiftInfo)
{
    int Output = -1;

    QString textQuery = "insert into tbl_shift (id_shift,"
                                               "openTime_shift,"
                                               "closeTime_shift,"
                                               "open_shift,"
                                               "close_shift,"
                                               "shift_id_user,"
                                               "shift_id_pointSale) "
                        "values ('" + QString::number(shiftInfo.id) + "',"
                                "'" + shiftInfo.openTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                                "'" + shiftInfo.closeTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                                "'" + QString::number(shiftInfo.open) + "',"
                                "'" + QString::number(shiftInfo.close) + "',"
                                "'" + QString::number(shiftInfo.idUser) + "',"
                                "'" + QString::number(shiftInfo.idPointSale) + "');";

    bool ok;
    QSqlQuery *queryAddShift = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdShift(shiftInfo);
    } else {
        qDebug()<<"Error add shift:"<<queryAddShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    return Output;
}

bool QCoffeeClientPlugin::editShift(QCoffeeShiftInfo &shiftInfo)
{
    bool Output;

    QString textQuery = "update tbl_shift set "
                        "id_shift = '" + QString::number(shiftInfo.id) + "',"
                        "openTime_shift = '" + shiftInfo.openTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "closeTime_shift = '" + shiftInfo.closeTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "open_shift = '" + QString::number(shiftInfo.open) + "',"
                        "close_shift = '" + QString::number(shiftInfo.close) + "',"
                        "shift_id_user = '" + QString::number(shiftInfo.idUser) + "',"
                        "shift_id_pointSale = '" + QString::number(shiftInfo.idPointSale) + "' "
                        "where id_shift = '" + QString::number(shiftInfo.id) + "';";

    QSqlQuery *queryEditShift = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit shift:"<<queryEditShift->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditShift;

    return Output;
}

bool QCoffeeClientPlugin::checkOpenShift(int idUser,int idPointSale)
{
    bool Output;

    QCoffeeShiftInfo currentOpenShift = getOpenShift(idUser,idPointSale);

    if (currentOpenShift.id != -1)
        Output = true;
            else Output = false;

    return Output;
}

QVector<QCoffeePictureInfo> QCoffeeClientPlugin::getListPicture()
{
    QVector<QCoffeePictureInfo> Output;

    QString textQuery = "select id_pictureDrink,"
                        "name_pictureDrink,"
                        "data_pictureDrink"
                        " from tbl_pictureDrink;";

    bool ok;
    QSqlQuery *queryPicture = execQueryPicture(textQuery,&ok);

    if (ok) {
        QCoffeePictureInfo currentPicture;
        while (queryPicture->next()) {
            currentPicture.id = queryPicture->value(0).toInt();
            currentPicture.name = queryPicture->value(1).toString();
            currentPicture.data = queryPicture->value(2).toByteArray();

            Output.push_back(currentPicture);
        }
    } else {
        qDebug()<<"Error get picture:"<<queryPicture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPicture;

    return Output;
}

//QCoffeePictureInfo
QCoffeePictureInfo QCoffeeClientPlugin::getPicture (int idPicture)
{
    QCoffeePictureInfo Output;

    QString textQuery = "select id_pictureDrink,"
                        "name_pictureDrink,"
                        "data_pictureDrink"
                        " from tbl_pictureDrink "
                        "where id_pictureDrink = '" + QString::number(idPicture) + "';";

    bool result;
    QSqlQuery *queryPcture = execQueryPicture(textQuery,&result);

    if (result) {
        if (queryPcture->next()) {
            Output.id = queryPcture->value(0).toInt();
            Output.name = queryPcture->value(1).toString();
            Output.data = queryPcture->value(2).toByteArray();
        }
    } else {
        qDebug()<<"Error get picture:"<<queryPcture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryPcture;

    return Output;
}

int QCoffeeClientPlugin::addPicture (QCoffeePictureInfo &picture)
{
    int Output = -1;

    QString textQuery =  "insert into tbl_pictureDrink ("
                         "id_pictureDrink,"
                         "name_pictureDrink,"
                         "data_pictureDrink"
                         ") values ("
                         "'" + QString::number(picture.id) + "',"
                         "'" + picture.name + "',"
                         ":imageData);";

    bool result;
    QSqlQuery *queryAddPicture = execQueryPicture(textQuery,":imageData", picture.data,&result);

    if (result){
        Output = picture.id;
    } else {
        qDebug()<<"Error add picture:"<<queryAddPicture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddPicture;

    return Output;
}

int QCoffeeClientPlugin::getIdPicture(QCoffeePictureInfo &picture)
{
    int Output = -1;

    QString textQuery = "select id_pictureDrink"
                        " from tbl_pictureDrink "
                        "where "
                        "and name_pictureDrink = '" + picture.name + "' "
                        "and data_pictureDrink = :imageData;";



    bool result;
    QSqlQuery *queryPcture = execQueryPicture(textQuery,":imageData",picture.data,&result);

    if (result) {
        if (queryPcture->next())
            Output = queryPcture->value(0).toInt();
    } else {
        qDebug()<<"Error get id picture:"<<queryPcture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }


    delete queryPcture;

    return Output;
}

void QCoffeeClientPlugin::editPicture(QCoffeePictureInfo &picture)
{
    QString textQuery = "update tbl_pictureDrink set "
                        "name_pictureDrink = '" + picture.name + "',"
                        "data_pictureDrink = :imageData;";

    bool result;
    QSqlQuery *queryEditPicture = execQueryPicture(textQuery,":imageData",picture.data,&result);

    if (!result) {
        qDebug()<<"Error edit picture:"<<queryEditPicture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditPicture;
}



//Synchronization
void QCoffeeClientPlugin::doStep ()
{

    if (desktopClient)
    {        switch (currentStep) {
        case 0: loadPointSales();
            break;
         case 1: loadAllCategoryes();
             break;
         case 2: loadAllDrinks();
             break;
         case 3: loadVolumeDrink();
            break;
         case 4: loadTypeDrink();
            break;
         case 5: loadUserInfo();
             break;
         case 6: loadAllPrices();
             break;
         case 7: loadAllShifts();
            break;
         // case 8: loadAllReceipt();
         //     break;
         // case 9: loadAllSoldPositions();
         //    break;
        case 8: updateDrinkPicture(); //10 эт временно
           break;
        default: endSynchronization();
            break;
        }
    }
    else {
        switch (currentStep) {
        case 0: loadPointSales();
            break;
        case 1: loadCategoriesForPointSale();
            break;
        case 2: loadDrinkInfo();
            break;
        case 3: loadVolumeDrink();
            break;
        case 4: loadTypeDrink();
            break;
        case 5: loadPriceInfo(idCurrentPointSale);
            break;
        case 6: loadUserInfo();
            break;
        case 7: openShift(idCurrentPointSale);
            break;
        case 8: loadReceipt(currentShift);
            break;
        case 9: loadSoldPosition(receipt);
            break;
        case 10: updateDrinkPicture();
            break;
        default: endSynchronization();
            break;
        }
    }
}

void QCoffeeClientPlugin::nextStep()
{
    if (firstRun) {
        doStep();
        currentStep++;
    }
}

void QCoffeeClientPlugin::endSynchronization()
{

    if (firstRun) {
        signalMessageSplashScreen(tr("Синхронизация завершена"));
        debugMessage("End synchronization");

        firstRun = false;
        emit signalEndSynchronization();
    }

}

//Commands

void QCoffeeClientPlugin::loadCurrentPintSaleInfo(int idPointSale)
{
    signalMessageSplashScreen(tr("Запрос информации о точке продаж"));
    debugMessage("Send get poin sale info");

    QByteArray buf;

    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream<<idPointSale;

    sendExtData(0x01,buf);
}

void QCoffeeClientPlugin::command01(QByteArray &data)
{
    signalMessageSplashScreen(tr("Прием информации о точке продаж"));
    debugMessage("Recive get poin sale info");
    QCoffeePointSale pointSaleInfo;
    pointSaleInfo.fromByteArray(data);

    currentPointSale = pointSaleInfo;

    int id = getPointSaleId(pointSaleInfo);
    if (id <=0) {
        addPointSale(pointSaleInfo);
    } else {
        editPointSaleInfo(pointSaleInfo);
    }

    emit signalNewPointInfo(pointSaleInfo);

    nextStep();
}

void QCoffeeClientPlugin::loadCategoriesForPointSale()
{
    signalMessageSplashScreen(tr("Запрос информации о категориях"));
    debugMessage("Send get poin sale categories");
    QByteArray buf;

    QVector<int> idCategories = getPointSaleInfo(idCurrentPointSale).idCategories;

    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream<<(quint16)idCategories.length();

    for (int i=0;i<idCategories.length();i++)
        stream<<idCategories.at(i);

    sendExtData(0x02,buf);
}

void QCoffeeClientPlugin::command02(QByteArray &data)
{
    signalMessageSplashScreen(tr("получение информации о категориях"));
    debugMessage("Recive poin sale categories");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);


    QCoffeeCategoryInfo currentCategory;

    int id;
    while (!streamIn.atEnd()) {
        currentCategory << streamIn;
        id = getIdCategory(currentCategory);
        if (id <=0) {
            addCategory(currentCategory);
        } else {
            editCategory(currentCategory);
        }
    }
    emit signalNewCategories();

    nextStep();
}

void QCoffeeClientPlugin::loadDrinkInfo()
{
    signalMessageSplashScreen(tr("Запрос информации о напитках"));
    debugMessage("Send get poin sale drink");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    QVector<int> idDrinks = getDrinkIdForPointSale(idCurrentPointSale);

    stream<<(quint16)idDrinks.length();
    for (int i=0;i<idDrinks.length();i++)
        stream << idDrinks.at(i);

    sendExtData(0x03,Output);
}

void QCoffeeClientPlugin::command03(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации о напитках"));
    debugMessage("Recive poin sale drink");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeDrinkInfo currentDrink;
    quint16 count;
    streamIn>>count;
    int id;
    while (!streamIn.atEnd()) {
        currentDrink <<streamIn;
        id = getIdDrink(currentDrink);

        if (id <=0) {
            addDrinkInfo(currentDrink);
        } else {
            editDrinkInfo(currentDrink);
        }
    }

    emit signalNewDrink();

    nextStep();
}

void QCoffeeClientPlugin::loadVolumeDrink()
{
    signalMessageSplashScreen(tr("Запрос информации о объемах напитков"));
    debugMessage("Send get drink volume");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    QVector<int> idDrinks = getListIdDrink();

    stream<<(quint16)idDrinks.length();
    for (int i=0;i<idDrinks.length();i++)
        stream << idDrinks.at(i);

    sendExtData(0x04,Output);
}

void QCoffeeClientPlugin::command04(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации об объемах напитков"));
    debugMessage("Recive volume for drink");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeVolumeDrinkInfo currentVolume;
    quint16 count;
    streamIn>>count;
    int id;


    while (!streamIn.atEnd()) {
        currentVolume <<streamIn;
        id = getVolumeId(currentVolume);

        if (id <=0) {
            addVolumeDrink(currentVolume);
        } else {
            editVolume(currentVolume);
        }
    }


    emit signalNewVOlume();

    nextStep();
}

void QCoffeeClientPlugin::loadTypeDrink()
{
    signalMessageSplashScreen(tr("Запрос информации о типах напитков"));
    debugMessage("Send get drink type");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x05,Output);
}

void QCoffeeClientPlugin::command05(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации об типах напитков"));
    debugMessage("Recive type for drink");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeTypeDrink currentType;
    quint16 count;
    streamIn>>count;
    int id;


    while (!streamIn.atEnd()) {
        currentType <<streamIn;
        id = getIdTypeDrink(currentType);

        if (id <=0) {
            addTypeDrink(currentType);
        } else {
            editTypeDrink(currentType);
        }
    }


    emit signalNewDrinkType();
    nextStep();
}

void QCoffeeClientPlugin::loadPriceInfo(int idPointSale)
{
    signalMessageSplashScreen(tr("Запрос информации о позициях прайса"));
    debugMessage("Send get price");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<idPointSale;

    sendExtData(0x06,Output);
}

void QCoffeeClientPlugin::command06(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение прайса"));
    debugMessage("Recive price");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeePriceInfo currentPriceItem;
    quint16 count;
    streamIn>>count;
    int id;


    while (!streamIn.atEnd()) {
        currentPriceItem <<streamIn;
        id = getIdPriceInfo(currentPriceItem);

        if (id <=0) {
            addPriceInfo(currentPriceItem);
        } else {
            editPriceInfo(currentPriceItem);
        }
    }

    emit signalNewPrice();
    nextStep();
}

void QCoffeeClientPlugin::loadUserInfo()
{
    signalMessageSplashScreen(tr("Запрос информации о пользователе"));
    debugMessage("Send get user info");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x07,Output);
}

void QCoffeeClientPlugin::command07(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации о пользователе"));
    debugMessage("Recive user info");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    mutexCurrentUserInfo->lock();
    int id;


    currentUserInfo <<streamIn;
    id = getIdUser(currentUserInfo);

    if (id <=0) {
        addUser(currentUserInfo);
    } else {
        editUser(currentUserInfo);
    }
    mutexCurrentUserInfo->unlock();


    emit signalNewUserInfo();
    nextStep();
}

void QCoffeeClientPlugin::openShift(int idPointSale)
{
    signalMessageSplashScreen(tr("Запрос на открытие смены"));
    debugMessage("Send get open shifp");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<idPointSale;

    sendExtData(0x08,Output);
}

void QCoffeeClientPlugin::command08(QByteArray &data)
{
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool newShift;
    streamIn >> newShift;

    if (newShift) {
        signalMessageSplashScreen(tr("Открыта новая смена"));
        debugMessage("Open new shift");
    } else {
        signalMessageSplashScreen(tr("Уже имеется открытая смена"));
        debugMessage("shift already is open");
    }


    currentShift << streamIn;

    int id = -1;
    id = getIdShift(currentShift);

    if (id<=0) {
        addShift(currentShift);
    } else {
        editShift(currentShift);
    }

    emit signalNewShift();
    nextStep();
}

void QCoffeeClientPlugin::closeShift(QCoffeeShiftInfo &shift)
{
    signalMessageSplashScreen(tr("Запрос на закрытие смены"));
    debugMessage("Send get close shifp");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    shift >> stream;

    sendExtData(0x09,Output);
}

void QCoffeeClientPlugin::command09(QByteArray &data)
{
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool result;
    streamIn >> result;

    currentShift << streamIn;
    editShift(currentShift);

    if (result) {
        debugMessage("shift close");
    } else {
        qDebug()<<"Error close shift";
    }

    emit signalCloseShift();
}

void QCoffeeClientPlugin::loadReceipt (QCoffeeShiftInfo &shift)
{
    signalMessageSplashScreen(tr("Запрос чеков для смены"));
    debugMessage("Send get Receipt for shifp");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    shift >> stream;

    sendExtData(0x0A,Output);
}

void QCoffeeClientPlugin::command0A(QByteArray &data)
{

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool result;
    quint16 count;

    streamIn >> result;
    streamIn >> count;

    int id;
    receipt.clear();
    QCoffeeReceiptInfo currentReceipt;
    while (!streamIn.atEnd()) {
        currentReceipt << streamIn;
        id = getIdReceiptInfo(currentReceipt);

        receipt.push_back(currentReceipt);

        if (id<=0) {
            addReceiptInfo(currentReceipt);
        } else {
            editReceiptInfo(currentReceipt);
        }
    }

    emit signalNewReceipt();

    nextStep();
}

void QCoffeeClientPlugin::loadSoldPosition(QVector<QCoffeeReceiptInfo> &receipt)
{
    signalMessageSplashScreen(tr("Запрос проданных позиций"));
    debugMessage("Send get sold position for Receipt");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << (quint16)receipt.length();

    for (int i=0;i<receipt.length();i++)
        stream << receipt.at(i).id;

    sendExtData(0x0B,Output);
}

void QCoffeeClientPlugin::command0B(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получены товарные позиции"));
    debugMessage("Received sold positions");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    quint16 count;
    streamIn >> count;

    int id;
    QCoffeeSoldPositionInfo currentSoldPosition;
    while (!streamIn.atEnd()) {
        currentSoldPosition << streamIn;
        id = getIdSoldPosition(currentSoldPosition);

        if (id<=0) {
            addSoldPosition(currentSoldPosition);
        } else {
            editSoldPosition(currentSoldPosition);
        }
    }

    emit signalNewSoldPosition();
    nextStep();
}

void QCoffeeClientPlugin::sendSale(QCoffeeReceiptInfo &receipt, QVector<QCoffeeSoldPositionInfo> &soldPositions)
{
    signalMessageSplashScreen(tr("Отправка продажи"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    receipt.id = -1;
    receipt.dateTime = QDateTime::currentDateTime();
    receipt.idPointSale = idCurrentPointSale;
    receipt.idShift = currentShift.id;
    receipt.id = addReceiptInfo(receipt);
    receipt >> stream;

    stream << (quint16)soldPositions.length();

    QCoffeeSoldPositionInfo currentSoldPosition;
    for (int i=0;i<soldPositions.length();i++) {
        currentSoldPosition = soldPositions.at(i);

        currentSoldPosition.id = -1;
        currentSoldPosition.idReceipt = receipt.id;
        currentSoldPosition.date = receipt.dateTime;
        currentSoldPosition.idPointSale = idCurrentPointSale;
        currentSoldPosition.idUser = getCurrentUserInfo().id;

        currentSoldPosition.id = addSoldPosition(currentSoldPosition);
        currentSoldPosition >> stream;
    }

    sendExtData(0x0C,Output);
}

void QCoffeeClientPlugin::command0C(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получен информация о продаже"));
    debugMessage("Received sale information");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool result;
    streamIn >> result;

    QCoffeeReceiptInfo currentReceipt;
    currentReceipt << streamIn;


    int id = getIdReceiptInfo(currentReceipt);
    if (id<=0) {
        addReceiptInfo(currentReceipt);
    } else {
        editReceiptInfo(currentReceipt);
    }

    quint16 count;
    streamIn >> count;

    QCoffeeSoldPositionInfo currentSoldPosition;
    while (!streamIn.atEnd()    ) {
        currentSoldPosition << streamIn;
        id = getIdSoldPosition(currentSoldPosition);

        if (id<=0) {
            addSoldPosition(currentSoldPosition);
        } else {
            editSoldPosition(currentSoldPosition);
        }
    }
}

void QCoffeeClientPlugin::uploadPicture(QCoffeePictureInfo &picture)
{
    debugMessage("Upload picture");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    picture >> stream;

    sendExtData(0x0D,Output);
}

void QCoffeeClientPlugin::command0D(QByteArray &data)
{
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool result;
    streamIn >> result;

    if (result) {
        debugMessage("Picture is uploaded");
    } else debugMessage("Error upload picture");

    QCoffeePictureInfo currentPicture;
    currentPicture << streamIn;

    emit signalImageUploaded(result);
}

void QCoffeeClientPlugin::updateDrinkPicture()
{
    signalMessageSplashScreen(tr("Синхронизация изображений напитков"));
    currentStep++;
    debugMessage("update drink picture");
    idPictures.clear();

    QVector<QCoffeeDrinkInfo> drinks = getListDrink();

    QVector<int> buf;
    QCoffeeDrinkInfo currentDrink;
    for (int i=0;i<drinks.length();i++) {
        currentDrink = drinks.at(i);

        if ((buf.indexOf(currentDrink.idPicture) <0) &&
                (currentDrink.idPicture > 0))
            buf.push_back(currentDrink.idPicture);
    }

    QCoffeePictureInfo currentPicture;
    for (int i=0;i<buf.length();i++) {
        currentPicture = getPicture(buf.at(i));

        if (currentPicture.id == -1)
            idPictures.push_back(buf.at(i));
    }

    if (idPictures.length()>0) {
        getPictureInformation(idPictures.at(0));
        idPictures.remove(0,1);
    } else {
        nextStep();
    }

}

void QCoffeeClientPlugin::getPictureInformation(int idPicture)
{
    debugMessage("Get picture info, id picture:"+QString::number(idPicture));

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idPicture;

    sendExtData(0x0E,Output);
}

void QCoffeeClientPlugin::command0E(QByteArray &data)
{
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeePictureInfo currenrPivture;
    currenrPivture << streamIn;

    debugMessage("Recive picture info, id picture:"+QString::number(currenrPivture.id));

    addPicture(currenrPivture);

    if (idPictures.length()>0) {
        getPictureInformation(idPictures.at(0));
        idPictures.remove(0,1);
    } else {
        nextStep();
    }
}

void QCoffeeClientPlugin::loadPointSales()
{
    signalMessageSplashScreen(tr("Запрос информации о точках продаж"));
    debugMessage("Send get poin sale info");

    QByteArray buf;

    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream << 0x01;
    sendExtData(0x0F,buf);
}

void QCoffeeClientPlugin::command0F(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение колличества точек продаж"));
    debugMessage("Received sold positions");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QVector<int> idPointSales;
    streamIn >> idPointSales;

    countPointSales = idPointSales.count();

    for(int i=0;i<idPointSales.count();i++)
    {
        loadPointSale(idPointSales.at(i));
    }



}

void QCoffeeClientPlugin::loadPointSale(int idPointSale)
{
    debugMessage("Send get poin sale info");

    QByteArray buf;

    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream<<idPointSale;

    sendExtData(0x10,buf);
}

void QCoffeeClientPlugin::command10(QByteArray &data)
{
    signalMessageSplashScreen(tr("Прием информации о точке продаж"));
    debugMessage("Recive get poin sale info");
    QCoffeePointSale pointSaleInfo;
    pointSaleInfo.fromByteArray(data);

    currentPointSale = pointSaleInfo;

    int id = getPointSaleId(pointSaleInfo);
    if (id <=0) {
        addPointSale(pointSaleInfo);
    } else {
        editPointSaleInfo(pointSaleInfo);
    }

    emit signalNewPointInfo(pointSaleInfo);

    counterPointSales++;
    if (counterPointSales==countPointSales) {
        nextStep();
    }
}

void QCoffeeClientPlugin::loadAllCategoryes()
{
    signalMessageSplashScreen(tr("Запрос информации о категориях"));
    debugMessage("Send get categories");
    QByteArray buf;

    QDataStream stream(&buf,QIODevice::WriteOnly);
    stream << 0x01;
    sendExtData(0x11,buf);
}

void QCoffeeClientPlugin::command11(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации о категориях"));
    debugMessage("Recive categories");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);


    QCoffeeCategoryInfo currentCategory;

    int id;
    while (!streamIn.atEnd()) {
        currentCategory << streamIn;
        id = getIdCategory(currentCategory);
        if (id <=0) {
            addCategory(currentCategory);
        } else {
            editCategory(currentCategory);
        }
    }
    emit signalNewCategories();

    nextStep();
}

void QCoffeeClientPlugin::loadAllDrinks()
{
    signalMessageSplashScreen(tr("Запрос информации о напитках"));
    debugMessage("Send request for getting drinks");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << 0x01;

    sendExtData(0x12,Output);
}

void QCoffeeClientPlugin::command12(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение информации о напитках"));
    debugMessage("Recive drinks info");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeDrinkInfo currentDrink;

    int id;
    while (!streamIn.atEnd()) {
        currentDrink <<streamIn;
        id = getIdDrink(currentDrink);

        if (id <=0) {
            addDrinkInfo(currentDrink);
        } else {
            editDrinkInfo(currentDrink);
        }
    }

    emit signalNewDrink();

    nextStep();
}

void QCoffeeClientPlugin::loadAllPrices()
{
    signalMessageSplashScreen(tr("Запрос информации о позициях прайса"));
    debugMessage("Send get price");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x13,Output);
}

void QCoffeeClientPlugin::command13(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение всех цен"));
    debugMessage("Recive price");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeePriceInfo currentPriceItem;
    int id;

    while (!streamIn.atEnd()) {
        currentPriceItem <<streamIn;
        id = getIdPriceInfo(currentPriceItem);

        if (id <=0) {
            addPriceInfo(currentPriceItem);
        } else {
            editPriceInfo(currentPriceItem);
        }
    }

    emit signalNewPrice();
    nextStep();
}

void QCoffeeClientPlugin::loadAllShifts()
{
    signalMessageSplashScreen(tr("Запрос информации о сменах пользователей"));
    debugMessage("Send get user shift info");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x14,Output);
}

void QCoffeeClientPlugin::command14(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение всех смен"));
    debugMessage("Recive shift");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeShiftInfo currentShiftInfo;
    int id;

    while (!streamIn.atEnd()) {
        currentShiftInfo <<streamIn;
        id = getIdShift(currentShiftInfo);

        if (id <=0) {
            addShift(currentShiftInfo);
        } else {
            editShift(currentShiftInfo);
        }
    }

    emit signalNewPrice();
    nextStep();
}

void QCoffeeClientPlugin::loadAllReceipt()
{
    signalMessageSplashScreen(tr("Запрос информации о чеках"));
    debugMessage("Send get receipt info");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x15,Output);
}

void QCoffeeClientPlugin::command15(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение всех чеков"));
    debugMessage("Recive receipt");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeReceiptInfo currentReceiptInfo;
    int id;

    while (!streamIn.atEnd()) {
        currentReceiptInfo <<streamIn;
        id = getIdReceiptInfo(currentReceiptInfo);

        if (id <=0) {
            addReceiptInfo(currentReceiptInfo);
        } else {
            editReceiptInfo(currentReceiptInfo);
        }
    }

    emit signalNewPrice();
    nextStep();
}

void QCoffeeClientPlugin::loadAllSoldPositions()
{
    signalMessageSplashScreen(tr("Запрос информации о проданных позициях"));
    debugMessage("Send get sold positions info");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream<<0x01;

    sendExtData(0x16,Output);
}

void QCoffeeClientPlugin::command16(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получение всех проданных позиций"));
    debugMessage("Recive sold positions");

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeSoldPositionInfo currentSoldPositionInfo;
    int id;

    while (!streamIn.atEnd()) {
        currentSoldPositionInfo <<streamIn;
        id = getIdSoldPosition(currentSoldPositionInfo);

        if (id <=0) {
            addSoldPosition(currentSoldPositionInfo);
        } else {
            editSoldPosition(currentSoldPositionInfo);
        }
    }

    emit signalNewPrice();
    nextStep();
}

void QCoffeeClientPlugin::crudOpDrinkInfo(QCoffeeDrinkInfo &drinkInfo, quint32 idOperation)
{
    signalMessageSplashScreen(tr("Отправка напитка"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idOperation;
    drinkInfo >> stream;
    qDebug()<<drinkInfo.id;
    sendExtData(0xA0,Output);
}




void QCoffeeClientPlugin::commandA0(QByteArray &data)
{
    signalMessageSplashScreen(tr("Получен информация о напитке"));
    debugMessage("Received drink information");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    bool result;
    streamIn >> result;

    quint32 code;
    streamIn >> code;

    QCoffeeDrinkInfo currentDrink;
    currentDrink << streamIn;

    switch (code) {
        case 0x01: addDrinkInfo(currentDrink);
            break;
        case 0x02: editDrinkInfo(currentDrink);
            break;
        case 0x03: deleteDrinkInfo(currentDrink);
            break;
    }

    emit signalNewDrink();
}




void QCoffeeClientPlugin::sendPredictionRequest(QDate startDate, QDate endDate, quint32 id,qint32 idWg) {

    QByteArray Output;
    QDataStream stream(&Output, QIODevice::WriteOnly);
    qDebug()<<"command sendPredictionRequest";

    stream << id;
    stream << startDate;
    stream << endDate;
    stream <<idWg;

    sendExtData(0x17, Output);
}

void QCoffeeClientPlugin::command17(QByteArray &data) {
    QDataStream streamIn(&data, QIODevice::ReadOnly);
    qDebug()<<"Prediction for product recived";
    QMap<QDateTime, float> predictionResults;
    qint32 idWg;
    qint32 id;
    streamIn >> predictionResults;
    streamIn >> idWg;
    streamIn >> id;

    if (idWg) {
        emit predictionDataReceived(predictionResults);
    } else {
        emit predictionDataReceivedToStat(predictionResults,id);
    }

}


void QCoffeeClientPlugin::sendPredictionRequestFotStat(QDate startDate, QDate endDate, qint32 idstart, qint32 idend) {
    QByteArray Output;
    QDataStream stream(&Output, QIODevice::WriteOnly);
    qDebug()<<"command sendPredictionRequestForStat";

    stream << idstart;
    stream << startDate;
    stream << endDate;
    stream <<idend;

    sendExtData(0x18, Output);
}

void QCoffeeClientPlugin::command18(QByteArray &data) {
    QDataStream streamIn(&data, QIODevice::ReadOnly);
    qDebug()<<"Prediction for stat recived";
    QMap<qint32, float> predictionResults;

    streamIn >> predictionResults;


    emit predictionDataReceivedForStart(predictionResults);

}


void QCoffeeClientPlugin::crudCategoryInfo(QCoffeeCategoryInfo &categoryInfo,QVector<qint32> newDrinks,quint32 idOperation) {
    signalMessageSplashScreen(tr("Отправка категории"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idOperation;
    stream << newDrinks;
    categoryInfo >> stream;
    qDebug()<<categoryInfo.id;
    sendExtData(0x19,Output);
}

void QCoffeeClientPlugin::command19(QByteArray &data) {
    signalMessageSplashScreen(tr("Получен информация о категории"));
    debugMessage("Received drink information");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    qDebug()<<"Получен информация о категории";
    bool result;
    streamIn >> result;

    quint32 code;
    streamIn >> code;

    QVector<qint32> newDrinks;
    streamIn >> newDrinks;
    QCoffeeCategoryInfo currentCategory;
    currentCategory << streamIn;


    if (code!=0x03) unlinkCategoryAndDrink(currentCategory.id);

    switch (code) {
    case 0x01: addCategory(currentCategory);
        break;
    case 0x02: editCategory(currentCategory);
        break;
    case 0x03: deleteCategoryInfo(currentCategory);
        break;
    }

    if (code!=0x03) {
        for (int i=0;i<newDrinks.size();i++) {
            linkDrinkAndCategory(newDrinks.at(i),currentCategory.id);
        }
    }


    emit signalNewCategory();
}


void QCoffeeClientPlugin::crudPointSaleInfo(QCoffeePointSale &pointSaleInfo,quint32 idOperation) {
    signalMessageSplashScreen(tr("Отправка категории"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idOperation;
    stream << pointSaleInfo.id;
    stream <<pointSaleInfo.name;
    stream << pointSaleInfo.idCategories;
    stream << pointSaleInfo.idDrinks;



    sendExtData(0x25,Output);
}


void QCoffeeClientPlugin::command25(QByteArray &data) {
    signalMessageSplashScreen(tr("Получен информация о точке продаж"));

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    qDebug()<<"Получен информация о точке продаж";
    bool result;
    streamIn >> result;

    quint32 code;
    streamIn >> code;


    QCoffeePointSale currentPointSale;



    streamIn >> currentPointSale.id;
    streamIn >> currentPointSale.name;
    streamIn >> currentPointSale.idCategories;
    streamIn >> currentPointSale.idDrinks;




    //if (code!=0x03) unlinkCategoryAndDrink(currentCategory.id);

    switch (code) {
        case 0x01: addPointSale(currentPointSale);
            break;
        // case 0x02: editCategory(currentCategory);
        //     break;
        // case 0x03: deleteCategoryInfo(currentCategory);
        //     break;
        }

    // if (code!=0x03) {
    //     for (int i=0;i<newDrinks.size();i++) {
    //         linkDrinkAndCategory(newDrinks.at(i),currentCategory.id);
    //     }
    // }


   emit signalNewPoint();
}



void QCoffeeClientPlugin::crudVolumeInfo(QCoffeeVolumeDrinkInfo &volumeInfo,quint32 idOperation) {

    signalMessageSplashScreen(tr("Отправка обьема"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idOperation;

    volumeInfo >> stream;

    qDebug()<<volumeInfo.id;
    sendExtData(0x20,Output);

}



void QCoffeeClientPlugin::command20(QByteArray &data) {
    signalMessageSplashScreen(tr("Получен информация о обьеме"));

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    qDebug()<<"Получен информация о обьеме";
    bool result;
    streamIn >> result;

    quint32 code;
    streamIn >> code;


    QCoffeeVolumeDrinkInfo currentVolume;
    currentVolume << streamIn;




    switch (code) {
    case 0x01: addVolumeDrink(currentVolume);
        break;
    case 0x02: //editCategory(currentCategory);
        break;
    case 0x03: //deleteCategoryInfo(currentCategory);
        break;
    }



    emit signalVolumeGetted();



}



void QCoffeeClientPlugin::crudPriceInfo(QCoffeePriceInfo &priceInfo,quint32 idOperation) {

    signalMessageSplashScreen(tr("Отправка обьема"));
    debugMessage("Sand sale");

    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << idOperation;

    priceInfo >> stream;

    qDebug()<<priceInfo.id;
    sendExtData(0x21,Output);

}



void QCoffeeClientPlugin::command21(QByteArray &data) {
    signalMessageSplashScreen(tr("Получен информация о цене"));

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    qDebug()<<"Получен информация о цене";
    bool result;
    streamIn >> result;

    quint32 code;
    streamIn >> code;


    QCoffeePriceInfo currentPrice;
    currentPrice << streamIn;




    switch (code) {
    case 0x01: addPriceInfo(currentPrice);
        break;
    case 0x02: //editCategory(currentCategory);
        break;
    case 0x03: //deleteCategoryInfo(currentCategory);
        break;
    }



}

void QCoffeeClientPlugin::sendRelearnRequest() {
    QByteArray Output;
    QDataStream stream(&Output, QIODevice::WriteOnly);
    stream << 1;
    qDebug()<<"command sendLearnRequest";

    sendExtData(0x22, Output);
}


void QCoffeeClientPlugin::command22(QByteArray &data) {

    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);



    qint32 progress;
    streamIn >> progress;
    qDebug()<<"new status"<<progress;
    emit signalSendStatus(progress);
}


void QCoffeeClientPlugin::getListNnVersions() {

    signalMessageSplashScreen(tr("Запрос на получение версий"));


    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream <<1;

    sendExtData(0x23,Output);




}

void QCoffeeClientPlugin::command23(QByteArray &data) {


    QMap<QString,QString> config;
     QList<QString> result;
    qint32 progresLearn;
    \
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    qDebug()<<"Получены версий и конфига нейронки";

    streamIn >> result;
    streamIn >> config;
    streamIn >> progresLearn;



    emit signalNnVersiomsGetted(result,config,progresLearn);


}

void QCoffeeClientPlugin::sendNnConfig(QMap<QString,QString> &config) {

    signalMessageSplashScreen(tr("Запрос на обновление конфига"));


    QByteArray Output;
    QDataStream stream(&Output,QIODevice::WriteOnly);

    stream << config;

    sendExtData(0x24,Output);

}
