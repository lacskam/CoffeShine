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
    explicit QCategoryItem(QCoffeeCategoryInfo info, QCoffeeClientPlugin* plugin_, QWidget *parent);
private:
    QCoffeeClientPlugin * currentPlugin;
    QCoffeeCategoryInfo categoryInfo;

    QLabel *categoryName;
    QHBoxLayout *hblCategory;

protected:

    virtual void mouseDoubleClickEvent(QMouseEvent*);

signals:

     void signalOpenEditCategoryWidget(QCoffeeCategoryInfo);
};

#endif // QCATEGORYITEM_H
