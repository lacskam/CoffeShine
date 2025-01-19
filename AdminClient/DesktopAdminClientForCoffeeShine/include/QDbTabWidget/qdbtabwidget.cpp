#include "qdbtabwidget.h"


QDbTabWidget::QDbTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget (parent)
{
    currentPlugin = plugin_;
    createForm();
    updateDrinkItems();
    updateCategoryItems();
    connect(currentPlugin,SIGNAL(signalNewDrink()),this,SLOT(updateDrinkItems()));
    connect(currentPlugin,SIGNAL(signalNewCategory()),this,SLOT(updateCategoryItems()));
    connect(currentPlugin,&QCoffeeClientPlugin::signalNewPoint,this,&QDbTabWidget::updatePointsaleItems);
}

void QDbTabWidget::createForm()
{
    mainLayout = new QHBoxLayout(this);
    QTabWidget * tabWidget = new QTabWidget();      // используется для вкладок с бд

    drinksWidget = new QWidget(this);
    tabWidget->addTab(drinksWidget,"Напитки");
    drinksWidgetLayout = new QVBoxLayout(drinksWidget);
    layForSearchWidget = new QHBoxLayout();
    drinksWidgetLayout->addLayout(layForSearchWidget);
    searchLine = new QLineEdit(this);
    labelForIcon = new QLabel(this);
    searchIcon = new QPixmap(":/icons/other/Search.png");
    *searchIcon = searchIcon->scaled(24,24,Qt::KeepAspectRatio);
    labelForIcon->setPixmap(*searchIcon);
    layForSearchWidget->addWidget(labelForIcon);
    layForSearchWidget->addWidget(searchLine,1);
    QPushButton * addNewDrinkButton = new QPushButton(tr("Создать новый"));
    layForSearchWidget->addWidget(addNewDrinkButton);
    connect(addNewDrinkButton,SIGNAL(clicked()),this,SLOT(slotAddNewDrink()));
    connect(searchLine,SIGNAL(textEdited(QString)),this,SLOT(slotSearchDrinksTextChanged(QString)));

    drinkListWidget = new QListWidget();
    drinkListWidget->setFlow(QListView::LeftToRight);    //Lays out horizontally instead of vertically
    drinkListWidget->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
    drinkListWidget->setGridSize(QSize(180, 230));       //This is an arbitrary value, but it forces the layout into a grid
    drinkListWidget->setSpacing(10);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    drinkListWidget->setViewMode(QListView::IconMode);   //And the most important part:
    drinksWidgetLayout->addWidget(drinkListWidget);



    categoryWidget = new QWidget();
    hblForButtons = new QHBoxLayout;
    categoryWidgetLayout = new QVBoxLayout(categoryWidget);
    QPushButton *addCategoryButton = new QPushButton(tr("Создать новую"));
    btnAddExistenCategory = new QPushButton("Добавить существующую категорию");

    connect(btnAddExistenCategory,&QPushButton::clicked,this,[=]() {
        QDialog *dioWgEx = new QDialog(this);
        wgExCategory = new QWidget();

        vblExCategory = new QVBoxLayout(wgExCategory);

        dioWgEx->setLayout(vblExCategory);
        categoryListWidgetToAddEx = new QListWidget();
        categoryListWidgetToAddEx->setFlow(QListView::TopToBottom);    //Lays out horizontally instead of vertically
        categoryListWidgetToAddEx->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
        categoryListWidgetToAddEx->setGridSize(QSize(200, 40));       //This is an arbitrary value, but it forces the layout into a grid

        categoryListWidgetToAddEx->setViewMode(QListView::ListMode);
        categoryListWidgetToAddEx->setSelectionMode(QAbstractItemView::NoSelection);
        categoryListWidgetToAddEx->setStyleSheet(
            "QListWidget::item:hover { "
            "background-color: rgba(0, 120, 240, 15); "
            "}"
            );

        vblExCategory->addWidget(categoryListWidgetToAddEx);

        btnExAccept = new QPushButton("Принять");
        vblExCategory->addWidget(btnExAccept);
        updateCategoryItemsForEx();
        dioWgEx->setModal(this);
        dioWgEx->show();
        connect(btnExAccept,&QPushButton::clicked,this,[=]() {
            QVector<QCoffeeCategoryInfo> pickedExCategories;


            for (int i = 0; i < categoryListWidgetToAddEx->count(); ++i) {
                QListWidgetItem *listItem = categoryListWidgetToAddEx->item(i);


                QCategoryItem *categoryItem = qobject_cast<QCategoryItem*>(categoryListWidgetToAddEx->itemWidget(listItem));


                if (categoryItem && categoryItem->isPicked) {
                    categoryItem->categoryInfo.idPointSale.push_back(pickPointSaleForCategory->currentIndex());
                    pickedExCategories.push_back(categoryItem->categoryInfo);
                }
            }
            for (int i=0;i<pickedExCategories.count();i++) {
                QCoffeeCategoryInfo categoryCopy = pickedExCategories.at(i);
                currentPlugin->crudCategoryInfo(categoryCopy,currentPlugin->getIdDrinkForCategory(pickedExCategories.at(i).id),0x02);

            }



            dioWgEx->close();

        });

    });

    addCategoryButton->setMaximumWidth(100);
    pickPointSaleForCategory = new  QComboBox;
    pickPointSaleForCategory->addItem("Все катеогрии");
    QVector<QCoffeePointSale> pointSales = currentPlugin->getListPointSale();

    for (int i =0;i<pointSales.count();i++) {
        pickPointSaleForCategory->addItem("Категории для точки продаж "+pointSales.at(i).name);

    }

    connect(pickPointSaleForCategory, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, [this](int index) {

             updateCategoryItems();

            });
     connect(addCategoryButton,SIGNAL(clicked()),this,SLOT(slotAddNewCategory())); //что это блять

     hblForButtons->addWidget(pickPointSaleForCategory);
       hblForButtons->addWidget(addCategoryButton);
    categoryWidgetLayout->addLayout(hblForButtons);
    tabWidget->addTab(categoryWidget,"Категории");

    categoryListWidget = new QListWidget();

    categoryListWidget->setFlow(QListView::TopToBottom);
    categoryListWidget->setResizeMode(QListView::Fixed);
    categoryListWidget->setGridSize(QSize(200, 50));
    categoryListWidget->setSpacing(1);
    categoryListWidget->setViewMode(QListView::ListMode);
      categoryListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    categoryListWidget->setStyleSheet(
        "QListWidget::item:hover { "
        "background-color: rgba(0, 120, 240, 15); "
        "}"
        );


    categoryWidgetLayout->addWidget(categoryListWidget);
    categoryWidgetLayout->addWidget(btnAddExistenCategory);
    btnAddExistenCategory->hide();


    pointSaleWidget = new QWidget;
    vblforPointSale = new QVBoxLayout(pointSaleWidget);
     pointSalePickWidget = new QPointSalePickWidget(currentPlugin,1,this);
    btnForAddPointSale = new QPushButton("Создать новую");
     connect(btnForAddPointSale,&QPushButton::clicked,this,&QDbTabWidget::slotAddNewPointSale);
     connect(pointSalePickWidget,&QPointSalePickWidget::signalOpenEditPickWidget,this,&QDbTabWidget::slotOpenPointSaleEditWidget);
     vblforPointSale->addWidget(btnForAddPointSale);
    vblforPointSale->addWidget(pointSalePickWidget);

    tabWidget->addTab(pointSaleWidget,"Точки продаж");
    tabWidget->addTab(new QWidget(),"Другое");

    mainLayout->addWidget(tabWidget,3);
    editOrAddItemGroupBox = new QGroupBox();
    mainLayout->addWidget(editOrAddItemGroupBox,2);

    editOrAddItemGroupBox->setMinimumWidth(380);
    editOrAddItemGroupBox->setMaximumWidth(600);

    scrollAreaForEdit = new QScrollArea(editOrAddItemGroupBox);
    editLayout = new QVBoxLayout(editOrAddItemGroupBox);
    scrollAreaForEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editLayout->addWidget(scrollAreaForEdit);
    scrollAreaForEdit->setBackgroundRole(QPalette::Light);
}

