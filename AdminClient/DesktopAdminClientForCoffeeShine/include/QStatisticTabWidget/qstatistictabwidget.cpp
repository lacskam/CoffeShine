#include "qstatistictabwidget.h"

QStatisticTabWidget::QStatisticTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent)
{
    currentPlugin = plugin_;
    chartList <<"Продажи"<< "Деньги";
    createForm();
}

void QStatisticTabWidget::createForm()
{


       mainLayout = new QVBoxLayout(this);
    stat1 = new QWidget;
    QVBoxLayout *subLayout = new QVBoxLayout(stat1);


    tabWidget = new QTabWidget;
    mainLayout->addWidget(tabWidget);
    tabWidget->addTab(stat1,"Общее");
    QGroupBox * groupBox = new QGroupBox();
    subLayout->addWidget(groupBox);
    QVBoxLayout * layForGroupBox = new QVBoxLayout();
    groupBox->setLayout(layForGroupBox);
    QHBoxLayout* layForDate = new QHBoxLayout();
    layForGroupBox->addLayout(layForDate);

    chartsComboBox = new QComboBox();
    chartsComboBox->addItems(chartList);
    layForGroupBox->addWidget(chartsComboBox);
    chartsComboBox->setMaximumWidth(250);



    QLabel* lab = new QLabel(tr("C:"));
    layForDate->addWidget(lab);
    calendarStart = new QDateEdit();
    calendarStart->setCalendarPopup(true);
    calendarStart->setDate(QDate::currentDate());
    layForDate->addWidget(calendarStart);
    layForDate->addSpacing(20);
    QLabel* lab2 = new QLabel(tr("По:"));
    layForDate->addWidget(lab2);
    calendarFinish = new QDateEdit();
    calendarFinish->setCalendarPopup(true);
    calendarFinish->setDate(QDate::currentDate());
    layForDate->addWidget(calendarFinish);
    layForDate->addStretch(1);

    QPushButton *openStat = new QPushButton(tr("Показать"));
    openStat->setMaximumWidth(120);
    layForGroupBox->addSpacing(15);
    layForGroupBox->addWidget(openStat);
    connect(openStat,SIGNAL(clicked()),this,SLOT(createMoneyStatictic()));

    QChartView *chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    layForGroupBox->addWidget(chartView,1);


    createDrinkStatForm();
    createCategoryStatForm();

    tabWidget->addTab(stat2,"Напитки");
    tabWidget->addTab(stat3,"Категории");
}


void QStatisticTabWidget::createDrinkStatForm() {

    stat2 = new QWidget();
    vb = new QVBoxLayout(stat2);
    hb = new QHBoxLayout();


    comboBoxForDrinkStat = new QComboBox;
    comboBoxForDrinkStat->addItems({"Продажи","Деньги"});
    vb->addWidget(comboBoxForDrinkStat);


    vbstPred = new QVBoxLayout();
    series = new QHorizontalBarSeries();

    QStringList categories;
    temp = new QBarSet("");
    for (int i = 0; i < 0; i++) {

        categories <<QString::number(i);
        int h = qrand() % ((100 + 1) - 20) + 20;
        *temp << h;
        temp->setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));
         temp->setLabel(QString::number(h));

    }
    series->append(temp);
     series->setLabelsVisible(true);
    chartProdStat = new QChart();

    chartProdStat->addSeries(series);

    chartProdStat->setAnimationOptions(QChart::SeriesAnimations);



    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chartProdStat->addAxis(axisX, Qt::AlignLeft);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
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
    vb->addLayout(vbstPred);



    leFirstDateLabel = new QLabel("C: ");
    leFirstDate = new QDateEdit;

    leFSecondDateLabel = new QLabel("До: ");
    leSecondDate = new QDateEdit;


    drinksComboBox = new QComboBox;


    btn = new QPushButton("Показать");
    connect(btn,&QPushButton::clicked,this,&QStatisticTabWidget::createDrinkStatistic);



    hb->addWidget(leFirstDateLabel);
    hb->addWidget(leFirstDate, Qt::AlignLeft);
    hb->addWidget(leFSecondDateLabel);
    hb->addWidget(leSecondDate, Qt::AlignLeft);
    hb->addWidget(drinksComboBox);
    hb->addWidget(btn);

    vb->addLayout(hb);




}


