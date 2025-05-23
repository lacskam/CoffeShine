#include "QNnLocalProcessing.h"

void writeFile(int prod, int day, int mou, float dem) {
    QFile file(QString::number(prod) + "_prod.sex");

    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        {
            stream << QString::number(dem) + " 2024." + QString::number(mou) + "." + QString::number(day) << endl;
        }

    } else {
        qDebug() << "Ошибка открытия файла";
    }
    file.close();
}

/*std::vector<std::tuple<int, int, int, float>> getfile(int *prod) {
    std::vector<std::tuple<int, int, int, float>> data;
    //QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", "", "Файлы с расширением .sex (*.sex);;Все файлы (*.*)");
    QString filePath = "allprod.sex";

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);

            QString fileContents = stream.readAll();

            file.close();

            QList<QString> temp = fileContents.split("\n");
            for (int i = 0; i < temp.size() - 1; i++) {
                if (temp[i].split(" ")[0].toInt()==*prod) {
                    data.push_back({(temp[i].split(" ")[0]).toInt(), (temp[i].split(" ")[1]).toInt(),
                                    temp[i].split(" ")[2].toInt(), (temp[i].split(" ")[3]).toFloat()});
                    qDebug() << (temp[i].split(" ")[0]).toInt() << " " << (temp[i].split(" ")[1]).toInt() << " "
                             << temp[i].split(" ")[2].toInt() << " " << (temp[i].split(" ")[3]).toFloat();
                }
            }
        } else {
            qDebug() << "Ошибка открытия файла для чтения";
        }
    } else {
        qDebug() << "Отменено пользователем";
    }
    return data;
}
*/

std::vector<std::tuple<float,float,float,float,float,float,float,float>> getfile1(int *prod) {
    std::vector<std::tuple<float,float,float,float,float,float,float,float>> data;
    //QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", "", "Файлы с расширением .sex (*.sex);;Все файлы (*.*)");
    QString filePath = "allprod1.sex";
    QString filePath2 = "weather.sex";

    if (!filePath.isEmpty()) {
        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);

            QString fileContents = stream.readAll();

            file.close();
        QFile file1(filePath2);
        if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
             QTextStream stream1(&file1);
            QString fileContents1 = stream1.readAll();
            file1.close();







            QList<QString> temp = fileContents.split("\n");
            QList<QString> temp1 = fileContents1.split("\n");
            QDate date;



            for (int i = 0; i < temp.size() - 1; i++) {
                if (temp[i].split(" ")[0].toInt()==*prod) {
                    date = QDate(temp[i].split(" ")[4].toInt(),temp[i].split(" ")[2].toInt(),temp[i].split(" ")[1].toInt());

                    for (int j=0;j<temp1.size()-1;j++) {

                        if (date == QDate(temp1[j].split(" ")[4].split("-")[0].toInt(),temp1[j].split(" ")[4].split("-")[1].toInt(),temp1[j].split(" ")[4].split("-")[2].toInt())) {
                            data.push_back(std::make_tuple((temp[i].split(" ")[0]).toFloat(), (temp[i].split(" ")[1]).toFloat(),
                                            temp[i].split(" ")[2].toFloat(), (temp[i].split(" ")[3]).toFloat(),
                                            (temp1[j].split(" ")[0]).toFloat(),(temp1[j].split(" ")[1]).toFloat(),
                                            (temp1[j].split(" ")[2]).toFloat(),(temp1[j].split(" ")[3]).toFloat()));


                        }

                    }



                }
            }
        }
        } else {
            qDebug() << "Ошибка открытия файла для чтения";
        }
    } else {
        qDebug() << "Отменено пользователем";
    }
    return data;
}


std::vector<std::tuple<float, float, float, float, float, float, float, float>>
getDataById(int id_drink) {
    std::vector<std::tuple<float, float, float, float, float, float, float, float>> result;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","db_nn");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("1756");
    db.setDatabaseName("nninfo");

    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError().text();
        return result;
    }


    QSqlQuery salesQuery(db);
    salesQuery.prepare("SELECT id_drink, DAY(dateSale) AS day, MONTH(dateSale) AS month, "
                       "sales, dateSale FROM salesInfo WHERE id_drink = :id_drink");
    salesQuery.bindValue(":id_drink", id_drink);

    if (!salesQuery.exec()) {
        qDebug() << "Failed to execute sales query:" << salesQuery.lastError().text();
        return result;
    }


    QSqlQuery weatherQuery(db);
    while (salesQuery.next()) {
        float salesId = salesQuery.value("id_drink").toFloat();
        float day = salesQuery.value("day").toFloat();
        float month = salesQuery.value("month").toFloat();
        float sales = salesQuery.value("sales").toFloat();
        QString date = salesQuery.value("dateSale").toString();


        weatherQuery.prepare("SELECT temp, hum, os, wind FROM weather WHERE dateWeather = :date");
        weatherQuery.bindValue(":date", date);

        if (!weatherQuery.exec()) {
            qDebug() << "Failed to execute weather query:" << weatherQuery.lastError().text();
            continue;
        }


        if (weatherQuery.next()) {
            float temp = weatherQuery.value("temp").toFloat();
            float hum = weatherQuery.value("hum").toFloat();
            float os = weatherQuery.value("os").toFloat();
            float wind = weatherQuery.value("wind").toFloat();

            result.emplace_back(salesId, month, day, sales, temp, hum, os, wind);
        }
    }

    db.close();
    return result;
}
