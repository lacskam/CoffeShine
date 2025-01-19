#include "qnntabwidget.h"
#include <algorithm>
#include <QWidget>



QList<QPointF> points;

QNnTabWidget::QNnTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent) {
    currentPlugin = plugin_;
    int df =8;
    QList<QDate> d = {QDate(2019,1,1),QDate(2020,3,8)};
    dbase = new DB(plugin_);

    createForm(0,&df,&d);
    connect(currentPlugin, &QCoffeeClientPlugin::predictionDataReceived, this, &QNnTabWidget::updateNN);
    connect(currentPlugin, &QCoffeeClientPlugin::predictionDataReceivedForStart, this, &QNnTabWidget::GetStatPrediction);
}

void QNnTabWidget::GetStatPrediction(QMap<qint32, float> predictionResults) {



    qint32 idt;
    QVector<QPair<float, QString>> indexedValues;
    QStringList drinksForBar;

    for (QMap<qint32, float>::const_iterator i = predictionResults.begin(); i != predictionResults.end(); i++) {
        idt = i.key();
        float roundedValue = QString::number(i.value(), 'f', 2).toFloat();
         indexedValues.append(qMakePair(roundedValue, dbase->getProductName(&idt)));
    }


    std::sort(indexedValues.begin(), indexedValues.end(), [](const QPair<float, QString>& a, const QPair<float, QString>& b) {
        return a.first < b.first;
    });


    float maxValue =0;
    for (int i=0;i<indexedValues.size();i++) {
        drinksForBar.push_back(indexedValues.at(i).second);
        *temp << indexedValues.at(i).first;

        if (maxValue<indexedValues.at(i).first) maxValue =indexedValues.at(i).first;
    }


    axisX->append(drinksForBar);
     axisY->setRange(0, maxValue+10);

    temp->setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));

    series->append(temp);

     series->setLabelsVisible(true);

    chartViewProdStat->setMinimumHeight(37*drinksForBar.size());
        updateStatWg();

         isLoadingStat=0;
        if (mainTabWidget->currentIndex()==1) {
            stackedWidget2->setCurrentIndex(0);
        }







}
/*
void QNnTabWidget::next() {
    QDate startDate = leFirstDate->date();
    QDate endDate = leSecondDate->date();

    qDebug()<<"slot sendPredictionRequest";
    if (startDate >= QDate::currentDate() && startDate <= QDate::currentDate().addDays(14) && endDate
         >= QDate::currentDate() && endDate <= QDate::currentDate().addDays(14) && *idFromGetStat < 153 && *idFromGetStat > 0) {
        currentPlugin->sendPredictionRequest(startDate, endDate, *idFromGetStat,0);
    } else {
        qDebug()<<"Нерпавильный ввод";
    }
}
*/

void QNnTabWidget::updateStatWg() {





    chartProdStat->addSeries(series);
    chartProdStat->setTitle("Самые востребованные товары на "+leFirstDate->date().toString()+" - "+leSecondDate->date().toString());



    chartProdStat->addAxis(axisX, Qt::AlignLeft);
    series->attachAxis(axisX);


    chartProdStat->addAxis(axisY, Qt::AlignBottom);
    series->attachAxis(axisY);


}


void QNnTabWidget::createStatForm() {


        idForGetStatStart = new quint32;
        idForGetStatEnd  = new quint32;
        btnLoadStat = new QPushButton("Загрузить статистику");

        vbstPred = new QVBoxLayout();
        series = new QHorizontalBarSeries();
        temp = new QBarSet("");
        categories = new QStringList;




        chartProdStat = new QChart();


        chartProdStat->setAnimationOptions(QChart::SeriesAnimations);


        chartProdStat->addSeries(series);
        axisX = new QBarCategoryAxis();
        axisX->append(*categories);
        chartProdStat->addAxis(axisX, Qt::AlignLeft);
        series->attachAxis(axisX);

        axisY = new QValueAxis();
        axisY->setRange(0, 20);
        chartProdStat->addAxis(axisY, Qt::AlignBottom);
        series->attachAxis(axisY);

        chartViewProdStat = new QChartView(chartProdStat);
        chartViewProdStat->setRenderHint(QPainter::Antialiasing);
        chartViewProdStat->setMinimumHeight(200);
        chartViewProdStat->setRubberBand(QChartView::HorizontalRubberBand);
        scrollArea = new CustomScrollArea(chartViewProdStat);
        scrollArea->setWidget(chartViewProdStat);
        scrollArea->setWidgetResizable(true);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        vbstPred->addWidget(scrollArea);
        vbstPred->addWidget(btnLoadStat);


        connect(btnLoadStat, &QPushButton::clicked, this, [=]() {
            QDate startDate = leFirstDate->date();
            QDate endDate = leSecondDate->date();

            *idForGetStatStart =1;
            *idForGetStatEnd =10;
            chartProdStat->removeAllSeries();
            chartProdStat->series().clear();
            series = new QHorizontalBarSeries();
            qDebug()<<"slot sendPredictionRequestToStat";
            if (startDate >= QDate::currentDate() && startDate <= QDate::currentDate().addDays(14) && endDate
                >= QDate::currentDate() && endDate <= QDate::currentDate().addDays(14) && *idForGetStatStart < 153
                && *idForGetStatStart > 0 && *idForGetStatEnd < 153
                && *idForGetStatEnd > 0 && *idForGetStatEnd >= *idForGetStatStart) {
                stackedWidget2->setCurrentIndex(1);
                isLoadingStat=1;
                currentPlugin->sendPredictionRequestFotStat(startDate, endDate, *idForGetStatStart,*idForGetStatEnd);
            } else {
                qDebug()<<"Нерпавильный ввод";
            }

        });



}

