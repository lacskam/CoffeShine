#ifndef QPOINTSALEPICKWIDGETITEM_H
#define QPOINTSALEPICKWIDGETITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class QPointSalePickWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QPointSalePickWidgetItem(QCoffeeClientPlugin *currentPlugin_, QCoffeePointSale pointSaleInfo_, QWidget *parent = nullptr);
    explicit QPointSalePickWidgetItem(QCoffeeClientPlugin *currentPlugin_, QCoffeePointSale pointSaleInfo_,bool isTabWidget, QWidget *parent = nullptr);
    bool isPicked=false;
        QCoffeePointSale  pointSaleInfo;

protected:
    virtual void paintEvent(QPaintEvent*);
   // virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
    virtual void mousePressEvent(QMouseEvent*);

private:
    bool isAnimating = false;
     QSize originalSize;

    QPropertyAnimation *animation;
    bool isTabWidget =0;
    QWidget *parent_;
    QLabel * pointSaleName;
    QSize tempS;
    QPoint tempP;
    QWidget *parent;

    QAction *actionAddHost;
    QAction *actionEditHost;
    QAction *actionDeleteHost;

    QCoffeeClientPlugin *currentPlugin;


    QLabel *pointSaleNamef ;


    void sizeScale(QSize sizeT);
signals:

     void signalPointSalePicked(const QCoffeePointSale &pointSale);
    void signalOpenEditPickWidget(QCoffeePointSale &pointSale);

public slots:
     void slotmousePressEvent();

private slots:
    void slotShowMenuContext(QPoint pos);



};

#endif // QPOINTSALEPICKWIDGETITEM_H









