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

    QDateEdit * calendarStart;
    QDateEdit * calendarFinish;
    QChartView *chartView;
    QComboBox * chartsComboBox;
    QStringList chartList;

    void createForm();
    bool isDatesValid();
private slots:
    void createMoneyStatictic();
};

#endif // QSTATISTICTABWIDGET_H
