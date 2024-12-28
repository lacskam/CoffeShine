#ifndef PLOT_H
#define PLOT_H
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QDateEdit>
#include <QFileDialog>
#include<QLineEdit>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QtCharts>
#include "../QNnFileProcessing/QNnDbProcessing.h"
#include <QPainter>
#include <QLabel>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "QProductsStatPred/qproductstatisticprediction.h"
#include "QLoadingWidget/qloadingwidget.h"
#include <algorithm>
#include <QWidget>
#include "QNnSettingsWidget/qnnsettingswidget.h"
QT_CHARTS_USE_NAMESPACE

class QNnTabWidget : public QWidget {
    Q_OBJECT
public:
    explicit QNnTabWidget(QCoffeeClientPlugin* plugin_,QWidget *parent = nullptr);

signals:
    void requestPrediction(QDate startDate, QDate endDate, quint32 pickedProduct);
public slots:

    void updateChart(DB *dbase1, QValueAxis *axisY, QDateTimeAxis *axisX,
                     QSpinBox *lePickedProd, QDateEdit *leFirstDate, QDateEdit *leSecondDate,QMap<QDateTime,qint32> *mapSaless);

    void getfilePlot(QSpinBox *lePickedProd,QMap<QDateTime,qint32> *mapSales);

    void openfile(QValueAxis *axisY, QDateTimeAxis *axisX);

    void updateNN(QMap<QDateTime, float> predictionResults);

    void lineMove(QMap<QDateTime,qint32> *mapSaless);

    void lineMoveNN(QMap<QDateTime,float> *mapSaless);
    void GetStatPrediction(QMap<qint32, float> predictionResults);
    void updateStatWg();
    void openSettingsDialog();

private:


    QVBoxLayout *mainLayout;
    QGroupBox *groupBox;
    QCoffeeClientPlugin *currentPlugin;
    DB *dbase;

    QMap<QDateTime,qint32> *mapSaless;
    QMap<QDateTime,float> *mapNN;


    QWidget *productPrediction;
    QWidget *productStatisticPrediction;
    QStackedWidget *stackedWidget ;
    QComboBox *chartsComboBox;
     QVBoxLayout *vbstacked;
    QHBoxLayout *hblforhead;
     QPushButton *buttonettings;

    quint32 *idForGetStatStart;
     quint32 *idForGetStatEnd;
    QStringList *categories;

    QWidget *main;
    QChart *chart;
     QChart *chartnn;
    QHBoxLayout *hblcharts;
    QPushButton *plotNNButton;

    QChartView *chartView;
    QChartView *chartViewNN;
    QSplineSeries *ser;
    QSplineSeries *serNN;

    QValueAxis *axisYnn;
    QDateTimeAxis *axisXnn;

    QHBoxLayout *hb;
    QVBoxLayout *vb;

    QDateEdit *leFirstDate;
    QDateEdit *leSecondDate;
    QSpinBox *lePickedProd;
    QPushButton *btn;

    QPushButton *btnGetFile;
    QPushButton *btnOpenFile;

    QSlider *sliderBase;
    QSlider *sliderNN;
    int *slider_range;
    int *slider_rangeNN;
    QHBoxLayout *vbs;

    QGraphicsLineItem *line;
    QGraphicsLineItem *lineNN;
    int x1;
    int y1;
    int x2;
    int y2;
    QLabel *labelValue;
    QLabel *labelValueNN;


    QVBoxLayout *vbstPred;
    QHorizontalBarSeries *series;
    QBarSet *temp;
    QChart *chartProdStat;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartViewProdStat;
    QScrollArea *scrollArea;
    QPushButton *btnLoadStat;

    void createForm(QWidget *parent = 0, qint32 *pickedprod = nullptr, QList<QDate> *endDAte= nullptr);
    void createStatForm();
    void next();


    QWidget *loadingWg;
    QVBoxLayout *lLoading;
    LoadingWidget *loading;
    bool isLoadingPred;
    bool isLoadingStat;


    QNnSettingsWidget *settingsWidget;



};

#endif // PLOT_H