void QDbTabWidget::updateDrinkItems()
{
    drinkListWidget->clear();

    QVector<QCoffeeDrinkInfo> listDrink = currentPlugin->getListDrink();



    for (int i=0;i<listDrink.count();i++)
    {
        QListWidgetItem *listWidgetItem = new QListWidgetItem(drinkListWidget);
        drinkListWidget->addItem (listWidgetItem);
            //Creating an object of the designed widget which is to be added to the listwidget
        drinkWidgetItem = new QDrinkWidgetItem(listDrink.at(i),currentPlugin,1, drinkListWidget);
        connect(drinkWidgetItem,SIGNAL(signalOpenEditDrinkWidget(QCoffeeDrinkInfo)),this,SLOT(slotOpenDrinkEditWidget(QCoffeeDrinkInfo)));
        connect(drinkWidgetItem,SIGNAL(signalUpdateListWidget()),this,SLOT(updateDrinkItems()));
            //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (drinkWidgetItem->sizeHint ());
        drinkListWidget->setItemWidget(listWidgetItem, drinkWidgetItem);
    }
}


void QDbTabWidget::updatePointsaleItems()
{


    QVector<QCoffeePointSale> pointSales = currentPlugin->getListPointSale();

    pickPointSaleForCategory->clear();

    pickPointSaleForCategory->addItem("Все катеогрии");

    for (int i =0;i<pointSales.count();i++) {
        pickPointSaleForCategory->addItem("Категории для точки продаж "+pointSales.at(i).name);

    }
    pointSalePickWidget->fillFormTab();



}


