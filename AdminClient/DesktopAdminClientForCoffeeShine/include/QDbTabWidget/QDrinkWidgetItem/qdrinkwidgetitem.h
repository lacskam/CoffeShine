#ifndef QDRINKWIDGETITEM_H
#define QDRINKWIDGETITEM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "../QEditDrinkWidget/qeditdrinkwidget.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
class QDrinkWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QDrinkWidgetItem(QCoffeeDrinkInfo,QCoffeeClientPlugin*,qint32 flagAnim,QListWidget* parentWidget = nullptr,QWidget *parent = nullptr);
      QListWidget* parentWidget;
    QCoffeeDrinkInfo getDrinkInfo();
    bool isPickedForCategory;


    void setParentListWidget(QListWidget* widget) {
        parentWidget = widget;
    }
    QSize originalSize;
    QListWidget* getParentListWidget() const {
        return parentWidget;
    }

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);
    virtual void mousePressEvent(QMouseEvent*);

private:
    bool isAnimating = false;
    QPropertyAnimation *animation;
    QDeleteDialog *deletedialogIt;
    QCoffeeClientPlugin * currentPlugin;
    QLabel * drinkName;
    QSize tempS;
    QPoint tempP;
      QWidget *parent;
    QAction *actionAddHost;
    QAction *actionEditHost;
    QAction *actionDeleteHost;
    qint32 flagAnim;
    QCoffeeDrinkInfo drinkInfo;
    QLabel *drinkNamef ;
    bool isCliced =0;

    void sizeScale(QSize sizeT);
signals:
    void signalOpenEditDrinkWidget(QCoffeeDrinkInfo);
    void signalUpdateListWidget();
    void signalClicked();


public slots:
    void updateCoffeWidget(QCoffeeDrinkInfo);
private slots:
    void slotShowMenuContext(QPoint pos);
    void slotDeleteDrink(qint32 id);

};

#endif // QDRINKWIDGETITEM_H
