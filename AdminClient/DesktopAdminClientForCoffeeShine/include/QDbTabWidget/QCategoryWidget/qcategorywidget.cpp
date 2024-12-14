#include "qcategorywidget.h"

QCategoryWidget::QCategoryWidget(QCoffeeClientPlugin *plugin_,QCoffeeCategoryInfo *currentCat, QWidget *parent,int pointSaleId) {

    currentPlugin = plugin_;

    currentPointSaleId = pointSaleId;
    currentCategory = new QCoffeeCategoryInfo;


    *currentCategory = *currentCat;
    mainLayout = new QVBoxLayout(this);
    cId = currentCategory->id;

    cDrinks = currentPlugin->getListDrink(cId);

     nameFirst = currentCategory->name;
     descriptionFirst = currentCategory->description;
    linkedDrinksFirst = cDrinks;

    QLabel *mainLabel = new QLabel("Название");


    nameCategoryEdit = new QLineEdit;
    nameCategoryEdit->setText(currentCategory->name);


      mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(nameCategoryEdit);
      mainLayout->addSpacing(10);
    QLabel *descriptionLabel = new QLabel("Описание");

    descriptionCategoryEdit = new QTextEdit;
    descriptionCategoryEdit->setText(currentCategory->description);


    mainLayout->addWidget(descriptionLabel);

    mainLayout->addWidget(descriptionCategoryEdit);
    mainLayout->addSpacing(10);

    QLabel *drinkListLabel = new QLabel("Список напитков");
    mainLayout->addWidget(drinkListLabel);


    buttonAddNewDrinkToCategory = new QPushButton("Добавить напиток");
    connect(buttonAddNewDrinkToCategory,SIGNAL(clicked()),this,SLOT(createDrinkPickDialog()));

    // кнопка добавить напиток при которое появляется всплывающее окно с выбором напитка


    QHBoxLayout *hblDrink = new QHBoxLayout;
    drinkScrol = new QScrollArea;

    drinkListWidget = new QListWidget();

    drinkListWidget->setFlow(QListView::LeftToRight);    //Lays out horizontally instead of vertically
    drinkListWidget->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
    drinkListWidget->setGridSize(QSize(165, 185));      //This is an arbitrary value, but it forces the layout into a grid
    drinkListWidget->setSpacing(30);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    drinkListWidget->setViewMode(QListView::IconMode);   //And the most important part:


    hblDrink->addWidget(drinkListWidget);

    drinkScrol->setLayout(hblDrink);
    mainLayout->addWidget(drinkScrol);

    mainLayout->addWidget(buttonAddNewDrinkToCategory);
    drinkScrol->setMinimumHeight(500);

    mainLayout->addSpacing(10);
    if (this->currentCategory->id>0)
        updateDrinkItems();

     QHBoxLayout *hblButton= new QHBoxLayout;

    btnSave = new QPushButton("Сохранить");
     btnCancel = new QPushButton("Отмена");
    connect(btnCancel,SIGNAL(clicked()),parent,SLOT(slotCloseCategoryEditWidget()));
    connect(btnSave,SIGNAL(clicked()),this,SLOT(sendCategory()));


    hblButton->addWidget(btnSave);
    hblButton->addWidget(btnCancel);

    if (currentPointSaleId==0) {
        labelDelete = new QLabel("Удаление категории");
        QPushButton *buttonDelete = new QPushButton("Удалить");
         connect(buttonDelete,SIGNAL(clicked()),this,SLOT(slotDeleteCategory()));
        if (currentCategory->id>0) {
            mainLayout->addSpacing(10);
            mainLayout->addWidget(labelDelete);

            mainLayout->addWidget(buttonDelete);
        }
    } else {
        labelUnlink = new QLabel("Отвязать категорию от точки продажи "+currentPlugin->getPointSaleInfo(currentPointSaleId).name);
        buttonUnlink = new QPushButton("Отвязать");

        connect(buttonUnlink,&QPushButton::clicked,this,[=](){
            QVector<int> iDsDrinks = currentPlugin->getIdDrinkForCategory(currentCategory->id);

            for (int i =0;i< currentCategory->idPointSale.size();i++) {
                if (currentCategory->idPointSale.at(i)==currentPointSaleId) {
                    currentCategory->idPointSale.removeAt(i);
                }
            }

            currentPlugin->crudCategoryInfo(*currentCategory,iDsDrinks,0x02);
              emit btnCancel->clicked();
        });

        if (currentCategory->id>0) {
            mainLayout->addSpacing(10);
            mainLayout->addWidget(labelUnlink);

            mainLayout->addWidget(buttonUnlink);
        }
    }








    mainLayout->addSpacing(20);

    mainLayout->addStretch();
    mainLayout->addLayout(hblButton);


}




