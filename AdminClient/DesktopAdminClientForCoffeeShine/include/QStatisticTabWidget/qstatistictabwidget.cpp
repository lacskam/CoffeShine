#include "qstatistictabwidget.h"

QStatisticTabWidget::QStatisticTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent)
{
    currentPlugin = plugin_;
    chartList << "Деньги";
    createForm();
}

void QStatisticTabWidget::createForm()
{
    mainLayout = new QVBoxLayout(this);
    QGroupBox * groupBox = new QGroupBox();
    mainLayout->addWidget(groupBox);
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

    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    layForGroupBox->addWidget(chartView,1);
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

