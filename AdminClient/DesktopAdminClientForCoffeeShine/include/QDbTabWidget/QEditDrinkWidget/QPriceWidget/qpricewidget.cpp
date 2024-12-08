#include "qpricewidget.h"

QPriceWidget::QPriceWidget(QWidget *parent) : QWidget(parent)
{
    this->setMinimumHeight(600);

    mainLayout = new QVBoxLayout(this);
    QGroupBox *mainGroupBox = new QGroupBox();
    mainLayout->addWidget(mainGroupBox);
    mainGroupBox->setTitle("Цены для категории");
    QVBoxLayout *layForGroupBox = new QVBoxLayout();
    mainGroupBox->setLayout(layForGroupBox);


 //   connect(drinkCategoryComboBox,SIGNAL(activated(int)),this,SLOT(slotChangeCategoryComboBox(int)));
 //   drinkCategoryComboBox->setMinimumHeight(25);

    listWidgetWithPriceAndVolume = new QListWidget();
    listWidgetWithPriceAndVolume->setSelectionMode( QAbstractItemView::NoSelection );
    layForGroupBox->addWidget(listWidgetWithPriceAndVolume,1);

    QPushButton *addNewVolumeButton = new QPushButton("Добавить новый объём", this);
    layForGroupBox->addWidget(addNewVolumeButton);
    connect(addNewVolumeButton,SIGNAL(clicked()),this,SLOT(slotAddPriceAndVolumeItem()));
}

void QPriceWidget::createNewItems(QVector<CategoryForComboBoxInfo> currentCategory, QVector<PriceAndVolumeInfo> currentItems)
{



    for (int i = 0; i < currentItems.count(); ++i)
    {
        listWidgetItem = new QListWidgetItem(listWidgetWithPriceAndVolume);
        listWidgetWithPriceAndVolume->addItem (listWidgetItem);
        priceWidgetItem = new QPriceWidgetItem(currentItems.at(i));
        connect(priceWidgetItem,SIGNAL(signalDeleteThisItem(QPriceWidgetItem*)),this,SLOT(slotDeleteItem(QPriceWidgetItem*)));
        priceWidgetItem->setTitle(QString::number(i+1));
        slotFillingComboBoxInWidgetItem();
        //Making sure that the listWidgetItem has the same size as the TheWidgetItem
        listWidgetItem->setSizeHint (priceWidgetItem->sizeHint());
        listWidgetWithPriceAndVolume->setItemWidget(listWidgetItem, priceWidgetItem);
    }
}

void QPriceWidget::slotDeleteItem(QPriceWidgetItem * deleteItem)
{
    for (int i=0;i<listWidgetWithPriceAndVolume->count();i++)
    {
        priceWidgetItem = dynamic_cast<QPriceWidgetItem*>(listWidgetWithPriceAndVolume->itemWidget(listWidgetWithPriceAndVolume->item(i)));
        if (deleteItem == priceWidgetItem)
        {
            listWidgetWithPriceAndVolume->removeItemWidget(listWidgetWithPriceAndVolume->item(i));
            delete listWidgetWithPriceAndVolume->item(i);
        }
    }

    refreshTitleForAllItems();
}

void QPriceWidget::setListExistenVolumes(QVector<VolumeForComboBoxInfo> list)
{
    allVolume = &list;
}

QVector<PriceAndVolumeInfo> QPriceWidget::getListInfoAboutPriceAndVolume()
{
    QVector<PriceAndVolumeInfo> Output;

    for(int i=0;i<listWidgetWithPriceAndVolume->count();i++)
    {
        PriceAndVolumeInfo info = dynamic_cast<QPriceWidgetItem*>(listWidgetWithPriceAndVolume->
                                                                  itemWidget(listWidgetWithPriceAndVolume->item(i)))->
                                                                  getInfoAboutPriceAndVolume();

        Output.push_back(info);
    }

    return Output;
}

void QPriceWidget::refreshTitleForAllItems()
{
    for(int i=0;i<listWidgetWithPriceAndVolume->count();i++)
    {
        priceWidgetItem = dynamic_cast<QPriceWidgetItem*>(listWidgetWithPriceAndVolume->
                                                          itemWidget(listWidgetWithPriceAndVolume->item(i)));
        priceWidgetItem->setTitle(QString::number(i+1));
    }
}


void QPriceWidget::slotAddPriceAndVolumeItem()
{
    listWidgetItem = new QListWidgetItem(listWidgetWithPriceAndVolume);
    listWidgetWithPriceAndVolume->addItem (listWidgetItem);

    PriceAndVolumeInfo sadf;
    sadf.volumeId = 2;
    priceWidgetItem = new QPriceWidgetItem(sadf);
    priceWidgetItem->setTitle(QString::number(listWidgetWithPriceAndVolume->count()));
    connect(priceWidgetItem,SIGNAL(signalDeleteThisItem(QPriceWidgetItem*)),this,SLOT(slotDeleteItem(QPriceWidgetItem*)));
    slotFillingComboBoxInWidgetItem();
    //Making sure that the listWidgetItem has the same size as the TheWidgetItem
    listWidgetItem->setSizeHint (priceWidgetItem->sizeHint ());
    listWidgetWithPriceAndVolume->setItemWidget(listWidgetItem, priceWidgetItem);

}

void QPriceWidget::slotFillingComboBoxInWidgetItem()
{
    priceWidgetItem->fillingVolumeComboBox(allVolume);
}