void QNnTabWidget::createForm(QWidget *parent, qint32 *pickedprod, QList<QDate> *endDAte)  {

    productPrediction = new QWidget;
     productStatisticPrediction = new QWidget;
    allDrinks = currentPlugin->getListDrink();

    createStatForm();


    mainTabWidget = new QTabWidget;
    loadingWg = new QWidget;

    lLoading = new QVBoxLayout;
    loadingWg->setLayout(lLoading);
    loading = new LoadingWidget();
    isLoadingPred =0;
    isLoadingStat=0;
    lLoading->addWidget(loading);


    loadingWg2 = new QWidget;

    lLoading2 = new QVBoxLayout;
    loadingWg2->setLayout(lLoading2);
    loading2 = new LoadingWidget();
      lLoading2->addWidget(loading2);

    mainLayout = new QVBoxLayout(this);

    vbstacked = new QVBoxLayout;
    groupBox = new QGroupBox();

    vb = new QVBoxLayout();
    hb = new QHBoxLayout;
    hblcharts = new QHBoxLayout;
    vbs = new QHBoxLayout;

    stackedWidget = new QStackedWidget();
     stackedWidget2 = new QStackedWidget;
    hblforhead = new QHBoxLayout;
      mainLayout->addLayout(hblforhead);
    buttonettings = new QPushButton();
    buttonettings->setIcon(QIcon(":/icons/other/settingsNn.png"));
    buttonettings->setMaximumSize(30,30);
     connect(buttonettings,&QPushButton::clicked,this,&QNnTabWidget::openSettingsDialog);
    // connect(buttonettings,&QPushButton::clicked,this, [=]() {
    //     currentPlugin->sendRelearnRequest();
    // });


    mainTabWidget->setCornerWidget(buttonettings, Qt::TopRightCorner);

    hblforhead->addWidget(mainTabWidget);







    vbstacked->addLayout(hblcharts);
    vbstacked->addLayout(hb);
    productPrediction->setLayout(vbstacked);
    productStatisticPrediction->setLayout(vbstPred);

    stackedWidget->addWidget(productPrediction);
    stackedWidget->addWidget(loadingWg);

    stackedWidget2->addWidget(productStatisticPrediction);
    stackedWidget2->addWidget(loadingWg2);

    mainTabWidget->addTab(stackedWidget,"Предсказание по одному товару");
    mainTabWidget->addTab(stackedWidget2,"Рейтинг востребованности товаров");

    vb->addLayout(hblforhead);
   // vb->addWidget(stackedWidget);



    mapNN = new QMap<QDateTime, float>;


    leFirstDate = new QDateEdit;
    leFirstDate->setDate(QDate::currentDate().addDays(1));

    hb->addWidget(leFirstDate);

    leSecondDate = new QDateEdit;
    leSecondDate->setDate(QDate::currentDate().addDays(13));
    hb->addWidget(leSecondDate);

    lePickedProd = new QComboBox;

    for (int i=0;i<allDrinks.size();i++) {
        lePickedProd->addItem(allDrinks.at(i).name);
    }

    hb->addWidget(lePickedProd);

   // QValueAxis *axisY = new QValueAxis();
   // QDateTimeAxis *axisX = new QDateTimeAxis();

    axisYnn = new QValueAxis();
    axisXnn = new QDateTimeAxis();



   // QMap<QDateTime,qint32> *mapSaless = new QMap<QDateTime,qint32>;

    //btn = new QPushButton("Plot");
 //   connect(btn, &QPushButton::clicked, this, [=]() {
 //       updateChart(dbase1, axisY, axisX, lePickedProd, leFirstDate, leSecondDate,mapSaless);
  //  });



    // connect(chartsComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
    //         this, [this](int index) {


    //             if (index == 0) {
    //                  if (isLoadingPred == 0) {

    //                  } else {
    //                      stackedWidget->setCurrentIndex(1);
    //                  }

    //             } else if (index == 1) {
    //                  if (isLoadingStat == 0) {

    //                  } else {
    //                      stackedWidget->setCurrentIndex(1);
    //                  }
    //             }

    //         });



    connect(mainTabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 0) {
            if (isLoadingPred == 0) {

            } else {
                stackedWidget->setCurrentIndex(1);
            }
        } else if (index == 1) {
            if (isLoadingStat == 0) {

            } else {
                stackedWidget2->setCurrentIndex(1);
            }
        }
    });
    plotNNButton = new QPushButton("Прогноз");
    connect(plotNNButton, &QPushButton::clicked, this, [=]() {
        QDate startDate = leFirstDate->date();
        QDate endDate = leSecondDate->date();

        quint32 pickedProduct = allDrinks.at(lePickedProd->currentIndex()).id;
        qDebug()<<"slot sendPredictionRequest";
        if (startDate >= QDate::currentDate() && startDate <= QDate::currentDate().addDays(14) && pickedProduct < 153
            && pickedProduct > 0) {
            stackedWidget->setCurrentIndex(1);
            isLoadingPred=1;

            currentPlugin->sendPredictionRequest(startDate, endDate, pickedProduct,1);
        } else {
            qDebug()<<"Нерпавильный ввод";
        }

    });

   // hb->addWidget(btn);
    hb->addWidget(plotNNButton);

    btnGetFile = new QPushButton("Получить");
    connect(btnGetFile, &QPushButton::clicked, this, [=]() {
     //  getfilePlot(lePickedProd,mapSaless);
    });

   btnOpenFile = new QPushButton("Открыть");
    /* connect(btnOpenFile, &QPushButton::clicked, this, [=]() {
       openfile(axisY, axisX);
    });

*/

     hb->addWidget(btnGetFile);
     hb->addWidget(btnOpenFile);


   // chart = new QChart();
    chartnn = new QChart();
    chartnn->setAnimationOptions(QChart::SeriesAnimations);

  /*  QList<QDateTime> dates;
    QList<qint32> values;
    QList<Data> dataFromDb = getDateFromDb(dbase1);
    *mapSaless = (getNumSalesProd(pickedprod,&dataFromDb,endDAte));

    for (QMap<QDateTime,qint32>::const_iterator i=mapSaless->begin();i!=mapSaless->end();i++) {
        dates.push_back(i.key());
        values.push_back(i.value());
    }

*/

   /*for (int i = 0; i < dataFromDb->size(); i++) {
        if (dateCheker(&(dataFromDb->at(i).date), endDAte) && !dates.contains(QDateTime(dataFromDb->at(i).date))) {
            dates.append(QDateTime(dataFromDb->at(i).date));
            values.append(std::count(dataFromDb->at(i).prod.begin(), dataFromDb->at(i).prod.end(), *pickedprod));
        }
    }*/

  //  ser = new QSplineSeries();
    serNN = new QSplineSeries;


 /*   for (int i = 0; i < dates.size(); ++i) {
        ser->append(dates[i].toMSecsSinceEpoch(), values[i]);
    }
*/

    /*
    axisX->setTickCount(11);
    axisX->setFormat("dd.MM.yyyy");

    line = new QGraphicsLineItem(0, 0, 0, 0);
    line->setPen(QPen(Qt::red,1));
*/
    lineNN = new QGraphicsLineItem(0, 0, 0, 0);
    lineNN->setPen(QPen(Qt::red,1));

    axisXnn->setTickCount(11);
    axisXnn->setFormat("dd.MM.yyyy");
 /*
    chart->addSeries(ser);
    chart->addAxis(axisX, Qt::AlignBottom);
*/
    chartnn->addSeries(serNN);
    chartnn->addAxis(axisXnn, Qt::AlignBottom);

  //  ser->attachAxis(axisX);

    serNN->attachAxis(axisXnn);

  //  chart->addAxis(axisY, Qt::AlignLeft);
    chartnn->addAxis(axisYnn, Qt::AlignLeft);

   // ser->attachAxis(axisY);
      serNN->attachAxis(axisYnn);

   // axisX->setTitleText("дата");
  //  axisY->setTitleText("продажи \"" + dbase1->getProductName(pickedprod) + "\"");

    axisXnn->setTitleText("дата");
    axisYnn->setTitleText("продажи \"" + dbase->getProductName(pickedprod) + "\"");

  //  chartView = new QChartView(this->chart);
     chartViewNN = new QChartView(this->chartnn);

