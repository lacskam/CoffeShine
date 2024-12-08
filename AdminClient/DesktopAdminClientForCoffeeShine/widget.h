#ifndef WIDGET_H
#define WIDGET_H

#include <QVBoxLayout>
#include <QTabWidget>
#include <QWidget>
#include <QCloseEvent>
#include "include/QHTabWidget/qhtabwidget.h"
#include "include/QDbTabWidget/qdbtabwidget.h"
#include "include/QStatisticTabWidget/qstatistictabwidget.h"
#include "include/QNnTabWidget/qnntabwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    Widget(QSettings *settings_,QUnNetProtokolCLient *client_,QCoffeeClientPlugin *plugin_,QWidget *parent = 0);
    ~Widget();

private:
    QSettings *settings;
    QUnNetProtokolCLient *client;
    QCoffeeClientPlugin *plugin;

    QVBoxLayout *mainLayout;
    QHTabWidget *tabWidget;
    QDbTabWidget *dbTabWidget;
    QStatisticTabWidget *statTabWidget;
    QNnTabWidget *nnTabWigdet;

    void loadSettings();
    void saveSettings();
    void createForm();

    void closeEvent (QCloseEvent *event);
signals:
    void signalCloseProgramm();
};

#endif // WIDGET_H
