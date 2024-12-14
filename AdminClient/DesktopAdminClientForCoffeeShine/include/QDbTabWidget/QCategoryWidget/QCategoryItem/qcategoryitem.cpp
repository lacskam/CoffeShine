#include "qcategoryitem.h"



QCategoryItem::QCategoryItem(QCoffeeCategoryInfo info, QCoffeeClientPlugin *plugin_,  QWidget *parent, int isEx) {


    currentPlugin = plugin_;
    categoryInfo = info;
    isEx_ = isEx;
    hblCategory = new QHBoxLayout(this);


    categoryName = new QLabel(categoryInfo.name);
    categoryName->setAlignment(Qt::AlignLeft);
    categoryName->setFont(QFont("Sans", 10));
    categoryName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    hblCategory->addWidget(categoryName);



    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);




}


void QCategoryItem::mouseDoubleClickEvent(QMouseEvent *)
{
    if (!isEx_) {
        emit signalOpenEditCategoryWidget(categoryInfo);

    }
}
void QCategoryItem::mousePressEvent(QMouseEvent* e) {
    if (isEx_) {
        if (isPicked) {

            isPicked = false;
            categoryName->setStyleSheet("color: rgb(255, 255, 255)");
        } else {

            isPicked = true;
            categoryName->setStyleSheet("color: rgb(0, 204, 255)");



        }


        this->update();
    }

    QWidget::mousePressEvent(e);
}