void QStatisticTabWidget::createCategoryStatForm() {

    stat3 = new QWidget();

    vbCategory = new QVBoxLayout(stat3);
    hbCategory = new QHBoxLayout();


    comboBoxForStatCategory = new QComboBox;
    comboBoxForStatCategory->addItems({"Продажи","Деньги"});
    vbCategory->addWidget(comboBoxForStatCategory);


    vbstPredCategory = new QVBoxLayout();
    seriesCategory = new QHorizontalBarSeries();

    QStringList categories;
    tempCategory = new QBarSet("");
    for (int i = 0; i < 0; i++) {

        categories <<QString::number(i);
        int h = qrand() % ((100 + 1) - 20) + 20;
        *tempCategory << h;
        tempCategory->setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));
        tempCategory->setLabel(QString::number(h));

    }
    seriesCategory->append(tempCategory);
    seriesCategory->setLabelsVisible(true);
    chartProdStatCategory = new QChart();

    chartProdStatCategory->addSeries(seriesCategory);

    chartProdStatCategory->setAnimationOptions(QChart::SeriesAnimations);



    axisXCategory = new QBarCategoryAxis();
    axisXCategory->append(categories);
    chartProdStatCategory->addAxis(axisXCategory, Qt::AlignLeft);
    seriesCategory->attachAxis(axisXCategory);

    axisYCategory = new QValueAxis();
    axisYCategory->setRange(0, 100);
    chartProdStatCategory->addAxis(axisYCategory, Qt::AlignBottom);
    seriesCategory->attachAxis(axisYCategory);



    chartViewProdStatCategory = new QChartView(chartProdStatCategory);
    chartViewProdStatCategory->setRenderHint(QPainter::Antialiasing);
    chartViewProdStatCategory->setMinimumHeight(200);
    chartViewProdStatCategory->setRubberBand(QChartView::HorizontalRubberBand);

    scrollAreaCategory = new CustomScrollArea(chartViewProdStatCategory);
    scrollAreaCategory->setWidget(chartViewProdStatCategory);
    scrollAreaCategory->setWidgetResizable(true);
    scrollAreaCategory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    vbstPredCategory->addWidget(scrollAreaCategory);
    vbCategory->addLayout(vbstPredCategory);



    leFirstDateLabelCategory = new QLabel("C: ");
    leFirstDateCategory = new QDateEdit;

    leFSecondDateLabelCategory = new QLabel("До: ");
    leSecondDateCategory = new QDateEdit;


    drinksComboBoxCategory = new QComboBox;


    btnCategory = new QPushButton("Показать");
    connect(btnCategory,&QPushButton::clicked,this,&QStatisticTabWidget::createCategoryStatistic);



    hbCategory->addWidget(leFirstDateLabelCategory);
    hbCategory->addWidget(leFirstDateCategory, Qt::AlignLeft);
    hbCategory->addWidget(leFSecondDateLabelCategory);
    hbCategory->addWidget(leSecondDateCategory, Qt::AlignLeft);
    hbCategory->addWidget(drinksComboBoxCategory);
    hbCategory->addWidget(btnCategory);

    vbCategory->addLayout(hbCategory);
}


