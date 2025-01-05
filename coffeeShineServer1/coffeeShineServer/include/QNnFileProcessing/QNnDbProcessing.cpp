#include"QNnDbProcessing.h"
#include <vector>

Data::Data(QString *dateStr) {

    date = QDate(dateStr->split("-")[0].toInt(),dateStr->split("-")[1].toInt(),dateStr->split("-")[2].toInt());


}


Data::~Data() {


}


DB::DB(QSqlDatabase *dbase) {
    db = dbase;

    dbForNn =  QSqlDatabase::addDatabase("QMYSQL", "db_connection2");
    dbForNn.setHostName("127.0.0.1");
    dbForNn.setUserName("root");
    dbForNn.setPassword("1756");
    dbForNn.setDatabaseName("nninfo");

    if (!dbForNn.open()) {
        qDebug() << "Failed to connect to database:" << dbForNn.lastError().text();

    }



}

DB::~DB() {
    dbForNn.close();

}





void DB::getIdProduct() {
    // QVector<QCoffeeSoldPositionInfo> SPinfo= currentPlugin->getListSoldPosition();

    // int i =1;
    // while (i<SPinfo.size()) {

    //     idProduct.push_back(SPinfo.at(i).idDrink);

    //     dateProductSale.push_back(QString::number(SPinfo.at(i).date.date().year())+
    //                               "-"+QString::number(SPinfo.at(i).date.date().month())+"-"
    //                               +QString::number(SPinfo.at(i).date.date().day()));
    //     i++;

    // }


    QSqlQuery *query = new QSqlQuery(*db);
    query->exec("SELECT soldPosition_id_drink FROM tbl_soldPosition ORDER BY id_soldPosition;");
    while (query->next()) {

        idProduct.push_back(query->value(0).toInt());

    }
    delete query;


}

void DB::getDateProductSale() {




    QSqlQuery *query = new QSqlQuery(*db);
    query->exec("select date_soldPosition from tbl_soldPosition;");
    while (query->next()) {

        dateProductSale.push_back(query->value(0).toString().split("T")[0]);

    }
    delete query;



}




QString DB::getProductName(qint32 *id) {

    // QCoffeeDrinkInfo DrinkInfo = currentPlugin->getDrinkInfo(*id);

    // QString answ = DrinkInfo.name;

    // return answ;


    QSqlQuery *query = new QSqlQuery(*db);
    query->exec("select name_drink from tbl_drink where id_drink="+QString::number(*id)+";");
    query->first();
    QString answ = query->value(0).toString();
    delete query;
    return answ;
}

void DB::getIdProducForDate(QDate startDate) {

    // bool ok = false;
    //  QString textQuery = "SELECT soldPosition_id_drink FROM tbl_soldPosition where date_soldPosition > '"+startDate.toString("yyyy-MM-dd")+"' ORDER BY id_soldPosition;";
    // QSqlQuery *query = currentPlugin->execQuery(textQuery,&ok);

    // while (query->next()) {

    //     idProduct.push_back(query->value(0).toInt());

    // }
    // delete query;


    QSqlQuery *query = new QSqlQuery(*db);
    query->exec("SELECT soldPosition_id_drink FROM tbl_soldPosition where date_soldPosition > '"+startDate.toString("yyyy-MM-dd")+"' ORDER BY id_soldPosition;");
    while (query->next()) {

        idProduct.push_back(query->value(0).toInt());

    }
    delete query;



}

void DB::getDateProductSaleForDate(QDate startDate) {


    //    bool ok = false;
    //  QString textQuery = "SELECT date_soldPosition FROM tbl_soldPosition WHERE date_soldPosition > '" + startDate.toString("yyyy-MM-dd") + "';";
    // QSqlQuery *query = currentPlugin->execQuery(textQuery,&ok);


    // while (query->next()) {

    //     dateProductSale.push_back(query->value(0).toString().split("T")[0]);

    // }

    // delete query;


    QSqlQuery *query = new QSqlQuery(*db);
    query->exec("SELECT date_soldPosition FROM tbl_soldPosition WHERE date_soldPosition > '" + startDate.toString("yyyy-MM-dd") + "';");

    while (query->next()) {

        dateProductSale.push_back(query->value(0).toString().split("T")[0]);

    }

    delete query;


}




QList<Data> getDateFromDb(DB *dbase, QDate startDate,bool readAll) {


    if (readAll) {
        dbase->getIdProduct();
        dbase->getDateProductSale();
    } else {
        dbase->getIdProducForDate(startDate);
        dbase->getDateProductSaleForDate(startDate);
    }

    QList<Data> D;

    QString *tempDate = new QString;
    int i1 =0;
    if (dbase->dateProductSale.size()>0) {
        *tempDate = dbase->dateProductSale[0];
        D.push_back(Data(&dbase->dateProductSale[0]));
        for (int i =0;i<dbase->dateProductSale.size();i++) {
            if (*tempDate==dbase->dateProductSale[i]) {
                D[i1].prod.push_back(dbase->idProduct[i]);
            } else {
                *tempDate = dbase->dateProductSale[i];
                D.push_back(Data(&dbase->dateProductSale[i]));
                i1++;
            }
        }
    }
    delete tempDate;
    qDebug()<<D.size()<<"sizeeee";
    return D;
}

