#include "interlayerforqml.h"
#include <QFile>
#include <QStringList>
#include <QDebug>


InterlayerForQML::InterlayerForQML(QObject *parent) : QObject(parent)
{
      drinkImageProvider = new DrinkImageProvider(QQmlImageProviderBase::Image,this);
      settings = new QSettings();
      client = new QUnNetProtokolCLient("CoffeeShine",settings,this);
      qmlProperty = new QQmlPropertyMap(this);
      getLastLoginFromQSettings_Slot();
}

bool InterlayerForQML::getOSAndroidOrNot()
{

#ifdef Q_OS_ANDROID
     return true;
#else
    return false;
#endif

}

void InterlayerForQML::getImageFromDB_Slot(int id)
{
     QImage image;
     QCoffeePictureInfo imageInfo = currentPlugin->getPicture(currentPlugin->getDrinkInfo(id).idPicture);
     image.loadFromData(imageInfo.data);

     emit sendImageFromDB_Signal(image);
}

void InterlayerForQML::getWarnings(QList<QQmlError> err)
{
    qDebug() << err;
}

void InterlayerForQML::fillingListTabs_Slot()
{
    QVector<QCoffeeCategoryInfo> categoryList = currentPlugin->getListCategories();

    qDebug() << "categoryListlength: " << categoryList.length();
    for(int i=0; i<categoryList.length(); i++)
    {
        QColorConsole categoryColor = categoryList.at(i).color;
        QColor color;
        color.setRgb(categoryColor.red(),categoryColor.green(),categoryColor.blue());

        qmlProperty->insert("tabsName", QVariant(categoryList.at(i).name));
        qmlProperty->insert("idCategory", QVariant(categoryList.at(i).id));
        qmlProperty->insert("colorForCategory", QVariant(color.name()));
        emit createTab_Signal();
    }
}

void InterlayerForQML::fillingListMenu_Slot(int index)
{
    QVector<QCoffeeDrinkInfo> drinkList = currentPlugin->getListDrink(index);

    for(int i=0; i<drinkList.length(); i++)
    {
        QColorConsole drinkColor = drinkList.at(i).color;
        QColor color;
        color.setRgb(drinkColor.red(),drinkColor.green(),drinkColor.blue());

        qmlProperty->insert("name", QVariant(drinkList.at(i).name));
        qmlProperty->insert("idDrink", QVariant(drinkList.at(i).id));
        qmlProperty->insert("colorCode", QVariant(color.name()));
        qmlProperty->insert("idCategory", QVariant(index));
        emit createMenu_Signal();
    }
}

void InterlayerForQML::fillingListVolume_Slot(int idDrink)
{
    QVector<QCoffeeVolumeDrinkInfo> volumeList = currentPlugin->getListVolume(idDrink);

    for(int i=0; i<volumeList.length(); i++)
    {
        qmlProperty->insert("nameVolume", QVariant(volumeList.at(i).name));
        qmlProperty->insert("idVolume", QVariant(volumeList.at(i).id));
        qmlProperty->insert("volume", QVariant(volumeList.at(i).volume));
        qmlProperty->insert("volumeUnits", QVariant(volumeList.at(i).units));
        emit createVolume_Signal();
    }
}

void InterlayerForQML::fillingListVarietyOfPay_Slot()
{
    QVector<QCoffeeTypeDrink> varietyOfPayList = currentPlugin->getListTypeDrink();

    for(int i=0; i<varietyOfPayList.length(); i++)
    {
        qmlProperty->insert("nameVarietyOfPay", QVariant(varietyOfPayList.at(i).name));
        qmlProperty->insert("idVarietyOfPay", QVariant(varietyOfPayList.at(i).id));
        emit createVarietyOfPay_Signal();
    }

}