/*
     sliderBase = new QSlider(Qt::Horizontal,chartView);
     sliderBase->setStyleSheet("QSlider::groove:horizontal { border: none; background: none; }"
                                  "QSlider::handle:horizontal { background: #024fad; width: 10px; height: 5px; border-radius: 5px; }");

    sliderBase->resize(width()*1.27,15);
     sliderBase->move(width()/8.2,height()/7.5);
      connect(sliderBase,&QSlider::valueChanged,this,[=]() {
          lineMove(mapSaless);
      });
*/
     sliderNN = new QSlider(Qt::Horizontal,chartViewNN);
     sliderNN->setStyleSheet("QSlider::groove:horizontal { border: none; background: none; }"
                                  "QSlider::handle:horizontal { background: #024fad; width: 10px; height: 5px; border-radius: 5px; }");
     connect(sliderNN,&QSlider::valueChanged,this,[=]() {
         lineMoveNN(mapNN);
     });
     sliderNN->resize(width()-149,15);
     sliderNN->move(71,65);

 //   slider_range = new int(1);
    slider_rangeNN = new int(1);
   // labelValue = new QLabel(chartView);
    labelValueNN = new QLabel(chartViewNN);

    labelValueNN->move(width()*2,height()*2);
   // labelValue->move(width()*2,height()*2);

 //   labelValue->setText("<p style=\"color: rgb(0, 0, 0)\">34365</p>");
    labelValueNN->setText("<p style=\"color: rgb(0, 0, 0)\">34365</p>");

  //  hblcharts->addWidget(chartView);
    hblcharts->addWidget(chartViewNN);



   // chart->scene()->addItem(line);
    chartnn->scene()->addItem(lineNN);


}

