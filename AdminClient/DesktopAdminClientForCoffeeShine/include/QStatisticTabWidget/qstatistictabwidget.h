#ifndef QSTATISTICTABWIDGET_H
#define QSTATISTICTABWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QCalendarWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QDateEdit>
#include <QtCharts>
#include <QTabWidget>


#include <QSpinBox>
#include <QPushButton>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QSplineSeries>
#include <QMap>
#include <QDateTime>

#include <../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h>

 #include "../QCustomScrollArea/QCustomScrollArea.h"

class QStatisticTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QStatisticTabWidget(QCoffeeClientPlugin* plugin_,QWidget *parent = nullptr);

private:
    QCoffeeClientPlugin *currentPlugin;
    qint64 daysCount;
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;

    QDateEdit * calendarStart;
    QDateEdit * calendarFinish;
    QChartView *chartView;
    QComboBox * chartsComboBox;
    QStringList chartList;


    QWidget *stat1;
    QWidget *stat2;
    QWidget *stat3;


    void createForm();
    void createDrinkStatForm();
    void createCategoryStatForm();
    bool isDatesValid();





    //drinkStatWidget
    QComboBox *comboBoxForDrinkStat;
    QVBoxLayout *vbstPred;
    QHorizontalBarSeries *series;
    QBarSet *temp;
    QChart *chartProdStat;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartViewProdStat;
    CustomScrollArea *scrollArea;
    QVBoxLayout *vb;
    QHBoxLayout *hb;
    QLabel *leFirstDateLabel;
    QDateEdit *leFirstDate;
    QLabel *leFSecondDateLabel;
    QDateEdit *leSecondDate;
    QComboBox *drinksComboBox;
    QSpinBox *lePickedProd;
    QPushButton *btn;


    //categoryStatWidget
    QComboBox *comboBoxForStatCategory;
    QVBoxLayout *vbstPredCategory;
    QHorizontalBarSeries *seriesCategory;
    QBarSet *tempCategory;
    QChart *chartProdStatCategory;
    QBarCategoryAxis *axisXCategory;
    QValueAxis *axisYCategory;
    QChartView *chartViewProdStatCategory;
    CustomScrollArea *scrollAreaCategory;
    QVBoxLayout *vbCategory;
    QHBoxLayout *hbCategory;
    QLabel *leFirstDateLabelCategory;
    QDateEdit *leFirstDateCategory;
    QLabel *leFSecondDateLabelCategory;
    QDateEdit *leSecondDateCategory;
    QComboBox *drinksComboBoxCategory;
    QSpinBox *lePickedProdCategory;
    QPushButton *btnCategory;


    // QVBoxLayout *vbforCategory;
    // QHBoxLayout *hbforCategory;
    // QLabel *leFirstDateLabelforCategory;
    // QDateEdit *leFirstDateforCategory;
    // QLabel *leFSecondDateLabelforCategory;
    // QDateEdit *leSecondDateforCategory;
    // QComboBox *drinksComboBoxforCategory;
    // QSpinBox *lePickedProdforCategory;
    // QValueAxis *axisYforCategory;
    // QDateTimeAxis *axisXforCategory;
    // QMap<QDateTime, qint32> *mapSalessforCategory;
    // QPushButton *btnforCategory;
    // QChart *chartforCategory;
    // QChartView *chartViewforCategory;
    // QSplineSeries *seriesforCategory;



private slots:
    void createMoneyStatictic();
    void createDrinkStatistic();
    void createCategoryStatistic();
};



#endif // QSTATISTICTABWIDGET_H