void InterlayerForQML::fillingReceiptList_Slot()
{
    QVector<QCoffeeReceiptInfo> listReceipt = currentPlugin->getListReceiptInfoForShift(currentPlugin->getCurrentShift().id);
    int nal=0;    double sumNal=0;
    int bezNal=0; double sumBezNal=0;
    int free=0;

    for (int i=0;i<listReceipt.count();i++)
    {
        QVector<QCoffeeSoldPositionInfo> listSoldPos =  currentPlugin->getListSoldPositionForReceipt(listReceipt.at(i).id);
        for (int j=0; j<listSoldPos.count();j++)
        {
            QString nameTypeCurrentDrink = currentPlugin->getTypeDrinkInfo(listSoldPos.at(j).idTypeDrink).name;

            if (nameTypeCurrentDrink == "Наличные")
            {
                nal++;
                sumNal+= currentPlugin->getPriceInfo(listSoldPos.at(j).idPrice).value;
            }
            else if (nameTypeCurrentDrink == "Безналичные")
            {
                bezNal++;
                sumBezNal += currentPlugin->getPriceInfo(listSoldPos.at(j).idPrice).value;
            }
            else if(nameTypeCurrentDrink == "Бесплатный")
            {
                free++;
            }
        }

        qmlProperty->insert("countReceipt", QVariant(i+1));
        qmlProperty->insert("countDrinksString", QVariant("Товаров: " + QString::number(listSoldPos.count()) + " шт."));
        qmlProperty->insert("sumReceiptString", QVariant("Сумма: " + QString::number(listReceipt.at(i).sum) + " руб."));
        qmlProperty->insert("dateReceiptString", QVariant(listReceipt.at(i).dateTime.toString("d MMM в hh:mm:ss")));
        qmlProperty->insert("idReceipt", QVariant(listReceipt.at(i).id));
        emit createReceipt_Signal();
    }

    qmlProperty->insert("nalString", QVariant("За наличные: " + QString::number(nal) + " шт. на сумму " + QString::number(sumNal) + " руб."));
    qmlProperty->insert("bezNalString", QVariant("За безналичные: " + QString::number(bezNal) + " шт. на сумму " + QString::number(sumBezNal) + " руб."));
    qmlProperty->insert("freeString", QVariant("Бесплатные "+ QString::number(free) + " шт."));
    emit fillingSomePropertyInReportView_Signal();

}

void InterlayerForQML::fillingSoldPositionList_Slot(int idReceipt)
{
    QVector<QCoffeeSoldPositionInfo> listSoldPosition = currentPlugin->getListSoldPositionForReceipt(idReceipt);

    for (int i=0;i<listSoldPosition.count();i++)
    {
        QCoffeeDrinkInfo drinkInfo = currentPlugin->getDrinkInfo(listSoldPosition.at(i).idDrink);

        QColorConsole drinkColor = drinkInfo.color;
        QColor color;
        color.setRgb(drinkColor.red(),drinkColor.green(),drinkColor.blue());

        QString nameTypeCurrentDrink = currentPlugin->getTypeDrinkInfo(listSoldPosition.at(i).idTypeDrink).name;
        QString priceString;

        if (nameTypeCurrentDrink == "Наличные")
        {
            priceString = QString::number(currentPlugin->getPriceInfo(listSoldPosition.at(i).idPrice).value) + " руб.\nНаличные";
        }
        else if (nameTypeCurrentDrink == "Безналичные")
        {
            priceString = QString::number(currentPlugin->getPriceInfo(listSoldPosition.at(i).idPrice).value) + " руб.\nБезналичные";
        }
        else if(nameTypeCurrentDrink == "Бесплатный")
        {
            priceString = "\nБесплатный";
        }

        QCoffeeVolumeDrinkInfo volumeInfo = currentPlugin->getVolumeInfo(listSoldPosition.at(i).idVolumeDrink);

        qmlProperty->insert("categoryString",QVariant(currentPlugin->getCategoryInfo(currentPlugin->getPriceInfo(listSoldPosition.at(i).idPrice).idDrinkCategory).name));
        qmlProperty->insert("volumeString",QVariant(volumeInfo.name + "\n" +QString::number(volumeInfo.volume) +" "+ volumeInfo.units));
        qmlProperty->insert("priceString",QVariant(priceString));
        qmlProperty->insert("nameDrinkString", QVariant(QString::number(i+1) + ") " + drinkInfo.name));
        qmlProperty->insert("colorCode", QVariant(color.name()));
        qmlProperty->insert("idDrink", QVariant(drinkInfo.id));

        emit createSoldPosition_Signal();
    }
}

