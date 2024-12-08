#ifndef MSQL_H
#define MSQL_H
#include<QtSql>
#include<iostream>
#include<QSqlDatabase>
#include<QSqlQuery>
#include "QWeather/weather.h"
#include"../../../../Common/QPlugins/QCoffeePlugin/qcoffeeserverplugin.h"
class Data;
class DB {
    private:
        QSqlDatabase *db;


     public:

        QCoffeeServerPlugin *currentPlugin;

        QList<qint32> idProduct;
        QList<QString> dateProductSale;
        QList<Data> dataNn;
        DB(QCoffeeServerPlugin *plugin_);
        ~DB();

        void getIdProduct();
        void getDateProductSale();
        QString getProductName(qint32 *id);


};





class Data {
public:


    QDate date; // дата
    QList<qint32> prod; //айдишки продуктов проданных за эту дату



    Data(QString *dateStr);
    ~Data();



};


QList<Data> getDateFromDb(DB *dbase); //получение всей инфы

QMap<QDateTime,qint32> getNumSalesProd(qint32 *prodId,QList<Data> *dataFromDb, QList<QDate> *endDAte); //расчет числа продаж за дату
bool dateCheker(const QDate *date, QList<QDate> *enddate);

void importFullInfo(QList<Data> *data); //импорт из файлов

#endif // MSQL_H
