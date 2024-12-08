#include "qdbtabwidget.h"


QDbTabWidget::QDbTabWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget (parent)
{
    currentPlugin = plugin_;
    createForm();
    updateDrinkItems();
    updateCategoryItems();
    connect(currentPlugin,SIGNAL(signalNewDrink()),this,SLOT(updateDrinkItems()));
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
    categoryWidgetLayout = new QVBoxLayout(categoryWidget);
    QPushButton *addCategoryButton = new QPushButton(tr("Создать новую"));
     connect(addCategoryButton,SIGNAL(clicked()),this,SLOT(slotAddNewCategory())); //что это блять
    categoryWidgetLayout->addWidget(addCategoryButton);
    tabWidget->addTab(categoryWidget,"Категории");

    categoryListWidget = new QListWidget();
    categoryListWidget->setFlow(QListView::TopToBottom);    //Lays out horizontally instead of vertically
    categoryListWidget->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
  //  categoryListWidget->setGridSize(QSize(180, 230));       //This is an arbitrary value, but it forces the layout into a grid
    categoryListWidget->setSpacing(10);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    categoryListWidget->setViewMode(QListView::ListMode);   //And the most important part:
    categoryWidgetLayout->addWidget(categoryListWidget);


    tabWidget->addTab(new QWidget(),"Точки продаж");
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


void QDbTabWidget::updateCategoryItems() {
    categoryListWidget->clear();

    QVector<QCoffeeCategoryInfo> listCategories = currentPlugin->getListCategories();

    for (int i = 0; i < listCategories.count(); i++) {


        QListWidgetItem *listWidgetItem = new QListWidgetItem(categoryListWidget);
        listWidgetItem->setText(listCategories.at(i).name);


        QCategoryItem *categoryWidgetItem = new QCategoryItem(listCategories.at(i), currentPlugin, categoryListWidget);
          connect(categoryWidgetItem,SIGNAL(signalOpenEditCategoryWidget(QCoffeeCategoryInfo)),this,SLOT(slotOpenCategoryEditWidget(QCoffeeCategoryInfo)));

        categoryListWidget->setItemWidget(listWidgetItem, categoryWidgetItem);


        qDebug() << listCategories.at(i).name;
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
    catrgoryEdit = new QCategoryWidget(currentPlugin,&info,this);

      scrollAreaForEdit->setWidget(catrgoryEdit);


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
        if(drinkWidgetItem->getDrinkInfo().name.contains(searchText) ||
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
    catrgoryEdit = new QCategoryWidget(currentPlugin,&info,this);
    scrollAreaForEdit->setWidget(catrgoryEdit);
}