void InterlayerForQML::createReceipt_Slot(int countSoldPositions)
{
    QVector<QCoffeeSoldPositionInfo> soldPositions;
    int currentIdUser = currentPlugin->getCurrentShift().idUser;
    QString totalSum = qmlProperty->value("rubels").toString() + "." + qmlProperty->value("coins").toString();
    qDebug()<<"rubels: " <<qmlProperty->value("rubels").toString();
    qDebug()<<"coins: " <<qmlProperty->value("coins").toString();


    for(int i=0;i<countSoldPositions;i++)
    {
        QCoffeeSoldPositionInfo newSoldPosition;
        emit getSomePropertyForSoldPosition_Signal(i);
        newSoldPosition.idUser = currentIdUser;
        newSoldPosition.count = 1;
        newSoldPosition.idDrink = qmlProperty->value("idDrinkForSoldPosition").toInt();
        newSoldPosition.idTypeDrink = qmlProperty->value("idTypeDrinkForSoldPosition").toInt();
        newSoldPosition.idPrice  = qmlProperty->value("idPriceForSoldPosition").toInt();
        newSoldPosition.idVolumeDrink =  qmlProperty->value("idVolumeForSoldPosition").toInt();
        newSoldPosition.numberInReceipt = i+1;

        soldPositions.append(newSoldPosition);
    }

    QCoffeeReceiptInfo newReceipt;
    int currentIdShift = currentPlugin->getCurrentShift().id;

    qDebug()<<"count elements: "<<soldPositions.length();

    newReceipt.idShift = currentIdShift;
    newReceipt.dateTime = QDateTime::currentDateTimeUtc();
    newReceipt.sum = totalSum.toDouble();

    qDebug()<<"receiptCreate";
    currentPlugin->sale(newReceipt,soldPositions);
}

bool InterlayerForQML::getPrice(int idCategory, int idDrink, int idVolume)
{
    QCoffeePriceInfo currentPriceInfo = currentPlugin->getPricceInfo(idCategory, idDrink, idVolume);
    qmlProperty->insert("idPrice", QVariant(currentPriceInfo.id));

    QList<QString> list;
    for (QString item : QString::number(currentPriceInfo.value, 'g', 2).split("."))
    {
        list.append(item);
    }
    qmlProperty->insert("rubels", QVariant(list[0]));

    if (list.count()>1)
    {
        if (list[1].length() == 1)
           list[1]+='0';

        qmlProperty->insert("coins", QVariant(list[1]));
        return true;
    }
    return false;
}

void InterlayerForQML::saveIdPointSaleInQSettings_Slot()
{
    if (qmlProperty->value("idPointSaleInOptions") != 0)
    {
        settings->setValue("idPointSaleInOptions", qmlProperty->value("idPointSaleInOptions"));
    }
    else
    {
        settings->setValue("idPointSaleInOptions", QVariant("255"));
    }
}

void InterlayerForQML::getIdPointSaleFromQSettings_Slot()
{
    if (settings->contains("idPointSaleInOptions"))
    {
        qmlProperty->insert("idPointSaleInOptions", settings->value("idPointSaleInOptions"));
    }
    else
    {
        qmlProperty->insert("idPointSaleInOptions", QVariant(255));
    }
}

void InterlayerForQML::connectToHost_Slot()
{
    client = new QUnNetProtokolCLient("CoffeeShine",settings,this);

    connect(drinkImageProvider,SIGNAL(signalGetImageFromDB(int)),this,SLOT(getImageFromDB_Slot(int)));
    connect(this,SIGNAL(sendImageFromDB_Signal(QImage)),drinkImageProvider,SLOT(slotGetImageFromDB(QImage)));
    connect(client,SIGNAL(signalConnected()),this,SLOT(slotConnected()));
    connect(client,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()));
    connect(client,SIGNAL(signalAuthorized()),this,SLOT(slotAuthorized()));
    connect(client,SIGNAL(signalUserInfoReceived()),this,SLOT(slotUserInfoReceived()));
    connect(client,SIGNAL(signalError(UnErr)),this,SLOT(slotError(UnErr)));

    qDebug()<< qmlProperty->value("lastLogin") << qmlProperty->value("password");

    client->setAccount(qmlProperty->value("lastLogin").toString(),qmlProperty->value("password").toString());
    qDebug()<<"connect:"<<client->connectToHost();

}

void InterlayerForQML::disconnectFromHost_Slot()
{
    currentPlugin->closeShift();

    client->disconnectFromHost();  
    qDebug()<<"disconnect from host";
}

void InterlayerForQML::exitApp()
{
    qmlProperty->deleteLater();
    delete client;
    qGuiApp->quit();
}

void InterlayerForQML::saveLastLoginInQSettings_Slot()
{
    settings->beginGroup("Login");
    settings->setValue("lastLogin", qmlProperty->value("lastLogin"));
    settings->endGroup();
}

