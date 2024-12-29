#include "qstatistictabwidget.h"

QStatisticTabWidget::QStatisticTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent)
{
    currentPlugin = plugin_;
    chartList << "Деньги"<<"Продажи";
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

    tabWidget->addTab(stat2,"Напитки");
    tabWidget->addTab(new QWidget(),"Категории");
}


void QStatisticTabWidget::createDrinkStatForm() {

    stat2 = new QWidget();
    vb = new QVBoxLayout(stat2);
    hb = new QHBoxLayout();


    leFirstDateLabel = new QLabel("C: ");
    leFirstDate = new QDateEdit;

    leFSecondDateLabel = new QLabel("До: ");
    leSecondDate = new QDateEdit;

    drinksComboBox = new QComboBox;
    drinksComboBox->setMinimumWidth(200);

    lePickedProd = new QSpinBox;
    lePickedProd->setRange(0, 153);

    btn = new QPushButton("Показать");

    axisY = new QValueAxis();
    axisX = new QDateTimeAxis();
    axisX->setTickCount(22);
    axisX->setFormat("dd.MM.yyyy");
    axisX->setTitleText("дата");
    axisY->setTitleText("продажи");

    mapSaless = new QMap<QDateTime, qint32>;

    chart = new QChart();
    series = new QSplineSeries();
    chartView = new QChartView(chart);


    hb->addWidget(leFirstDateLabel);
    hb->addWidget(leFirstDate, Qt::AlignLeft);
    hb->addWidget(leFSecondDateLabel);
    hb->addWidget(leSecondDate, Qt::AlignLeft);
    hb->addWidget(drinksComboBox);
    hb->addWidget(btn);

    vb->addWidget(chartView);
    vb->addLayout(hb);

    QList<QDateTime> dates;
    QList<qint32> values;

    for (QMap<QDateTime, qint32>::const_iterator i = mapSaless->begin(); i != mapSaless->end(); ++i) {
        dates.push_back(i.key());
        values.push_back(i.value());
    }

    for (int i = 0; i < dates.size(); ++i) {
        series->append(dates[i].toMSecsSinceEpoch(), values[i]);
    }


    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
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

bool QStatisticTabWidget::isDatesValid()
{
    daysCount = calendarStart->date().daysTo(calendarFinish->date());
    if (daysCount < 0)
        return false;

    return true;
}