void QDbTabWidget::updateCategoryItems() {
    categoryListWidget->clear();
    int index = pickPointSaleForCategory->currentIndex();
    QVector<QCoffeeCategoryInfo> listCategories;
    if (index==0) {
         listCategories = currentPlugin->getListCategories();
        btnAddExistenCategory->hide();
    } else {
        QVector<int> categiryIdis = currentPlugin->getPointSaleInfo(index).idCategories;

        for (int i =0;i<categiryIdis.count();i++) {
             listCategories.push_back(currentPlugin->getCategoryInfo(categiryIdis.at(i)));
        }
        btnAddExistenCategory->show();

    }


    for (int i = 0; i < listCategories.count(); i++) {

        QListWidgetItem *listWidgetItem = new QListWidgetItem(categoryListWidget);


        QCategoryItem *categoryWidgetItem = new QCategoryItem(listCategories.at(i), currentPlugin, categoryListWidget);
        connect(categoryWidgetItem, SIGNAL(signalOpenEditCategoryWidget(QCoffeeCategoryInfo)),
                this, SLOT(slotOpenCategoryEditWidget(QCoffeeCategoryInfo)));


        listWidgetItem->setSizeHint(categoryWidgetItem->sizeHint());


        categoryListWidget->setItemWidget(listWidgetItem, categoryWidgetItem);

        qDebug() << listCategories.at(i).name;
    }
}

void QDbTabWidget::updateCategoryItemsForEx() {

    categoryListWidgetToAddEx->clear();

    int index = pickPointSaleForCategory->currentIndex();


    QVector<QCoffeeCategoryInfo> listCategories;
    listCategories = currentPlugin->getListCategories();

    QVector<int> categiryIdis = currentPlugin->getPointSaleInfo(index).idCategories;



    for (int i = 0; i < listCategories.count(); i++) {

        if (!categiryIdis.contains(listCategories.at(i).id)) {
            QListWidgetItem *listWidgetItem = new QListWidgetItem(categoryListWidgetToAddEx);


            QCategoryItem *categoryWidgetItem = new QCategoryItem(listCategories.at(i), currentPlugin, categoryListWidgetToAddEx,1);
            connect(categoryWidgetItem,SIGNAL(signalOpenEditCategoryWidget(QCoffeeCategoryInfo)),this,SLOT(slotOpenCategoryEditWidget(QCoffeeCategoryInfo)));
            listWidgetItem->setSizeHint(categoryWidgetItem->sizeHint());

            categoryListWidgetToAddEx->setItemWidget(listWidgetItem, categoryWidgetItem);


            qDebug() << listCategories.at(i).name;
        }

    }
}



void QDbTabWidget::slotOpenDrinkEditWidget(QCoffeeDrinkInfo info)
{
    delete scrollAreaForEdit->widget();
    editOrAddItemGroupBox->setTitle(tr("Редактировние напитка"));
    editDrinkWidget = new QEditDrinkWidget(&info,currentPlugin, this);
    scrollAreaForEdit->setWidget(editDrinkWidget);
}