void QNnTabWidget::updateNN(QMap<QDateTime, float> predictionResults) {



    QList<QDateTime> dates;
    QList<float> values;
    qDebug()<<"updateNN";



    if (predictionResults.size()<1) {
        qDebug()<<"Ошибка получения предсказания";
        isLoadingPred=0;
        if (mainTabWidget->currentIndex()==0) {
            stackedWidget->setCurrentIndex(0);
        }
        return;
    }

    *mapNN = predictionResults;

    for (QMap<QDateTime, float>::const_iterator i = mapNN->begin(); i != mapNN->end(); i++) {
        values.push_back(i.value());
        qDebug() << i.value();
        qDebug() << i.key();
        dates.push_back(i.key());
    }


    chartnn->removeAllSeries();
    chartnn->series().clear();
    *slider_rangeNN = dates.size();

    serNN = new QSplineSeries;
    for (int i = 0; i < dates.size(); ++i) {


        serNN->append(dates[i].toMSecsSinceEpoch(), values[i]);
    }

    chartnn->addSeries(serNN);
    sliderNN->setRange(0, *slider_rangeNN-1);
    QString a = lePickedProd->currentText();
    axisYnn->setTitleText("продажи \"" + a + "\"");
    axisXnn->setRange(dates.first(), dates.last());

    axisYnn->setRange(*std::min_element(values.begin(), values.end()), *(std::max_element(values.begin(), values.end())));

    isLoadingPred=0;
    if (mainTabWidget->currentIndex()==0) {
         stackedWidget->setCurrentIndex(0);
    }




}

void QNnTabWidget::openfile(QValueAxis *axisY, QDateTimeAxis *axisX) {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", "", "Файлы с расширением .sex (*.sex);;Все файлы (*.*)");
    int status =0;
    QString fileCont;
    if (!filePath.isEmpty()) {

        QFile file(filePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);


            fileCont = stream.readAll();

            status =1;
            file.close();
        } else {
            qDebug() << "Ошибка открытия файла для чтения";
        }
        }
     else  qDebug() << "Отменено пользователем";


    if (status ==1 && !fileCont.isEmpty()) {
        QList<QDateTime> dates;
        QList<qint32> values;
        QList<QString> list;
        ser = new QSplineSeries();
        list = fileCont.split('\n');
        for (int i=0;i<list.size()-1;i++) {
            dates.push_back(QDateTime(QDate(list[i].split(" ")[1].split(".")[0].toInt(),list[i].split(" ")[1].split(".")[1].toInt(),list[i].split(" ")[1].split(".")[2].toInt())));
            qDebug()<<dates[i];
            values.push_back(std::abs(list[i].split(" ")[0].toFloat()));
        }
    \

        chart->removeAllSeries();
        chart->series().clear();

        for (int i = 0; i < dates.size(); ++i) {
            ser->append(dates[i].toMSecsSinceEpoch(), values[i]);

        }

        chart->addSeries(ser);

        axisX->setRange(dates.first(), dates.last());
                axisY->setRange(*std::min_element(values.begin(), values.end()), *std::max_element(values.begin(), values.end()));



    }
}


