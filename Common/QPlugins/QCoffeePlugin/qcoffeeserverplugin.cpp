#include "qcoffeeserverplugin.h"

QCoffeeServerPlugin::QCoffeeServerPlugin(QUnNetProtokolServer *server,quint16 business, QObject *parent)
    : QServerPlugin (server, business, parent)
{
    qDebug()<<"Create QCoffeeServerPlugin";

    currentPluginCode = 0x04;
    currentPluginName = "QCoffeeServerPlugin";
    currentIdBusiness = business;
}

QCoffeeServerPlugin::~QCoffeeServerPlugin()
{
    qDebug()<<"Destroy QCoffeeServerPlugin";
}

void QCoffeeServerPlugin::processCommand(quint8 command,QByteArray data, QUnClientHandler *client)
{
    qDebug()<<"QCoffeeServerPlugin::processCommand "<<command;

    debugMessage("processCommand: " + QString::number(command));

    switch (command) {
    case 0x01: command01(data,client);
        break;
    case 0x02: command02(data,client);
        break;
    case 0x03: command03(data,client);
        break;
    case 0x04: command04(data,client);
        break;
    case 0x05: command05(data,client);
        break;
    case 0x06: command06(data,client);
        break;
    case 0x07: command07(data,client);
        break;
    case 0x08: command08(data,client);
        break;
    case 0x09: command09(data,client);
        break;
    case 0x0A: command0A(data,client);
        break;
    case 0x0B: command0B(data,client);
        break;
    case 0x0C: command0C(data,client);
        break;
    case 0x0D: command0D(data,client);
        break;
    case 0x0E: command0E(data,client);
        break;
    case 0x0F: command0F(data,client);
        break;
    case 0x10: command10(data,client);
        break;
    case 0x11: command11(data,client);
        break;
    case 0x12: command12(data,client);
        break;
    case 0x13: command13(data,client);
        break;
    case 0x14: command14(data,client);
        break;
    case 0x15: command15(data,client);
        break;
    case 0x16: command16(data,client);
        break;
    case 0xA0: commandA0(data,client);
        break;
    case 0x17: command17(data,client);
        break;
    case 0x18: command18(data,client);
        break;
    case 0x19: command19(data,client);
        break;
    case 0x20: command20(data,client);
        break;
    case 0x21: command21(data,client);
        break;
    }

}

void QCoffeeServerPlugin::dataBaseIsInit()
{
    qDebug()<<"QCoffeeServerPlugin::dataBaseIsInit";
}

//QCoffeePointSale

QVector<QCoffeePointSale> QCoffeeServerPlugin::getListPointSale()
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

QVector<int> QCoffeeServerPlugin::getDrinkIdForPointSale(int idPointSale)
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

QVector<int> QCoffeeServerPlugin::getCategoryIdForPointSale(int idPointSale)
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

QCoffeePointSale QCoffeeServerPlugin::getPointSaleInfo(int idPointSale)
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

bool QCoffeeServerPlugin::editPointSaleInfo(QCoffeePointSale pointSaleInfo)
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

bool  QCoffeeServerPlugin::unlinkPointSaleAndDrink(int idPointSale)
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

bool  QCoffeeServerPlugin::unlinkPointSaleAndCategory(int idPointSale)
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

bool QCoffeeServerPlugin::linkPointSaleAndDrink(int idPointSale,int idDrink)
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

bool QCoffeeServerPlugin::linkPointSaleAndCategory(int idPointSale,int idCategory)
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

int QCoffeeServerPlugin::getPointSaleId(QCoffeePointSale pointSale)
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