void QCategoryWidget::createDrinkPickDialog() {


    vDrinkAddres = new QVector<QDrinkWidgetItem*>;
    QVector<QCoffeeDrinkInfo> currentCategoryDrinkInfo = currentPlugin->getListDrink();


    lDrinkPick = new QLabel("Выберите напитки для добавления в категорию.");
    hblDPick = new QHBoxLayout;
    vblDPick = new QVBoxLayout;
    hblDPickListW = new QHBoxLayout;
    bntA = new QPushButton("Добавить");
    btnD = new QPushButton("Отмена");

    drinkPickDialog = new QDialog;
    connect(bntA,SIGNAL(clicked()),this,SLOT(drinkPickDialogAccepted()));
    connect(btnD,SIGNAL(clicked()),drinkPickDialog,SLOT(close()));
    drinkPickScrolarea = new QScrollArea;
    drinPickkListWidget = new QListWidget();

    drinPickkListWidget->setFlow(QListView::LeftToRight);    //Lays out horizontally instead of vertically
    drinPickkListWidget->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
    drinPickkListWidget->setGridSize(QSize(170, 190));      //This is an arbitrary value, but it forces the layout into a grid
    drinPickkListWidget->setSpacing(30);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    drinPickkListWidget->setViewMode(QListView::IconMode);



    hblDPickListW->addWidget(drinPickkListWidget);
    drinkPickScrolarea->setLayout(hblDPickListW);
    drinkPickDialog->setLayout(vblDPick);
    vblDPick->addWidget(lDrinkPick);
    vblDPick->addWidget(drinkPickScrolarea);
    vblDPick->addLayout(hblDPick);
    hblDPick->addWidget(bntA);
    hblDPick->addWidget(btnD);

    for (int i=0;i<currentCategoryDrinkInfo.count();i++)
    {
        qDebug()<<currentCategoryDrinkInfo.at(i).id;
        QListWidgetItem *listWidgetItem = new QListWidgetItem(drinPickkListWidget);
        drinPickkListWidget->addItem (listWidgetItem);
            //Creating an object of the designed widget which is to be added to the listwidget

        QVector<qint32> cDrinksId;

        for (int i=0;i<cDrinks.size();i++) {
            cDrinksId.push_back(cDrinks.at(i).id);
        }

        if (cDrinksId.contains(currentCategoryDrinkInfo.at(i).id)) {
            drinkPickWidgetItemDio = new QDrinkWidgetItem(currentCategoryDrinkInfo.at(i),currentPlugin,3,drinPickkListWidget);
            vDrinkAddres->push_back(drinkPickWidgetItemDio);
        } else {
            drinkPickWidgetItemDio = new QDrinkWidgetItem(currentCategoryDrinkInfo.at(i),currentPlugin,4,drinPickkListWidget);
            vDrinkAddres->push_back(drinkPickWidgetItemDio);
        }

        // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalOpenEditDrinkWidget(QCoffeeDrinkInfo)),this,SLOT(QDrinkWidgetItem::slotOpenDrinkEditWidget(QCoffeeDrinkInfo)));
        // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalUpdateListWidget()),this,SLOT(QDrinkWidgetItem::updateDrinkItems()));
        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (drinkPickWidgetItemDio->sizeHint ());
        drinPickkListWidget->setItemWidget(listWidgetItem, drinkPickWidgetItemDio);
    }
    drinkPickDialog->setMinimumSize(570,500);
    drinkPickDialog->show();


}

void QCategoryWidget::drinkPickDialogAccepted() {

     QVector<QCoffeeDrinkInfo> currentCategoryDrinkInfo;

    for (int i=0;i<vDrinkAddres->size();i++) {
         if (vDrinkAddres->at(i)->isPickedForCategory) {
             currentCategoryDrinkInfo.push_back(vDrinkAddres->at(i)->getDrinkInfo());
             qDebug()<<vDrinkAddres->at(i)->getDrinkInfo().id;
         }
    }


    vDrinkAddres->clear();


    cDrinks = currentCategoryDrinkInfo;


    drinkListWidget->clear();



    for (int i=0;i<currentCategoryDrinkInfo.count();i++)
    {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(drinkListWidget);
        drinkListWidget->addItem (listWidgetItem);
            //Creating an object of the designed widget which is to be added to the listwidget
        drinkWidgetItem = new QDrinkWidgetItem(currentCategoryDrinkInfo.at(i),currentPlugin,2,drinkListWidget);
        // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalOpenEditDrinkWidget(QCoffeeDrinkInfo)),this,SLOT(QDrinkWidgetItem::slotOpenDrinkEditWidget(QCoffeeDrinkInfo)));
        // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalUpdateListWidget()),this,SLOT(QDrinkWidgetItem::updateDrinkItems()));
        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (drinkWidgetItem->sizeHint ());
        drinkListWidget->setItemWidget(listWidgetItem, drinkWidgetItem);
    }



    drinkPickDialog->close();

}

