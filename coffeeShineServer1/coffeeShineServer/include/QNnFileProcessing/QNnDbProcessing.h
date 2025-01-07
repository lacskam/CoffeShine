#ifndef MSQL_H
#define MSQL_H
#include<QtSql>
#include<iostream>
#include<QSqlDatabase>
#include<QSqlQuery>
#include "QWeather/weather.h"
#include"../../../../Common/QPlugins/QCoffeePlugin/qcoffeeserverplugin.h"
#include"QDate"

class Data;
class DB {
    private:
        QSqlDatabase *db;
         QSqlDatabase dbForNn;


     public:



        QList<qint32> idProduct;
        QList<QString> dateProductSale;
        QList<Data> dataNn;
        DB(QSqlDatabase *dbase);
        ~DB();

        void getIdProduct();
        void getDateProductSale();
        QString getProductName(qint32 *id);


        QString getLastDateFromNn();

        void getDateProductSaleForDate(QDate startDate);
            void getIdProducForDate(QDate startDate);

        void insertInNnTable(double id_prod,double day,double mou,double sales,double year);
            void insettInWeatherTable(double temp,double hum,double os,double wind,QDate date);


};





class Data {
public:


    QDate date; // дата
    QList<qint32> prod; //айдишки продуктов проданных за эту дату



    Data(QString *dateStr);
    ~Data();



};

bool saveNnConfig(QMap<QString, QString> &config);
QMap<QString, QString> loadNnConfig();


QList<Data> getDateFromDb(DB *dbase, QDate startDate,bool readAll); //получение всей инфы
void updateDataForNN(QList<Data> *data,DB *db);
QMap<QDateTime,qint32> getNumSalesProd(qint32 *prodId,QList<Data> *dataFromDb, QList<QDate> *endDAte); //расчет числа продаж за дату
bool dateCheker(const QDate *date, QList<QDate> *enddate);

void importFullInfo(QList<Data> *data); //импорт из файлов

#endif // MSQL_H