void QStatisticTabWidget::createMoneyStatictic()
{
    if(!isDatesValid())
        return;

    QVector<QCoffeePointSale> listPointSale = currentPlugin->getListPointSale();
    QBarSeries *series = new QBarSeries();
    QBarSet *set;

    double maxPrice = 0;
    for(int i=0;i<listPointSale.count();i++)
    {
        QDate startDate = calendarStart->date();
        set = new QBarSet(listPointSale.at(i).name);

        QVector<QCoffeeReceiptInfo> listReceipt = currentPlugin->getListReceiptInfoForPointSale(listPointSale.at(i).id);
        while (startDate<calendarFinish->date().addDays(1))
        {
            double allPrice = 0;
            for(int j=0;j<listReceipt.count();j++)
            {
                if (listReceipt.at(j).dateTime.date() == startDate)
                {
                    allPrice += listReceipt.at(j).sum;
                }
            }

            if (maxPrice < allPrice)
            {
                maxPrice = allPrice;
            }

            *set<< allPrice;
            series->append(set);
            startDate = startDate.addDays(1);
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Статистика по выручке"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    for (int i=0;i<daysCount+1;i++)
    {
        categories.push_back(calendarStart->date().addDays(i).toString(Qt::DefaultLocaleShortDate));
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("руб.");
    axisY->setRange(0,maxPrice+5);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView->setChart(chart);
}



void QStatisticTabWidget::createDrinkStatistic() {



        comboBoxForDrinkStat->setEnabled(false);

        QVector<qint32> drinks = currentPlugin->getListIdDrink();
         chartViewProdStat->setMinimumHeight(drinks.size()*37);


        QDate firstDate = leFirstDate->date();
        QDate lastDate = leSecondDate->date();
        series->clear();
        qDebug() << "sasdasd";
        QStringList drinksForBar;


        QVector<QPair<double, QString>> indexedValues;


        int maxValue = 0;
        QVector<std::tuple<int, QString,int, double,QDate>> ListSales;
        temp = new QBarSet("");

        qint32 currentId;

        for (int i=1;i<drinks.size();i++) {
            currentId = drinks.at(i);
            ListSales = currentPlugin->getListSoldPositionForIdAndDates(currentId,firstDate,lastDate);


            if (ListSales.isEmpty()) {
                continue;
            }

            QString drinkName = std::get<1>(ListSales.at(0));
            double totalValue = 0;
             if (comboBoxForDrinkStat->currentIndex()==0) {
                for (int j=0;j<ListSales.size()-1;j++) {

                    totalValue += std::get<2>(ListSales.at(j));

                }
             } else if (comboBoxForDrinkStat->currentIndex()==1) {
                 for (int j=0;j<ListSales.size()-1;j++) {

                     totalValue += std::get<3>(ListSales.at(j));

                 }
             }


            if (totalValue>maxValue) {
                maxValue=totalValue;
            }


              indexedValues.append(qMakePair(totalValue, drinkName));


        }




        std::sort(indexedValues.begin(), indexedValues.end(), [](const QPair<double, QString>& a, const QPair<double, QString>& b) {
            return a.first < b.first;
        });



        if (comboBoxForDrinkStat->currentIndex()==0) {
            for (int i=0;i<indexedValues.size();i++) {
                drinksForBar.push_back(indexedValues.at(i).second);
                *temp << static_cast<int>(indexedValues.at(i).first);
            }
        } else if (comboBoxForDrinkStat->currentIndex()==1) {
            for (int i=0;i<indexedValues.size();i++) {
                drinksForBar.push_back(indexedValues.at(i).second);
                *temp <<  QString::number(indexedValues.at(i).first, 'f', 2).toFloat();;
            }
        }




        temp->setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));


        series->append(temp);
         series->setLabelsVisible(true);

        axisX->clear();
        axisX->append(drinksForBar);


        axisY->setRange(0,maxValue+40);
        if (comboBoxForDrinkStat->currentIndex()==0) {
             chartProdStat->setTitle("Количество продаж за период "+firstDate.toString("dd.MM.yyyy")+"-"+lastDate.toString("dd.MM.yyyy")+"");
        } else if (comboBoxForDrinkStat->currentIndex()==1) {
             chartProdStat->setTitle("Сумма продаж за период "+firstDate.toString("dd.MM.yyyy")+"-"+lastDate.toString("dd.MM.yyyy")+"");
        }

        comboBoxForDrinkStat->setEnabled(true);

}

void QStatisticTabWidget::createCategoryStatistic() {


    comboBoxForStatCategory->setEnabled(false);

    QVector<QCoffeeCategoryInfo> categoryes = currentPlugin->getListCategories();
    chartViewProdStatCategory->setMinimumHeight(categoryes.size()*37);

    QVector<QPair<QCoffeeCategoryInfo, float>> categoryesdValues;
    QVector<qint32> drinks = currentPlugin->getListIdDrink();
    for (int i =0;i<categoryes.size();i++) {
        categoryesdValues.append(qMakePair(categoryes.at(i),0));
    }


    QDate firstDate = leFirstDateCategory->date();
    QDate lastDate = leSecondDateCategory->date();
    seriesCategory->clear();
    qDebug() << "sasdasd";
    QStringList drinksForBar;


    QVector<QPair<qint32, QString>> indexedValues;


    int maxValue = 0;
    QVector<std::tuple<int, QString,int, double,QDate>> ListSales;
    tempCategory = new QBarSet("");

    qint32 currentId;

    for (int i=1;i<drinks.size();i++) {
        currentId = drinks.at(i);
        ListSales = currentPlugin->getListSoldPositionForIdAndDates(currentId,firstDate,lastDate);


        if (ListSales.isEmpty()) {
            continue;
        }





        double totalValue = 0;
        if (comboBoxForStatCategory->currentIndex()==0) {
            for (int j=0;j<ListSales.size()-1;j++) {

                totalValue += std::get<2>(ListSales.at(j));

            }
        } else if (comboBoxForStatCategory->currentIndex()==1) {
            for (int j=0;j<ListSales.size()-1;j++) {

                totalValue += std::get<3>(ListSales.at(j));

            }
        }


        for (int j=0;j<categoryesdValues.size();j++) {
            if (currentPlugin->getDrinkInfo(currentId).idCategories.contains(categoryesdValues.at(j).first.id)) {
                categoryesdValues[j].second+=totalValue;

            }
        }




    }



    std::sort(categoryesdValues.begin(), categoryesdValues.end(), [](const QPair<QCoffeeCategoryInfo, float>& a, const QPair<QCoffeeCategoryInfo, float>& b) {
        return a.second < b.second;
    });

    for (int i=0;i<categoryesdValues.size();i++) {
        drinksForBar.push_back(categoryesdValues.at(i).first.name);
         if (comboBoxForStatCategory->currentIndex()==0) {
            *tempCategory << static_cast<int>(categoryesdValues.at(i).second);
         } else  if (comboBoxForStatCategory->currentIndex()==1) {

             *tempCategory << QString::number(categoryesdValues.at(i).second, 'f', 2).toFloat();
         }
        if (maxValue < categoryesdValues.at(i).second) {
            maxValue = categoryesdValues.at(i).second;
        }
    }


    tempCategory->setColor(QColor::fromRgb(qrand() % 256, qrand() % 256, qrand() % 256));


    seriesCategory->append(tempCategory);
    seriesCategory->setLabelsVisible(true);

    axisXCategory->clear();
    axisXCategory->append(drinksForBar);


    axisYCategory->setRange(0,maxValue+40);
    if (comboBoxForStatCategory->currentIndex()==0) {
        chartProdStatCategory->setTitle("Количество продаж за период "+firstDate.toString("dd.MM.yyyy")+"-"+lastDate.toString("dd.MM.yyyy")+"");
    } else if (comboBoxForStatCategory->currentIndex()==1) {
        chartProdStatCategory->setTitle("Сумма продаж за период "+firstDate.toString("dd.MM.yyyy")+"-"+lastDate.toString("dd.MM.yyyy")+" (руб.)");
    }

    comboBoxForStatCategory->setEnabled(true);

}



bool QStatisticTabWidget::isDatesValid()
{
    daysCount = calendarStart->date().daysTo(calendarFinish->date());
    if (daysCount < 0)
        return false;

    return true;
}