void QDbTabWidget::slotOpenCategoryEditWidget(QCoffeeCategoryInfo info)
{

    delete scrollAreaForEdit->widget();
    editOrAddItemGroupBox->setTitle(tr("Редактировние категории"));
    catrgoryEdit = new QCategoryWidget(currentPlugin,&info,this,pickPointSaleForCategory->currentIndex());

      scrollAreaForEdit->setWidget(catrgoryEdit);


}


void QDbTabWidget::slotOpenPointSaleEditWidget(QCoffeePointSale &info) {
    delete scrollAreaForEdit->widget();

    editOrAddItemGroupBox->setTitle(tr("Редактировние точки продаж"));
    pointSaleEditWidget = new QPointSaleEditWidget(info,currentPlugin,this);
    connect(pointSaleEditWidget,&QPointSaleEditWidget::signaleClose,this,&QDbTabWidget::slotClosePointSaleEditWidget);
   // connect(pointSaleEditWidget,&QPointSaleEditWidget::signalSave,this,&QDbTabWidget::slotClosePointSaleEditWidget);
     scrollAreaForEdit->setWidget(pointSaleEditWidget);
}
void QDbTabWidget::slotAddNewPointSale() {
    delete scrollAreaForEdit->widget();

    editOrAddItemGroupBox->setTitle(tr("Создание точки продаж"));
    pointSaleEditWidget = new QPointSaleEditWidget(currentPlugin,this);

    connect(pointSaleEditWidget,&QPointSaleEditWidget::signaleClose,this,&QDbTabWidget::slotClosePointSaleEditWidget);
    // connect(pointSaleEditWidget,&QPointSaleEditWidget::signalSave,this,&QDbTabWidget::slotClosePointSaleEditWidget);
    scrollAreaForEdit->setWidget(pointSaleEditWidget);
}

void QDbTabWidget::slotClosePointSaleEditWidget() {
    QWidget *widgetToDelete = scrollAreaForEdit->widget();
    scrollAreaForEdit->setWidget(nullptr);


    delete widgetToDelete;


    editOrAddItemGroupBox->setTitle("");
}


void QDbTabWidget::slotCloseDrinkEditWidget()
{
    delete scrollAreaForEdit->widget();
    editOrAddItemGroupBox->setTitle("");
}

void QDbTabWidget::slotCloseCategoryEditWidget()
{
    QWidget *widgetToDelete = scrollAreaForEdit->widget();
    scrollAreaForEdit->setWidget(nullptr);


    delete widgetToDelete;


    editOrAddItemGroupBox->setTitle("");
}

void QDbTabWidget::slotSearchDrinksTextChanged(QString searchText)
{
    for(int i=0;i<drinkListWidget->count();i++)
    {
        drinkWidgetItem = dynamic_cast<QDrinkWidgetItem*>(drinkListWidget->itemWidget(drinkListWidget->item(i)));
        if(drinkWidgetItem->getDrinkInfo().name.contains(searchText, Qt::CaseInsensitive) ||
           searchText==nullptr || searchText=="")
        {
            drinkListWidget->setItemHidden(drinkListWidget->item(i), false);
        }
        else
        {
            drinkListWidget->setItemHidden(drinkListWidget->item(i), true);
        }
    }
}

void QDbTabWidget::slotAddNewDrink()
{
    delete scrollAreaForEdit->widget();
    editOrAddItemGroupBox->setTitle(tr("Создание нового напитка"));
    editDrinkWidget = new QEditDrinkWidget(currentPlugin, this);
    scrollAreaForEdit->setWidget(editDrinkWidget);
}

void QDbTabWidget::slotAddNewCategory() {
    delete scrollAreaForEdit->widget();
    editOrAddItemGroupBox->setTitle(tr("Создание новой категории"));
    QCoffeeCategoryInfo info;
    info.id=-1;
    info.description = "";
    info.name = "";
    info.idPointSale.push_back(1);
    info.idPointSale.push_back(2);
    catrgoryEdit = new QCategoryWidget(currentPlugin,&info,this);
    scrollAreaForEdit->setWidget(catrgoryEdit);
}