void InterlayerForQML::getLastLoginFromQSettings_Slot()
{
    settings->beginGroup("Login");
    if (settings->value("lastLogin") == "")
    {
        qmlProperty->insert("lastLogin", QVariant("admin"));
    }
    else
    {
        qmlProperty->insert("lastLogin", settings->value("lastLogin").toString());
    }

    settings->endGroup();
}

void InterlayerForQML::saveHostAndPortInInterlayer_Slot()
{
    HostInfo newHost;
    newHost.address = qmlProperty->value("host").toString();

    if ((qmlProperty->value("port").toInt() <0) ||
            (qmlProperty->value("port").toInt() >65535) ||
                (!qmlProperty->value("port").toInt()))
    {
        emit somePortNotValid_Signal();
        vectorForSaveHosts.clear();
        return;
    }

    newHost.port = qmlProperty->value("port").toInt();
    vectorForSaveHosts.append(newHost);
}


void InterlayerForQML::saveHostAndPortInQSettings_Slot()
{
    client->setHosts(vectorForSaveHosts);
    vectorForSaveHosts.clear();
}

void InterlayerForQML::fillingHostAndPortListFromQSettings_Slot()
{
     QVector<HostInfo> buf = client->hosts();

     for (int i=0;i<buf.length();i++) {
         qmlProperty->insert("host", buf.at(i).address);
         qmlProperty->insert("port", QString::number(buf.at(i).port));
         emit createHostAndPort_Signal();
     }
}

void InterlayerForQML::slotBeginSynchronization()
{
    qDebug()<<"начало синхронизации";
}

void InterlayerForQML::slotEndSynchronization()
{
    qDebug()<<"окончание синхронизации";
    fillingListTabs_Slot();

    QCoffeeShiftInfo currentShift = currentPlugin->getCurrentShift();
    currentShift.openTime.setTimeSpec(Qt::UTC);
    QDateTime localTime = currentShift.openTime.toLocalTime();

    QCoffeeUserInfo userInfo = currentPlugin->getUserInfo(currentShift.idUser);

    qmlProperty->insert("patronymicAndName",QVariant(userInfo.patronymic +" "+userInfo.name));
    qmlProperty->insert("openData", localTime.toString("d MMM в hh:mm:ss"));
    emit fillOpenDate_Signal();
    emit finishCreateTabs_Signal();
    emit goodAuthorized_Signal();
}

void InterlayerForQML::slotConnected()
{
    qDebug()<<"InterlayerForQML::slotConnected";
}

void InterlayerForQML::slotAuthorized()
{
    qDebug()<<"InterlayerForQML::slotAuthorized";
}

void InterlayerForQML::slotDisconnected()
{
    qDebug()<<"InterlayerForQML::slotDisconnected";

 //   deleteLater() client;
}

void InterlayerForQML::slotUserInfoReceived()
{
    currentPlugin = new QCoffeeClientPlugin(settings->value("idPointSaleInOptions").toInt(),client,0x06,this);

    connect(currentPlugin,SIGNAL(signalBeginSynchronization()),
            this,SLOT(slotBeginSynchronization()));
    connect(currentPlugin,SIGNAL(signalMessageSplashScreen(QString)),
            this,SIGNAL(sendMessageToSplashScreen_Signal(QString)));
    connect(currentPlugin,SIGNAL(signalEndSynchronization()),
            this,SLOT(slotEndSynchronization()));

    currentPlugin->startSynchronization();  

}

void InterlayerForQML::slotError(UnErr err)
{
    qDebug()<<"InterlayerForQML::slotError";

    switch (err) {
    case UnErr::errAuthLogin: {
        qmlProperty->insert("errorString", "Логин введён неправильно");
        qDebug()<<"Login Error";
    }
        break;
    case UnErr::errAuthPassword: {
        qDebug()<<"Password Error";
        qmlProperty->insert("errorString", "Пароль введён неправильно");
    }
        break;
    case UnErr::errAuthBlock: {
        qDebug()<<"User is blocked";
        qmlProperty->insert("errorString", "Этот аккаунт заблокирован");
    }
        break;
    case UnErr::errAuthServWork: {
        qDebug()<<"Server not work";
        qmlProperty->insert("errorString", "Сервер не работает");
    }
        break;
    case UnErr::errNoRights: {
        qDebug()<<"Not rights";
        qmlProperty->insert("errorString", "У вас нет прав");
    }
        break;
    case UnErr::errNoAuthorized: {
        qDebug()<<"No authorized";
        qmlProperty->insert("errorString", "Авторизация не прошла");
    }
        break;

    }

    emit badAuthorized_Signal();
}