void QCategoryWidget::updateDrinkItems()
{
    drinkListWidget->clear();

    QVector<QCoffeeDrinkInfo> currentCategoryDrinkInfo = currentPlugin->getListDrink(currentCategory->id);

    for (int i=0;i<currentCategoryDrinkInfo.count();i++)
    {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(drinkListWidget);
        drinkListWidget->addItem (listWidgetItem);
            //Creating an object of the designed widget which is to be added to the listwidget
        drinkWidgetItem = new QDrinkWidgetItem(currentCategoryDrinkInfo.at(i),currentPlugin,2,drinkListWidget);
       // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalOpenEditDrinkWidget(QCoffeeDrinkInfo)),this,SLOT(QDrinkWidgetItem::slotOpenDrinkEditWidget(QCoffeeDrinkInfo)));
       // connect(drinkWidgetItem,SIGNAL(QDrinkWidgetItem::signalUpdateListWidget()),this,SLOT(QDrinkWidgetItem::updateDrinkItems()));
            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (drinkWidgetItem->sizeHint ());
        drinkListWidget->setItemWidget(listWidgetItem, drinkWidgetItem);
    }
}

void QCategoryWidget::paintEvent(QPaintEvent *e)
{
    this->resize(parentWidget()->width(),this->height());

}

bool QCategoryWidget::checkChanges()
{
    qDebug()<<"Проверка изменений";


    if (nameCategoryEdit->text() != nameFirst)
        return true;

    if (descriptionCategoryEdit->toPlainText() != descriptionFirst)
        return true;

    for (int i=0;i<cDrinks.size();i++) {
        if (cDrinks.size()==linkedDrinksFirst.size()) {
            if (linkedDrinksFirst.at(i).id!=cDrinks.at(i).id) {
                return true;
            }
        } else return true;

    }


    return false;
}

void QCategoryWidget::sendCategory() {

    QRegExp validReg("^[a-zA-Zа-яА-Я0-9 \"-]*$");
    int valid=0;

    if (!validReg.exactMatch(descriptionCategoryEdit->toPlainText())) {

        descriptionCategoryEdit->setStyleSheet("color: rgb(255, 0, 0)");
        connect(descriptionCategoryEdit, &QTextEdit::textChanged, this, [=]() {descriptionCategoryEdit->setStyleSheet("color: rgb(255, 255, 255)");});
        valid++;

    }

    if (!validReg.exactMatch(nameCategoryEdit->text())) {

        nameCategoryEdit->setStyleSheet("color: rgb(255, 0, 0)");
        connect(nameCategoryEdit, &QLineEdit::textChanged, this, [=]() {nameCategoryEdit->setStyleSheet("color: rgb(255, 255, 255)");});
        valid++;


    }
    if (valid>0) {
        return;
    } else {

    if (checkChanges()) {

        currentCategory->name = nameCategoryEdit->text();
        currentCategory->description = descriptionCategoryEdit->toPlainText();

        QVector<qint32> cDrinksId;

        for (int i=0;i<cDrinks.size();i++) {
            cDrinksId.push_back(cDrinks.at(i).id);
        }

        if (currentCategory->id == -1) {

            currentCategory->color.setBlue(1);
            currentCategory->color.setRed(1);
            currentCategory->color.setGreen(1);

            PsPw = new QPointSalePickWidget(currentPlugin);

            connect(PsPw, &QPointSalePickWidget::signalPointSalePicked, this, [=](const QVector<int> &pointSales) {

                currentCategory->idPointSale = pointSales;



                delete PsPw;
                PsPw = nullptr;
                 currentPlugin->crudCategoryInfo(*currentCategory,cDrinksId,0x01);
                 emit btnCancel->clicked();
            });
            PsPw->show();

        } else {
            currentPlugin->crudCategoryInfo(*currentCategory,cDrinksId,0x02);
             emit btnCancel->clicked();
        }
    }


    }
}


void QCategoryWidget::slotDeleteCategory() {


        QVector<qint32> cDrinksId;

        for (int i=0;i<cDrinks.size();i++) {
            cDrinksId.push_back(cDrinks.at(i).id);
        }
        currentPlugin->crudCategoryInfo(*currentCategory,cDrinksId,0x03);
        emit btnCancel->clicked();



}