int QCoffeeServerPlugin::addPointSale(QCoffeePointSale pointSale)
{
    int Output = -1;

    int nextId = getNextId("tbl_pointSale","id_pointSale");
    QString textQuery = "insert into tbl_pointSale (id_pointSale,name_pointSalecol) "
                        "values ('" + QString::number(nextId) + "','" + pointSale.name + "');";

    bool ok = false;
    QSqlQuery *queryAddPointSale = execQuery(textQuery,&ok);

    if (ok) {
        int currentPointId = getPointSaleId(pointSale);

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

QVector<QCoffeeCategoryInfo> QCoffeeServerPlugin::getListCategories()
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

QCoffeeCategoryInfo QCoffeeServerPlugin::getCategoryInfo(int idCategory)
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

QVector<QCoffeeCategoryInfo> QCoffeeServerPlugin::getListCategories(int idPointSale)
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

QVector<int> QCoffeeServerPlugin::getIdPointSaleForCatigory(int idCategory)
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

bool QCoffeeServerPlugin::linkCategoryAndPointSale(int idCategory, int idPointSale)
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

bool QCoffeeServerPlugin::unlinkCategoryAndPointSale(int idCategory)
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

int QCoffeeServerPlugin::getIdCategory(QCoffeeCategoryInfo &category)
{
    int Output = -1;

    QString textQuery = "select id_drinkСategory from tbl_drinkСategory "
                        "where name_drinkСategory = '" + category.name + "' "
                        "and description_drinkСategory = '" + category.description + "' "
                        "and red_drinkСategory = '" + QString::number(category.color.red()) + "' "
                        "and green_drinkСategory = '" + QString::number(category.color.green()) + "' "
                        "and blue_drinkСategory  = '" + QString::number(category.color.blue()) + "';";

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

bool QCoffeeServerPlugin::editCategory(QCoffeeCategoryInfo &category)
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

int QCoffeeServerPlugin::addCategory(QCoffeeCategoryInfo &category)
{
    int Output = -1;

    int nextId = getNextId("tbl_drinkСategory","id_drinkСategory");
    QString textQuery = "insert into tbl_drinkСategory ("
                        "id_drinkСategory,"
                        "name_drinkСategory,"
                        "description_drinkСategory,"
                        "red_drinkСategory,"
                        "green_drinkСategory,"
                        "blue_drinkСategory) values ("
                        "'" + QString::number(nextId) + "',"
                        "'" + category.name + "',"
                        "'" + category.description + "',"
                        "'" + QString::number(category.color.red()) + "',"
                        "'" + QString::number(category.color.green()) + "',"
                        "'" + QString::number(category.color.blue()) + "');";

    bool ok = false;
    QSqlQuery *queryAddCategory = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdCategory(category);
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

QVector<QCoffeeUserInfo> QCoffeeServerPlugin::getListUser()
{
    QVector<QCoffeeUserInfo> Output;

    QString textQuery = "select id_user,login_user,password_user,name_user,patronymic_user,surname_user "
                        "from tbl_user;";

    bool ok = false;
    QSqlQuery *queryListUserInfo = execQuery(textQuery,&ok);

    if (ok) {
        QCoffeeUserInfo currentUserInfo;
        while (queryListUserInfo->next()) {
            currentUserInfo.id = queryListUserInfo->value(0).toInt();
            currentUserInfo.login = queryListUserInfo->value(1).toInt();
            currentUserInfo.password = queryListUserInfo->value(2).toString();
            currentUserInfo.name = queryListUserInfo->value(3).toString();
            currentUserInfo.patronymic = queryListUserInfo->value(4).toString();
            currentUserInfo.surname = queryListUserInfo->value(5).toString();

            Output.push_back(currentUserInfo);
        }

    } else {
        qDebug()<<"Error get list user info:"<<queryListUserInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListUserInfo;

    return Output;
}

QCoffeeUserInfo QCoffeeServerPlugin::getUserInfo(int idUser)
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

QCoffeeUserInfo QCoffeeServerPlugin::getUserInfo(QString login)
{
    QCoffeeUserInfo Output;

    QString textQuery = "select id_user,login_user,password_user,name_user,patronymic_user,surname_user "
                        "from tbl_user where login_user = '" + login + "';";

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

int QCoffeeServerPlugin::getIdUser(QCoffeeUserInfo &userInfo)
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

int QCoffeeServerPlugin::addUser(QCoffeeUserInfo &userInfo)
{
    int Output = -1;

    int nextId = getNextId("tbl_user","id_user");
    QString textQuery = "insert into tbl_user (id_user,login_user,password_user,name_user,patronymic_user,surname_user) "
                        "values ('" + QString::number(nextId) + "','" + userInfo.login + "','" + userInfo.password + "','" + userInfo.name + "','" + userInfo.patronymic + "','" + userInfo.surname + "');";

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

bool QCoffeeServerPlugin::editUser(QCoffeeUserInfo &userInfo)
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

QVector<QCoffeeTypeDrink> QCoffeeServerPlugin::getListTypeDrink()
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

QCoffeeTypeDrink QCoffeeServerPlugin::getTypeDrinkInfo (int idTypeDrink)
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

int QCoffeeServerPlugin::getIdTypeDrink (QCoffeeTypeDrink &typeDrink)
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

int QCoffeeServerPlugin::addTypeDrink (QCoffeeTypeDrink &typeDrink)
{
    int Output = -1;

    int nextId = getNextId("tbl_typeDrink","id_typeDrink");
    QString textQuery = "insert into tbl_typeDrink (id_typeDrink,name_typeDrink,description_typeDrink) "
                        "values ('" + QString::number(nextId) + "','" + typeDrink.name + "','" + typeDrink.description + "');";

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

bool QCoffeeServerPlugin::editTypeDrink (QCoffeeTypeDrink &typeDrink)
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

QVector<QCoffeeVolumeDrinkInfo> QCoffeeServerPlugin::getListVolume()
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

QVector<QCoffeeVolumeDrinkInfo> QCoffeeServerPlugin::getListVolume(int idDrink)
{
    QVector<QCoffeeVolumeDrinkInfo> Output;
    QVector<int> idVolume = getIdVolumeForDrink(idDrink);

    for (int i=0;i<idVolume.length();i++)
        Output.push_back(getVolumeInfo(idVolume.at(i)));


    return Output;
}

QCoffeeVolumeDrinkInfo QCoffeeServerPlugin::getVolumeInfo(int idVolume)
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

            Output.idDrink = getIdDrinkForVolume(Output.id);
        }

    } else {
        qDebug()<<"Error get list volume:"<<queryListVolume->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListVolume;

    return Output;
}

int QCoffeeServerPlugin::getVolumeId(QCoffeeVolumeDrinkInfo &volume)
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

QVector<int> QCoffeeServerPlugin::getIdDrinkForVolume(int idVolume)
{
    QVector<int> Output;

    QString textQuery = "select tbl_drink_id_drink from tbl_drink_volumeDrink "
                        "where "
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

bool QCoffeeServerPlugin::linkVolumeAndDrink(int idVolume, int idDrink)
{
    bool Output = false;

    QString textQuery = "insert into tbl_drink_volumeDrink (tbl_drink_id_drink,tbl_volumeDrink_id_volumeDrink) "
                        "values ('" + QString::number(idDrink) + "','" + QString::number(idVolume) + "');";

    QSqlQuery *queryLinkColumeAndDrink = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error link volume and drink:"<<queryLinkColumeAndDrink->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryLinkColumeAndDrink;

    return Output;
}

bool QCoffeeServerPlugin::unlinkVolumeAndDrink(int idVolume)
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

int QCoffeeServerPlugin::addVolumeDrink(QCoffeeVolumeDrinkInfo &volume)
{
    int Output = -1;

    int nextId = getNextId("tbl_volumeDrink","id_volumeDrink");
    QString textQuery = "insert into tbl_volumeDrink (id_volumeDrink,"
                                                     "name_volumeDrink,"
                                                     "volume_volumeDrink,"
                                                     "description_volumeDrink,"
                                                     "units_volumeDrink"
                        ") values ("
                                    "'" + QString::number(nextId) + "',"
                                    "'" + volume.name + "',"
                                    "'" + QString::number(volume.volume) + "',"
                                    "'" + volume.description +"',"
                                    "'" + volume.units + "');";

    bool ok = false;
    QSqlQuery *queryAddVolume = execQuery(textQuery,&ok);

    if (ok) {
        Output = getVolumeId(volume);
        for (int i=0;i<volume.idDrink.length();i++)
            linkVolumeAndDrink(Output,volume.idDrink.at(i));

    } else {
        qDebug()<<"Error add volume:"<<queryAddVolume->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddVolume;

    return Output;
}

bool QCoffeeServerPlugin::editVolume(QCoffeeVolumeDrinkInfo &volume)
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

QVector<QCoffeeDrinkInfo> QCoffeeServerPlugin::getListDrink()
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

QVector<int> QCoffeeServerPlugin::getIdVolumeForDrink(int idDrink)
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

QVector<int> QCoffeeServerPlugin::getIdCategoriesForDrink(int idDrink)
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

QCoffeeDrinkInfo QCoffeeServerPlugin::getDrinkInfo(int idDrink)
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

int QCoffeeServerPlugin::addDrinkInfo(QCoffeeDrinkInfo &drinkInfo)
{
    int Output = -1;

    int nextId = getNextId("tbl_drink","id_drink");
    QString textQuery = "insert into tbl_drink ("
                        "id_drink,"
                        "name_drink,"
                        "description_drink,"
                        "red_drink,"
                        "green_drink,"
                        "blue_drink,"
                        "drink_id_pictureDrink,"
                        "hash_drink"
                        ") values ("
                        "'" + QString::number(nextId) + "',"
                        "'" + drinkInfo.name + "',"
                        "'" + drinkInfo.description + "',"
                        "'" + QString::number(drinkInfo.color.red()) + "',"
                        "'" + QString::number(drinkInfo.color.green()) + "',"
                        "'" + QString::number(drinkInfo.color.blue()) + "',"
                        "'" + QString::number(drinkInfo.idPicture) + "',"
                        "'" + drinkInfo.pictureHash + "'"
                        ");";

    bool ok = false;
    QSqlQuery *queryDrinkInfo = execQuery(textQuery,&ok);

    if (ok) {
        Output = getIdDrink(drinkInfo);

        for (int i=0;i<drinkInfo.idCategories.length();i++)
            linkDrinkAndCategory(Output,drinkInfo.idCategories.at(i));

    } else {
        qDebug()<<"Error get drink info:"<<queryDrinkInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryDrinkInfo;

    return Output;
}

int QCoffeeServerPlugin::getIdDrink (QCoffeeDrinkInfo &drinkInfo)
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

bool QCoffeeServerPlugin::editDrinkInfo(QCoffeeDrinkInfo &drinkInfo)
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


bool QCoffeeServerPlugin::deleteDrinkInfo(QCoffeeDrinkInfo &drinkInfo) {
    bool Output = false;

    unlinkDrinkAndCategory(drinkInfo.id);
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

bool QCoffeeServerPlugin::deleteCategoryInfo(QCoffeeCategoryInfo &categoryInfo) {
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

bool QCoffeeServerPlugin::unlinkDrinkAndCategory(int idDrink)
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

bool QCoffeeServerPlugin::linkDrinkAndCategory(int idDrink,int idCategory)
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

bool QCoffeeServerPlugin::unlinkCategoryAndDrink(int idCategory)
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

QVector<QCoffeePriceInfo> QCoffeeServerPlugin::getListPriceInfo()
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

QVector<QCoffeePriceInfo> QCoffeeServerPlugin::getListPricePointSale(int idPointSale)
{
    QVector<QCoffeePriceInfo> Output;

    QString textQuery = "select tbl_price_id_price from tbl_pointSale_price "
                        "where tbl_pointSale_id_pointSale = '" + QString::number(idPointSale) + "';";

    QVector<int> idPrice;

    bool ok = false;
    QSqlQuery *queryIdPrice = execQuery(textQuery,&ok);

    if (ok) {
        while (queryIdPrice->next()) {
            Output.push_back(getPriceInfo(queryIdPrice->value(0).toInt()));
        }

    } else {
        qDebug()<<"Error get list id price for point sale:"<<queryIdPrice->lastError().text();
        qDebug()<<"textQuery="<<textQuery;
    }

    delete queryIdPrice;

    return Output;
}

QCoffeePriceInfo QCoffeeServerPlugin::getPriceInfo(int idPriceInfo)
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

int QCoffeeServerPlugin::getIdPriceInfo(QCoffeePriceInfo &priceInfo)
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

QVector<int> QCoffeeServerPlugin::getIdPointSaleForPriceInfo(int idPriceinfo)
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

bool QCoffeeServerPlugin::unlinkPriceInfoAndPointSale(int idPriceInfo)
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

bool QCoffeeServerPlugin::linkPriceInfoAndPointSale(int idPriceInfo, int idPointSale)
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

int QCoffeeServerPlugin::addPriceInfo(QCoffeePriceInfo &priceInfo)
{
    int Output = -1;

    int nextId = getNextId("tbl_price","id_price");
    QString textQuery = "insert into tbl_price (id_price,"
                        "value_price,"
                        "startTime_price,"
                        "description_price,"
                        "price_id_drink,"
                        "price_id_volumeDrink,"
                        "price_id_drinkСategory) "
                        "values ('" + QString::number(nextId) +"',"
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

        for (int i=0;i<priceInfo.idPointSale.length();i++)
            linkPriceInfoAndPointSale(Output,priceInfo.idPointSale.at(i));

    } else {
        qDebug()<<"Error add price:"<<queryAddPrice->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddPrice;

    return Output;
}

bool QCoffeeServerPlugin::editPriceInfo(QCoffeePriceInfo &priceInfo)
{
    bool Output = false;

    QString textQuery = "update tbl_price set "
                        "value_price = '" + QString::number(priceInfo.value) + "',"
                        "startTime_price = '" + priceInfo.startDate.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "description_price = '" + priceInfo.description + "',"
                        "price_id_drink = '" + QString::number(priceInfo.idDrink) + "',"
                        "price_id_volumeDrink = '" + QString::number(priceInfo.idVolume) + "'."
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

QVector<QCoffeeReceiptInfo> QCoffeeServerPlugin::getListReceiptInfo()
{
    QVector<QCoffeeReceiptInfo> Output;

    QString textQuery = "select id_receipt,date_receipt,sum_receipt,receipt_id_pointSale,receipt_id_shift from tbl_receipt;";

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

QVector<QCoffeeReceiptInfo> QCoffeeServerPlugin::getListReceiptInfo(int idShift)
{
    QVector<QCoffeeReceiptInfo> Output;

    QString textQuery = "select id_receipt,"
                        "date_receipt,"
                        "sum_receipt,"
                        "receipt_id_pointSale,"
                        "receipt_id_shift "
                        "from tbl_receipt "
                        "where receipt_id_shift = '" + QString::number(idShift) + "';";

    qDebug()<<"textQuery = "<<textQuery;

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

QCoffeeReceiptInfo QCoffeeServerPlugin::getReceiptInfo(int idReceiptInfo)
{
    QCoffeeReceiptInfo Output;

    QString textQuery = "select id_receipt,date_receipt,sum_receipt,sum_receipt,receipt_id_shift from tbl_receipt "
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

int QCoffeeServerPlugin::getIdReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
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

int QCoffeeServerPlugin::addReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
{
    int Output = -1;

    int nextId = getNextId("tbl_receipt","id_receipt");
    QString textQuery = "insert into tbl_receipt (id_receipt,date_receipt,sum_receipt,receipt_id_pointSale,receipt_id_shift) "
                        "values ('" + QString::number(nextId) + "',"
                        "'" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "',"
                        "'" + QString::number(receiptInfo.sum) + "',"
                        "'" + QString::number(receiptInfo.idPointSale) + "',"
                        "'" + QString::number(receiptInfo.idShift) + "');";

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

bool QCoffeeServerPlugin::editReceiptInfo(QCoffeeReceiptInfo &receiptInfo)
{
    bool Output = false;

    QString textQuery = "update tbl_receipt set date_receipt = '" + receiptInfo.dateTime.toString("yyyy-MM-dd hh:mm:ss") + "', "
                        "sum_receipt = '" + QString::number(receiptInfo.sum) + "', "
                        "receipt_id_pointSale = '" + QString::number(receiptInfo.idPointSale) + "',"
                        "receipt_id_shift = '" + QString::number(receiptInfo.idShift) + "' "
                        "where id_receipt = '" + QString::number(receiptInfo.id) + "';";

    QSqlQuery *queryEditReceiptInfo = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit receipt info:"<<queryEditReceiptInfo->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditReceiptInfo;

    return Output;
}


//QCoffeeSoldPositionInfo

QVector<QCoffeeSoldPositionInfo> QCoffeeServerPlugin::getListSoldPosition()
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

QVector<QCoffeeSoldPositionInfo> QCoffeeServerPlugin::getSoldPositionsForReceipt(int idReceipt)
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
        qDebug()<<"Error load list sold position for Receipt:"<<queryListSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryListSoldPosition;

    return Output;
}

QCoffeeSoldPositionInfo QCoffeeServerPlugin::getSoldPosition(int idSoldPosition)
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

int QCoffeeServerPlugin::getIdSolfPosition (QCoffeeSoldPositionInfo &soldPosition)
{
    int Output = -1;

    QString textQuery = "select id_soldPosition from tbl_soldPosition where "
                        "soldPosition_id_user = '" + QString::number(soldPosition.idUser) + "' "
                        "and count_soldPosition = '" + QString::number(soldPosition.count) + "' "
                        "and soldPosition_id_drink = '" + QString::number(soldPosition.idDrink) + "' "
                        "and soldPosition_id_typeDrink = '" + QString::number(soldPosition.idTypeDrink) + "' "
                        "and soldPosition_id_price = '" + QString::number(soldPosition.idPrice) + "' "
                        "and soldPosition_id_volumeDrink = '" + QString::number(soldPosition.idVolumeDrink) + "' "
                        "and soldPosition_id_receipt = '" + QString::number(soldPosition.idReceipt) + "' "
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

int QCoffeeServerPlugin::addSoldPosition(QCoffeeSoldPositionInfo &soldPosition)
{
    int Output = -1;

    int nextId = getNextId("tbl_soldPosition","id_soldPosition");
    QString textQuery = "insert into tbl_soldPosition ("
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
                        "'" + QString::number(nextId) + "',"
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

    bool ok = false;
    QSqlQuery *queryAddSoldPosition = execQuery(textQuery,&ok);
    if (ok) {
        Output = getIdSolfPosition(soldPosition);

    } else {
        qDebug()<<"Error add sold position:"<<queryAddSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddSoldPosition;

    return Output;
}

bool QCoffeeServerPlugin::editSoldPosition(QCoffeeSoldPositionInfo &soldPosition)
{
    bool Output = false;


    QString textQuery = "update tbl_soldPosition set "
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
                        " where id_soldPosition = '" + QString::number(soldPosition.id) + "';";

    QSqlQuery *queryEditSoldPosition = execQuery(textQuery,&Output);

    if (!Output) {
        qDebug()<<"Error edit sold position:"<<queryEditSoldPosition->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditSoldPosition;

    return Output;
}

void QCoffeeServerPlugin::deleteSolfPosition (int idSoldPosition)
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
QVector<QCoffeeShiftInfo> QCoffeeServerPlugin::getListShift()
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

QCoffeeShiftInfo QCoffeeServerPlugin::getShiftInfo(int idShift)
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

QCoffeeShiftInfo QCoffeeServerPlugin::getShiftInfo(int idUser, int idPointSale)
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

QCoffeeShiftInfo QCoffeeServerPlugin::getOpenShift(int idUser,int idPointSale)
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

int QCoffeeServerPlugin::getIdShift(QCoffeeShiftInfo &shiftInfo)
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

int QCoffeeServerPlugin::addShift(QCoffeeShiftInfo &shiftInfo)
{
    int Output = -1;

    int nextId = getNextId("tbl_shift","id_shift");
    QString textQuery = "insert into tbl_shift (id_shift,"
                                               "openTime_shift,"
                                               "closeTime_shift,"
                                               "open_shift,"
                                               "close_shift,"
                                               "shift_id_user,"
                                               "shift_id_pointSale) "
                        "values ('" + QString::number(nextId) + "',"
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

bool QCoffeeServerPlugin::editShift(QCoffeeShiftInfo &shiftInfo)
{
    bool Output;

    QString textQuery = "update tbl_shift set "
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

bool QCoffeeServerPlugin::checkOpenShift(int idUser,int idPointSale)
{
    bool Output;

    QCoffeeShiftInfo currentOpenShift = getOpenShift(idUser,idPointSale);

    if (currentOpenShift.id != -1)
        Output = true;
            else Output = false;

    return Output;
}

QCoffeeShiftInfo QCoffeeServerPlugin::openShift(int idUser,int idPointSale, bool *newShift)
{
    QCoffeeShiftInfo Output;

    if (checkOpenShift(idUser,idPointSale)) {
        //Если открытая смена существует
        Output = getOpenShift(idUser,idPointSale);
        if (newShift != NULL)
            *newShift = false;

    } else {
        //Если открытой смены нет
        Output.openTime = QDateTime::currentDateTime();
        Output.open = true;
        Output.closeTime = QDateTime::currentDateTime();
        Output.idUser = idUser;
        Output.idPointSale = idPointSale;
        Output.close = false;

        Output.id = addShift(Output);

        if (newShift != NULL)
            *newShift = true;
    }

    return Output;
}

bool QCoffeeServerPlugin::closeShift(QCoffeeShiftInfo &shift)
{
    bool Output = false;

    if (checkOpenShift(shift.idUser,shift.idPointSale)) {
        //Если открытая смена существует

        shift.closeTime = QDateTime::currentDateTime();
        shift.close = true;

        editShift(shift);
        Output = true;

    } else {
        Output = false;
        qDebug()<<"Error close shift: open shit not found! shift info "<<shift.toString();
    }

    return Output;
}

//QCoffeePictureInfo
QCoffeePictureInfo QCoffeeServerPlugin::getPicture (int idPicture)
{
    QCoffeePictureInfo Output;

    QString textQuery = "select id_pictureDrink,"
                        "name_pictureDrink,"
                        "data_pictureDrink"
                        " from tbl_pictureDrink "
                        "where id_pictureDrink = '" + QString::number(idPicture) + "';";

    bool result;
    QSqlQuery *queryPcture = execQuery(textQuery,&result);

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

int QCoffeeServerPlugin::addPicture (QCoffeePictureInfo &picture)
{
    int Output = -1;

    int nextId = getNextId("tbl_pictureDrink","id_pictureDrink");
    QString textQuery =  "insert into tbl_pictureDrink ("
                         "id_pictureDrink,"
                         "name_pictureDrink,"
                         "data_pictureDrink"
                         ") values ("
                         "'" + QString::number(nextId) + "',"
                         "'" + picture.name + "',"
                         ":imageData);";

    bool result;
    QSqlQuery *queryAddPicture = execQuery(textQuery,":imageData", picture.data,&result);

    if (result){
        Output = nextId;
    } else {
        qDebug()<<"Error add picture:"<<queryAddPicture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryAddPicture;

    return Output;
}

int QCoffeeServerPlugin::getIdPicture(QCoffeePictureInfo &picture)
{
    int Output = -1;

    QString textQuery = "select id_pictureDrink"
                        " from tbl_pictureDrink "
                        "where "
                        "and name_pictureDrink = '" + picture.name + "' "
                        "and data_pictureDrink = :imageData;";



    bool result;
    QSqlQuery *queryPcture = execQuery(textQuery,":imageData",picture.data,&result);

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

void QCoffeeServerPlugin::editPicture(QCoffeePictureInfo &picture)
{
    QString textQuery = "update tbl_pictureDrink set "
                        "name_pictureDrink = '" + picture.name + "',"
                        "data_pictureDrink = :imageData;";

    bool result;
    QSqlQuery *queryEditPicture = execQuery(textQuery,":imageData",picture.data,&result);

    if (!result) {
        qDebug()<<"Error edit picture:"<<queryEditPicture->lastError().text();
        qDebug()<<"textQuery = "<<textQuery;
    }

    delete queryEditPicture;
}

//Commands

void QCoffeeServerPlugin::command01 (QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);

    int currentPointId;
    streamIn >> currentPointId;

    debugMessage("command01: current id " + QString::number(currentPointId));

    QCoffeePointSale currentPointSale = getPointSaleInfo(currentPointId);

    Output = currentPointSale.toByteArray();

    sendExtData(0x01,Output,client);

}

void QCoffeeServerPlugin::command02 (QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    quint16 count;
    QVector<int> idCategories;
    int currentIdCategory;

    streamIn>>count;
    debugMessage("command02: count " + QString::number(count));

    while (!streamIn.atEnd()) {
        streamIn>>currentIdCategory;
        idCategories.push_back(currentIdCategory);
    }

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeeCategoryInfo currentCategory;
    for (int i=0;i<idCategories.length();i++) {
        currentCategory = getCategoryInfo(idCategories.at(i));
        currentCategory >> streamOut;
    }

    sendExtData(0x02,Output,client);
}

void QCoffeeServerPlugin::command03(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    quint16 count;
    streamIn>>count;

    debugMessage("command03: count " + QString::number(count));

    QVector<int> idDrinks;
    int currentIdDrink;

    while (!streamIn.atEnd()) {
        streamIn>>currentIdDrink;
        idDrinks.push_back(currentIdDrink);
    }

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut <<count;
    QCoffeeDrinkInfo currentDrink;
    for (int i=0;i<idDrinks.length();i++) {
        currentDrink = getDrinkInfo(idDrinks.at(i));
        currentDrink >> streamOut;
    }

    sendExtData(0x03,Output,client);
}

void QCoffeeServerPlugin::command04(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    quint16 count;
    streamIn>>count;

    debugMessage("command04: count " + QString::number(count));

    QVector<int> idDrinks;
    int currentIdDrink;

    while (!streamIn.atEnd()) {
        streamIn>>currentIdDrink;
        idDrinks.push_back(currentIdDrink);
    }

    QVector<int> volumeId;

    for (int i=0;i<idDrinks.length();i++) {
        QVector<int> buf = getIdVolumeForDrink(idDrinks.at(i));

        for (int j=0;j<buf.length();j++)
            if(volumeId.indexOf(buf.at(j))<0)
                volumeId.push_back(buf.at(j));
    }


    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut <<(quint16)volumeId.length();

    QCoffeeVolumeDrinkInfo currentVolume;
    for (int i=0;i<volumeId.length();i++) {
        currentVolume = getVolumeInfo(volumeId.at(i));
        currentVolume >> streamOut;
    }

    sendExtData(0x04,Output,client);
}

void QCoffeeServerPlugin::command05(QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(data)

    QByteArray Output;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeeTypeDrink currentType;
    QVector<QCoffeeTypeDrink> typesDrinks;
    typesDrinks = getListTypeDrink();
    streamOut <<(quint16)typesDrinks.length();

    for (int i=0;i<typesDrinks.length();i++) {
        currentType = typesDrinks.at(i);
        currentType >> streamOut;
    }

    sendExtData(0x05,Output,client);
}

void QCoffeeServerPlugin::command06(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);
    int idPointSale;
    streamIn>>idPointSale;

    debugMessage("command06: id point sale " + QString::number(idPointSale));

    QVector<QCoffeePriceInfo> priceItems;
    priceItems = getListPricePointSale(idPointSale);

    QDataStream streamOut (&Output,QIODevice::ReadWrite);
    streamOut <<(quint16)priceItems.length();

    QCoffeePriceInfo currentPrice;
    for (int i=0;i<priceItems.length();i++) {
        currentPrice = priceItems.at(i);
        currentPrice >> streamOut;
    }

    sendExtData(0x06,Output,client);
}

void QCoffeeServerPlugin::command07(QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(data)

    QString currentLogin = client->getUnAccountInfo()->login;
    debugMessage("command07: currentLogin " + currentLogin);

    QCoffeeUserInfo currentUser = getUserInfo(currentLogin);

    QByteArray Output;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);
    currentUser >> streamOut;

    sendExtData(0x07,Output,client);
}

void QCoffeeServerPlugin::command08(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command08: Open shift ");
    int currentIdUser;
    int currentIdPointSale;

    streamIn >> currentIdPointSale;
    currentIdUser = getUserInfo(client->getCurrentAccountInfo()->login).id;

    bool newShift;

    QCoffeeShiftInfo currentShift = openShift(currentIdUser,currentIdPointSale,&newShift);


    QDataStream streamOut (&Output,QIODevice::ReadWrite);
    streamOut <<newShift;
    currentShift >> streamOut;

    sendExtData(0x08,Output,client);
}

void QCoffeeServerPlugin::command09(QByteArray data, QUnClientHandler *client)
{
    debugMessage("command09: Close shift ");
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeShiftInfo currentShift;
    currentShift << streamIn;

    bool result = closeShift(currentShift);

    QDataStream streamOut (&Output,QIODevice::ReadWrite);
    streamOut << result;
    currentShift >> streamOut;

    sendExtData(0x09,Output,client);
}

void QCoffeeServerPlugin::command0A(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeShiftInfo currentShift;
    currentShift << streamIn;

    QVector<QCoffeeReceiptInfo> receipt = getListReceiptInfo(currentShift.id);

    bool result = true;

    QDataStream streamOut (&Output,QIODevice::ReadWrite);
    streamOut << result;
    streamOut << (quint16)receipt.length();

    QCoffeeReceiptInfo currentReceipt;
    for (int i=0;i<receipt.length();i++) {
        currentReceipt = receipt.at(i);
        currentReceipt >> streamOut;
    }

    sendExtData(0x0A,Output,client);
}

void QCoffeeServerPlugin::command0B(QByteArray data, QUnClientHandler *client)
{
    debugMessage("command09: get sold position ");
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    quint16 count;
    streamIn >>count;

    QVector<int> idReceipt;
    int currentId;

    while (!streamIn.atEnd()) {
        streamIn >> currentId;
        idReceipt.push_back(currentId);
    }


    QVector<QCoffeeSoldPositionInfo> soldPositions;
    QVector <QCoffeeSoldPositionInfo> currentSoldPositions;
    QCoffeeSoldPositionInfo currentSoldPosition;
    for (int i=0;i<idReceipt.length();i++) {
        currentSoldPositions = getSoldPositionsForReceipt(idReceipt.at(i));

        for (int j=0;j<currentSoldPositions.length();j++)
            soldPositions.push_back(currentSoldPositions.at(j));
    }


    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut << (quint16)soldPositions.length();

    for (int i=0;i<soldPositions.length();i++) {
        currentSoldPosition = soldPositions.at(i);
        currentSoldPosition >> streamOut;
    }

    sendExtData(0x0B,Output,client);
}

void QCoffeeServerPlugin::command0C(QByteArray data, QUnClientHandler *client)
{
    debugMessage("command0C: Recive sale ");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeeReceiptInfo currentReceipt;
    currentReceipt << streamIn;

    int idcurrentReceipt = getIdReceiptInfo(currentReceipt);
    if (idcurrentReceipt<=0) {
        idcurrentReceipt = addReceiptInfo(currentReceipt);
    } else {
        editReceiptInfo(currentReceipt);
    }
    currentReceipt.id = idcurrentReceipt;

    quint16 count;
    streamIn>>count;

    int idCurrentSoldPositions;
    QCoffeeSoldPositionInfo currentSoldPosition;
    QVector <QCoffeeSoldPositionInfo> currentSoldPositions;

    while (!streamIn.atEnd()) {
        currentSoldPosition << streamIn;
        currentSoldPosition.idReceipt = currentReceipt.id;
        idCurrentSoldPositions = addSoldPosition(currentSoldPosition);
        currentSoldPosition.id = idCurrentSoldPositions;

        currentSoldPositions.push_back(currentSoldPosition);
    }

    QByteArray Output;
    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut << result;
    currentReceipt >> streamOut;
    streamOut << (quint16)currentSoldPositions.length();

    for (int i=0;i<currentSoldPositions.length();i++) {
        currentSoldPosition = currentSoldPositions.at(i);
        currentSoldPosition >> streamOut;
    }

    sendExtData(0x0C,Output,client);
}

void QCoffeeServerPlugin::command0D(QByteArray data, QUnClientHandler *client)
{
    debugMessage("command0D: upload picture ");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    QCoffeePictureInfo currentPicture;
    currentPicture << streamIn;

    if (currentPicture.id <=0) {
        addPicture(currentPicture);
    } else {
        editPicture(currentPicture);
    }

    QByteArray Output;
    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut << result;
    currentPicture >> streamOut;

    sendExtData(0x0D,Output,client);
}

void QCoffeeServerPlugin::command0E(QByteArray data, QUnClientHandler *client)
{
    debugMessage("command0E: Get picture info ");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    int currentIdPicture;
    streamIn >> currentIdPicture;

    QCoffeePictureInfo currentPicture = getPicture(currentIdPicture);

    QByteArray Output;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    currentPicture >> streamOut;

    sendExtData(0x0E,Output,client);
}

void QCoffeeServerPlugin::command0F(QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(data)

    QByteArray Output;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    debugMessage("command0F: get id for all point sales");

    QVector<QCoffeePointSale> pointSaleList;
    QVector<int> idPointSales;
    pointSaleList = getListPointSale();
    for (int i=0;i<pointSaleList.count();i++)
    {
        if (pointSaleList.at(i).id!=255)
        {
            idPointSales.push_back(pointSaleList.at(i).id);
        }
    }
    streamOut << idPointSales;

    sendExtData(0x0F,Output,client);
}

void QCoffeeServerPlugin::command10(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);

    int currentPointId;
    streamIn >> currentPointId;

    debugMessage("command10: current id " + QString::number(currentPointId));

    QCoffeePointSale currentPointSale = getPointSaleInfo(currentPointId);

    Output = currentPointSale.toByteArray();

    sendExtData(0x10,Output,client);
}

void QCoffeeServerPlugin::command11(QByteArray data, QUnClientHandler *client)
{
    Q_UNUSED(data)

    QByteArray Output;

    debugMessage("command11: get category");

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QVector<QCoffeeCategoryInfo> listCategory = getListCategories();
    QCoffeeCategoryInfo currentCategory;
    for (int i=0;i<listCategory.length();i++) {
        currentCategory = listCategory.at(i);
        currentCategory >> streamOut;
    }

    sendExtData(0x11,Output,client);
}

void QCoffeeServerPlugin::command12(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command12: get all drinks");

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QVector<QCoffeeDrinkInfo> listDrink = getListDrink();
    QCoffeeDrinkInfo currentDrink;
    for (int i=0;i<listDrink.length();i++) {
        currentDrink = listDrink.at(i);
        currentDrink >> streamOut;
    }

    sendExtData(0x12,Output,client);
}

void QCoffeeServerPlugin::command13(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command13: get all price list");

    QVector<QCoffeePriceInfo> priceItems;
    priceItems = getListPriceInfo();

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeePriceInfo currentPrice;
    for (int i=0;i<priceItems.length();i++) {
        currentPrice = priceItems.at(i);
        currentPrice >> streamOut;
    }

    sendExtData(0x13,Output,client);
}

void QCoffeeServerPlugin::command14(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command14: get all shift list");

    QVector<QCoffeeShiftInfo> shiftInfo;
    shiftInfo = getListShift();

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeeShiftInfo currentShift;
    for (int i=0;i<shiftInfo.length();i++) {
        currentShift = shiftInfo.at(i);
        currentShift >> streamOut;
    }

    sendExtData(0x14,Output,client);
}

void QCoffeeServerPlugin::command15(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command15: get all receipt list");

    QVector<QCoffeeReceiptInfo> receiptItems;
    receiptItems = getListReceiptInfo();

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeeReceiptInfo currentReceipt;
    for (int i=0;i<receiptItems.length();i++) {
        currentReceipt = receiptItems.at(i);
        currentReceipt >> streamOut;
    }

    sendExtData(0x15,Output,client);
}

void QCoffeeServerPlugin::command16(QByteArray data, QUnClientHandler *client)
{
    QByteArray Output;
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    debugMessage("command16: get all sold position list");

    QVector<QCoffeeSoldPositionInfo> soldPositionItems;
    soldPositionItems = getListSoldPosition();

    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    QCoffeeSoldPositionInfo currenSoldPosition;
    for (int i=0;i<soldPositionItems.length();i++) {
        currenSoldPosition = soldPositionItems.at(i);
        currenSoldPosition >> streamOut;
    }

    sendExtData(0x16,Output,client);
}

void QCoffeeServerPlugin::commandA0(QByteArray data, QUnClientHandler *client)
{
    debugMessage("CommandA0: Received drink information");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    quint32 code;
    streamIn >> code;

    QCoffeeDrinkInfo currentDrink;
    currentDrink << streamIn;

    switch (code) {
        case 0x01: currentDrink.id = addDrinkInfo(currentDrink);
            break;
        case 0x02: editDrinkInfo(currentDrink);
            break;
        case 0x03: deleteDrinkInfo(currentDrink);
            break;
    }

    QByteArray Output;
    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut << result;
    streamOut << code;
    currentDrink >> streamOut;

    sendExtData(0xA0,Output,client);
}

void QCoffeeServerPlugin::command17(QByteArray data, QUnClientHandler *client) {
    QDataStream streamIn(&data, QIODevice::ReadOnly);

    qint32 id;
    qint32 idWg;
    QDate startDate, endDate;

    qDebug()<<"command17NN";
    streamIn >> id;
    streamIn >> startDate;
    streamIn >> endDate;
    streamIn >> idWg;
    QList<QDate> date = {startDate, endDate};


    QMap<QDateTime, float> predictionResults = prediction(&date, &id);


    QByteArray Output;
    QDataStream streamOut(&Output, QIODevice::WriteOnly);

    streamOut << predictionResults;
    streamOut << idWg;
    streamOut << id;

    sendExtData(0x17, Output, client);
}

#include "QNnPredictionInfo/qnnpredictioninfo.h"
void QCoffeeServerPlugin::command18(QByteArray data, QUnClientHandler *client) {

    QThread *thread = new QThread;
    Command18Worker *worker = new Command18Worker(data);
    worker->moveToThread(thread);


    connect(thread, &QThread::started, worker, &Command18Worker::process);
    connect(worker, &Command18Worker::finished, this, [this, client, thread, worker](QByteArray result) {
        sendExtData(0x18, result, client);

        thread->quit();
        thread->wait();
        worker->deleteLater();
        thread->deleteLater();
    });


    thread->start();
}


void QCoffeeServerPlugin::command19(QByteArray data, QUnClientHandler *client) {

    debugMessage("");
    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);

    quint32 code;
    streamIn >> code;
    QVector<qint32> newDrinks;
    streamIn >> newDrinks;
    QCoffeeCategoryInfo currentCategory;
    currentCategory << streamIn;


    if (code!=0x03) {
        unlinkCategoryAndDrink(currentCategory.id);
    }

    switch (code) {
    case 0x01: currentCategory.id = addCategory(currentCategory);
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
    QByteArray Output;
    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);

    streamOut << result;
    streamOut << code;
    streamOut << newDrinks;
    currentCategory >> streamOut;

    sendExtData(0x19,Output,client);

}


void QCoffeeServerPlugin::command20(QByteArray data,QUnClientHandler *client) {


    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);;






    quint32 code;
    streamIn >> code;

    QCoffeeVolumeDrinkInfo currentVolume;
    currentVolume << streamIn;

    switch (code) {
    case 0x01: currentVolume.id = addVolumeDrink(currentVolume);
        break;
    case 0x02: //editCategory(currentCategory);
        break;
    case 0x03: //deleteCategoryInfo(currentCategory);
        break;
    }

    QByteArray Output;

    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);


    streamOut << result;
    streamOut << code;
    currentVolume >> streamOut;

     sendExtData(0x20,Output,client);


}


void QCoffeeServerPlugin::command21(QByteArray data,QUnClientHandler *client) {


    QDataStream streamIn (&data,QIODevice::ReadOnly);
    streamIn.device()->seek(0);;






    quint32 code;
    streamIn >> code;

    QCoffeePriceInfo currentPrice;
    currentPrice << streamIn;

    switch (code) {
    case 0x01: currentPrice.id = addPriceInfo(currentPrice);
        break;
    case 0x02: //editCategory(currentCategory);
        break;
    case 0x03: //deleteCategoryInfo(currentCategory);
        break;
    }

    QByteArray Output;

    bool result = true;
    QDataStream streamOut (&Output,QIODevice::ReadWrite);


    streamOut << result;
    streamOut << code;
    currentPrice >> streamOut;

    sendExtData(0x21,Output,client);


}
