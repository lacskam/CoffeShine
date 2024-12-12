#ifndef QPOINTSALEPICKWIDGET_H
#define QPOINTSALEPICKWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "QPointSalePickWidgetItem/qpointsalepickwidgetitem.h"

class QPointSalePickWidget: public QWidget
{
    Q_OBJECT
public:
    explicit QPointSalePickWidget(QCoffeeClientPlugin *currentPlugin_, QWidget* parent = nullptr);
    void createForm();
    void fillForm();

private:

    QCoffeeClientPlugin *currentPlugin;
    QHBoxLayout *mainLayout;
    QScrollArea *scrolAreaForPoints;
    QListWidget *listWidgetPickPointSale;
    QListWidgetItem *listWidgetItem;
    QPointSalePickWidgetItem *pointSaleWgItem;


signals:
    void signalPointSalePicked(const QCoffeePointSale &pointSale);
public slots:



private slots:

};









#endif // QPOINTSALEPICKWIDGET_H