void QNnTabWidget::getfilePlot(QSpinBox *lePickedProd,QMap<QDateTime,qint32> *mapSales) {
    QFile file(QString::number(lePickedProd->value())+"_prod.sex");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

           QTextStream stream(&file);
           for (QMap<QDateTime,qint32>::const_iterator i=mapSales->begin();i!=mapSales->end();i++) {
               stream << QString::number(i.value()) + " " + i.key().date().toString("yyyy.MM.dd")<< endl;
           }

       } else {

           qDebug() << "Ошибка открытия файла";
       }
    file.close();
}



void QNnTabWidget::updateChart(DB *dbase1, QValueAxis *axisY, QDateTimeAxis *axisX,
                  QSpinBox *lePickedProd, QDateEdit *leFirstDate, QDateEdit *leSecondDate,QMap<QDateTime,qint32> *mapSaless) {
    ser = new QSplineSeries();
    QList<Data> dataFromDb = getDateFromDb(dbase1);
    QList<QDate> endDAte = {leFirstDate->date(), leSecondDate->date()};
    QList<QDateTime> dates;
    QList<qint32> values;
    qint32 pickedprod = lePickedProd->value();
   *mapSaless =  getNumSalesProd(&pickedprod,&dataFromDb,&endDAte);


    for (QMap<QDateTime,qint32>::const_iterator i=mapSaless->begin();i!=mapSaless->end();i++) {
        dates.push_back(i.key());
        values.push_back(i.value());
    }

    *slider_range = dates.size();
    sliderBase->setRange(0,*slider_range);
    chart->removeAllSeries();
    chart->series().clear();

    for (int i = 0; i < dates.size(); ++i) {

        ser->append(dates[i].addMSecs(0).toMSecsSinceEpoch(), values[i]);

    }

    chart->addSeries(ser);

    qint32 a = lePickedProd->value();
    axisY->setTitleText("продажи \"" + dbase1->getProductName(&a) + "\"");
    axisX->setRange(dates.first(), dates.last());
        axisY->setRange(*std::min_element(values.begin(), values.end()), *std::max_element(values.begin(), values.end()));

}


void QNnTabWidget::lineMove(QMap<QDateTime,qint32> *mapSaless) {

    line->setLine(width()/23.1+sliderBase->value()*((width()-2*width()/23)/(*slider_range*2.187)), height()*0.078,
                              width()/23.2+sliderBase->value()*((width()-2*width()/23)/(*slider_range*2.187)),  height()/1.15);
    labelValue->move(width()/25.5+sliderBase->value()*((width()-2*width()/23)/(*slider_range*2.187)),height()/21);

        labelValue->setText("<p style=\"color: rgb(0, 0, 0)\">"+QString::number((((mapSaless->begin()+sliderBase->value()).value())))+"</p>");
    //points.at(sliderBase->value()).y()

}

void QNnTabWidget::lineMoveNN(QMap<QDateTime,float> *mapSaless) {


    sliderNN->resize(width()-150,15);
    sliderNN->move(72,65);

    QRect sliderRect = sliderNN->rect();

    int totalWidth = sliderRect.width();

    int handleX = sliderNN->geometry().left()+5 + static_cast<double>(sliderNN->value()) / (sliderNN->maximum()) * (totalWidth-10.5);
    lineNN->setLine(handleX, chartViewNN->height()-76,
                    handleX,  chartViewNN->height()-chartViewNN->height()+80);
    labelValueNN->move(handleX-10,chartViewNN->height()-chartViewNN->height()+40);
    qfloat16 g = std::abs((mapSaless->begin()+sliderNN->value()).value());

    labelValueNN->setText("<p style=\"color: rgb(0, 0, 0)\">"+QString::number(g,'f',2)+"</p>");

}


void QNnTabWidget::openSettingsDialog() {
    settingsWidget = new QNnSettingsWidget(currentPlugin,this);
    settingsWidget->show();

}
