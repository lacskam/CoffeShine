#ifndef QCATEGORYITEM_H
#define QCATEGORYITEM_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"

class QCategoryItem : public QWidget
{
    Q_OBJECT
public:
    explicit QCategoryItem(QCoffeeCategoryInfo info, QCoffeeClientPlugin* plugin_, QWidget *parent,int isEx =0);
    bool isPicked = false;
    int isEx_;
    QCoffeeCategoryInfo categoryInfo;
private:
    QCoffeeClientPlugin * currentPlugin;


    QLabel *categoryName;
    QHBoxLayout *hblCategory;

protected:

    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*e);

signals:

     void signalOpenEditCategoryWidget(QCoffeeCategoryInfo);
};

#endif // QCATEGORYITEM_H
