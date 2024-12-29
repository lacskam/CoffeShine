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



    QVBoxLayout *vb;
    QHBoxLayout *hb;
    QLabel *leFirstDateLabel;
    QDateEdit *leFirstDate;
    QLabel *leFSecondDateLabel;
    QDateEdit *leSecondDate;
    QComboBox *drinksComboBox;
    QSpinBox *lePickedProd;
    QValueAxis *axisY;
    QDateTimeAxis *axisX;
    QMap<QDateTime, qint32> *mapSaless;
    QPushButton *btn;
    QChart *chart;
    QChartView *chartView2;
    QSplineSeries *series;
private slots:
    void createMoneyStatictic();
};

#endif // QSTATISTICTABWIDGET_H
