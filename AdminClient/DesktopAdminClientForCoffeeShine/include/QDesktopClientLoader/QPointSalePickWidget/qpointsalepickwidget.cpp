#include "qpointsalepickwidget.h"





QPointSalePickWidget::QPointSalePickWidget(QCoffeeClientPlugin *currentPlugin_, QWidget* parent): QWidget(parent) {

    currentPlugin = currentPlugin_;
    createForm();
}
QPointSalePickWidget::QPointSalePickWidget(QCoffeeClientPlugin *currentPlugin_,bool isTabWidget, QWidget* parent): QWidget(parent) {
     currentPlugin = currentPlugin_;
     this->isTabWidget=1;
     createFormTab();
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
    connect(btnAllPointsales,&QPushButton::clicked,this,[=](){
        if (!isAllPointSalesPicked) {
            isAllPointSalesPicked=1;
            btnAllPointsales->setText("Убрать все категории");
        } else {
            isAllPointSalesPicked=0;
             btnAllPointsales->setText("Выбрать все категории");
        }

    });

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
    connect(btnDecline,&QPushButton::clicked,this,&QPointSalePickWidget::signelBtnCancel);
    lFoButtons->addWidget(btnAccept);
    lFoButtons->addWidget(btnDecline);



    mainLayout->addWidget(btnAllPointsales);
    mainLayout->addLayout(lFoButtons);

    fillForm();
    setMinimumSize(399,300);
}


void QPointSalePickWidget::createFormTab() {
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


    fillFormTab();
    setMinimumSize(399,300);
}



void QPointSalePickWidget::fillForm() {

    QVector<QCoffeePointSale> pointsSaleInfo =  currentPlugin->getListPointSale();

    for (int i = 0; i < pointsSaleInfo.count(); ++i)
    {
        listWidgetItem = new QListWidgetItem(listWidgetPickPointSale);
        listWidgetPickPointSale->addItem (listWidgetItem);
        pointSaleWgItem = new QPointSalePickWidgetItem(currentPlugin,pointsSaleInfo.at(i),this);


        connect(btnAllPointsales,&QPushButton::clicked,pointSaleWgItem,&QPointSalePickWidgetItem::slotmousePressEvent);
        // connect(pointSaleWgItem, &QPointSalePickWidgetItem::signalPointSalePicked, this, [=](const QCoffeePointSale &pS) {
        //     emit signalPointSalePicked(pS);
        // });


        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (pointSaleWgItem->sizeHint());
        listWidgetPickPointSale->setItemWidget(listWidgetItem, pointSaleWgItem);
    }


}



void QPointSalePickWidget::fillFormTab() {

    QVector<QCoffeePointSale> pointsSaleInfo =  currentPlugin->getListPointSale();
    listWidgetPickPointSale->clear();
    for (int i = 0; i < pointsSaleInfo.count(); ++i)
    {
        listWidgetItem = new QListWidgetItem(listWidgetPickPointSale);
        listWidgetPickPointSale->addItem (listWidgetItem);
        pointSaleWgItem = new QPointSalePickWidgetItem(currentPlugin,pointsSaleInfo.at(i),1,this);
        connect(pointSaleWgItem,&QPointSalePickWidgetItem::signalOpenEditPickWidget,this,[=](QCoffeePointSale &pointSale){
            qDebug()<<"isSlot";
            emit signalOpenEditPickWidget(pointSale);
        });
        // connect(pointSaleWgItem, &QPointSalePickWidgetItem::signalPointSalePicked, this, [=](const QCoffeePointSale &pS) {
        //     emit signalPointSalePicked(pS);
        // });


        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (pointSaleWgItem->sizeHint());
        listWidgetPickPointSale->setItemWidget(listWidgetItem, pointSaleWgItem);
    }


}
