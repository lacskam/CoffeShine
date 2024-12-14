#include "qpointsalepickwidget.h"





QPointSalePickWidget::QPointSalePickWidget(QCoffeeClientPlugin *currentPlugin_, QWidget* parent): QWidget(parent) {

    currentPlugin = currentPlugin_;
    createForm();
}



void QPointSalePickWidget::createForm() {
    mainLayout = new QVBoxLayout(this);
    lFoButtons = new QHBoxLayout();
    scrolAreaForPoints = new QScrollArea;

    listWidgetPickPointSale = new QListWidget;

    listWidgetPickPointSale->setFlow(QListView::LeftToRight);    //Lays out horizontally instead of vertically
    listWidgetPickPointSale->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
    listWidgetPickPointSale->setGridSize(QSize(180, 230));       //This is an arbitrary value, but it forces the layout into a grid
    listWidgetPickPointSale->setSpacing(10);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    listWidgetPickPointSale->setViewMode(QListView::IconMode);

    mainLayout->addWidget(listWidgetPickPointSale);
    btnAllPointsales = new QPushButton("Выбрать все категории");

    btnAccept = new QPushButton("Выбрать");
    connect(btnAccept, &QPushButton::clicked, this, [=]() {


        QVector<int> idsPickedPointSales;


        for (int i = 0; i < listWidgetPickPointSale->count(); ++i) {
            QListWidgetItem *listItem = listWidgetPickPointSale->item(i);


            QPointSalePickWidgetItem *pointSaleWidget = qobject_cast<QPointSalePickWidgetItem*>(listWidgetPickPointSale->itemWidget(listItem));


            if (pointSaleWidget && pointSaleWidget->isPicked) {
                idsPickedPointSales.push_back(pointSaleWidget->pointSaleInfo.id);
            }
        }

        emit signalPointSalePicked(idsPickedPointSales);

    });

    btnDecline = new QPushButton("Отмена");
    lFoButtons->addWidget(btnAccept);
    lFoButtons->addWidget(btnDecline);



    mainLayout->addWidget(btnAllPointsales);
    mainLayout->addLayout(lFoButtons);

    fillForm();
    setMinimumSize(399,300);
}




void QPointSalePickWidget::fillForm() {

    QVector<QCoffeePointSale> pointsSaleInfo =  currentPlugin->getListPointSale();

    for (int i = 0; i < pointsSaleInfo.count(); ++i)
    {
        listWidgetItem = new QListWidgetItem(listWidgetPickPointSale);
        listWidgetPickPointSale->addItem (listWidgetItem);
        pointSaleWgItem = new QPointSalePickWidgetItem(currentPlugin,pointsSaleInfo.at(i),this);

        // connect(pointSaleWgItem, &QPointSalePickWidgetItem::signalPointSalePicked, this, [=](const QCoffeePointSale &pS) {
        //     emit signalPointSalePicked(pS);
        // });


        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (pointSaleWgItem->sizeHint());
        listWidgetPickPointSale->setItemWidget(listWidgetItem, pointSaleWgItem);
    }


}

