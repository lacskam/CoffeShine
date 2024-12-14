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
    QVBoxLayout *mainLayout;
    QHBoxLayout *lFoButtons;
    QScrollArea *scrolAreaForPoints;
    QListWidget *listWidgetPickPointSale;
    QListWidgetItem *listWidgetItem;
    QPointSalePickWidgetItem *pointSaleWgItem;

    QPushButton *btnAllPointsales;
    QPushButton *btnAccept;
    QPushButton *btnDecline;
protected:



signals:
    void signalPointSalePicked(const QVector<int> &pointSales);
public slots:



private slots:

};









#endif // QPOINTSALEPICKWIDGET_H
