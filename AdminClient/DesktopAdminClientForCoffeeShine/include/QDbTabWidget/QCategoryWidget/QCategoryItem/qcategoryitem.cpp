#include "qcategoryitem.h"



QCategoryItem::QCategoryItem(QCoffeeCategoryInfo info, QCoffeeClientPlugin *plugin_,  QWidget *parent) {


    currentPlugin = plugin_;
    categoryInfo = info;

    hblCategory = new QHBoxLayout(this);

    categoryName = new QLabel(categoryInfo.name);
    categoryName->setAlignment(Qt::AlignCenter);
    categoryName->setFont(QFont("Sans",12));

    hblCategory->addWidget(categoryName);


}


void QCategoryItem::mouseDoubleClickEvent(QMouseEvent *)
{
    emit signalOpenEditCategoryWidget(categoryInfo);
}