bool dateCheker(const QDate *date, QList<QDate> *enddate) {
    if (*date <= enddate->at(1) && *date >= enddate->at(0)) {
        return true;
    }
    return false;
}

QMap<QDateTime,qint32> getNumSalesProd(qint32 *prodId,QList<Data> *dataFromDb, QList<QDate> *endDAte) {
    QMap<QDateTime,qint32> numOfSales;

    for (int i = 0; i < dataFromDb->size(); i++) {
        if (dateCheker(&(dataFromDb->at(i).date), endDAte) && !numOfSales.contains(QDateTime(dataFromDb->at(i).date))) {
            numOfSales.insert(QDateTime(dataFromDb->at(i).date),std::count(dataFromDb->at(i).prod.begin(), dataFromDb->at(i).prod.end(), *prodId));

        }
    }
    return numOfSales;
}

bool comp(const std::vector<double>& a, const std::vector<double>& b)
{
    if (a[2] < b[2]) {
         return 1;
    }

    if (a[2] > b[2]) {
         return 0;
    }
    if (a[2] == b[2]) {
        if (a[1] < b[1]) {
            return 1;
         } else if (a[1] > b[1])   {
            return 0;
        } else if(a[1] == b[1]) {
            return 0;
        }
    }

}

void importFullInfo(QList<Data> *data) {
   QFile file("weather.sex");
    qint32 *a=new qint32;
    QList<QDate> endDAte {QDate(2019,5,1),QDate(2021,8,26)};


   /*  if (file.open(QIODevice::Append | QIODevice::Text)) {


        QTextStream stream(&file);

        for (QDate i = endDAte.at(0);i<endDAte.at(1);i=i.addDays(1)) {

            QDate tempd = i;
             Weather weather(&tempd);
             stream <<QString::number(weather.get_temperature())<<" "<<QString::number(weather.get_humidity())<<" "<<QString::number(weather.getOs())
                   <<" "<<QString::number(weather.getWindSpeed())<<" "<<tempd.toString("yyyy-MM-dd")<< endl;
        }
   }
    file.close();*/
    QFile file1("allprod1.sex");
    std::vector<std::vector<double>> a1;
    QMap<QDateTime,qint32> temp;
    if (file1.open(QIODevice::WriteOnly | QIODevice::Text)) {


           QTextStream stream1(&file1);

           for (int prod=1;prod<=153;prod++) {
                *a=prod;

               temp = getNumSalesProd(a,data,&endDAte);


               for (auto i=temp.begin();i!=temp.end();i++) {
                   qDebug()<<i.value();

                   a1.push_back({prod,i.key().date().day(),i.key().date().month(),i.value(),i.key().date().year()});

                  // stream << QString::number(prod)+" "+ QString::number(i.key().date().day())+" "+ QString::number(i.key().date().month())+" "+ QString::number(i.value())<< endl;
               }

           }
            sort(a1.begin(),a1.end(),comp);
           for (int i=0;i<a1.size();i++) {
                    stream1 << QString::number(a1[i][0])+" "+ QString::number(a1[i][1])+" "+
                            QString::number(a1[i][2])+" "+ QString::number(a1[i][3])+" "+ QString::number(a1[i][4])<< endl;


           }




       } else {

           qDebug() << "Ошибка открытия файла";
       }
    file1.close();
}



QString DB::getLastDateFromNn() {
    QSqlQuery *query = new QSqlQuery(dbForNn);
    query->exec("select dateSale from salesInfo ORDER BY dateSale DESC LIMIT 1;");
    query->first();
    QString answ = query->value(0).toString();
    delete query;
    qDebug()<<answ<<"laaaastt";
    return answ;
}


void DB::insertInNnTable(double id_prod,double day,double mou,double sales,double year) {
    QString textQuery = "INSERT INTO salesInfo (id_drink, sales, dateSale) VALUES ('" +
                        QString::number(id_prod) + "','" +
                        QString::number(sales) + "','" +
                        QString::number(static_cast<int>(year)) + "-" +
                        QString::number(static_cast<int>(mou)).rightJustified(2, '0') + "-" +
                        QString::number(static_cast<int>(day)).rightJustified(2, '0') + "');";


    QSqlQuery *query = new QSqlQuery(dbForNn);
    query->exec(textQuery);

    qDebug()<<textQuery;
    delete query;

}




void updateDataForNN(QList<Data> *data,DB *db) {
    std::vector<std::vector<double>> a1;
    QMap<QDateTime,qint32> temp;
    qint32 *a=new qint32;
    QList<QDate> endDAte {data->first().date,data->last().date};


    for (int prod=1;prod<=150;prod++) {
        *a=prod;

        temp = getNumSalesProd(a,data,&endDAte);


        for (auto i=temp.begin();i!=temp.end();i++) {
            qDebug()<<i.value();

            a1.push_back({prod,i.key().date().day(),i.key().date().month(),i.value(),i.key().date().year()});

            // stream << QString::number(prod)+" "+ QString::number(i.key().date().day())+" "+ QString::number(i.key().date().month())+" "+ QString::number(i.value())<< endl;
        }

    }

    sort(a1.begin(),a1.end(),comp);


    for (int i=0;i<a1.size();i++) {
        db->insertInNnTable(a1.at(i).at(0),a1.at(i).at(1),a1.at(i).at(2),a1.at(i).at(3),a1.at(i).at(4));

    }





}
