#include "qpricewidget.h"

QPriceWidget::QPriceWidget(QCoffeeClientPlugin *plugin_, const QVector<CategoryForComboBoxInfo> &categories, const QVector<PriceAndVolumeInfo> &prices, const QVector<VolumeForComboBoxInfo> &VolumesForCb, QWidget *parent) : QWidget(parent)
{
    this->setMinimumHeight(600);
    currentPlugin = plugin_;
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
    setListExistenVolumes(VolumesForCb);
    QPushButton *addNewVolumeButton = new QPushButton("Добавить новый объём", this);

    QHBoxLayout *layForBtns = new QHBoxLayout();

    QPushButton *saveChangesBtn = new QPushButton("Сохранить", this);



    QPushButton *declineChangesBtn = new QPushButton("Отмена", this);

    layForBtns->addWidget(saveChangesBtn);
    layForBtns->addWidget(declineChangesBtn);

    mainLayout->addLayout(layForBtns);

    layForGroupBox->addWidget(addNewVolumeButton);
    connect(addNewVolumeButton,SIGNAL(clicked()),this,SLOT(slotAddPriceAndVolumeItem()));

    createNewItems(categories,prices);
     connect(saveChangesBtn, &QPushButton::clicked, this, &QPriceWidget::btnSaveClicked);
     connect(declineChangesBtn, &QPushButton::clicked, this, &QPriceWidget::signalDeclinePrices);
}

void QPriceWidget::createNewItems(const QVector<CategoryForComboBoxInfo> &currentCategory, const QVector<PriceAndVolumeInfo> &currentItems)
{



    for (int i = 0; i < currentItems.count(); ++i)
    {
        listWidgetItem = new QListWidgetItem(listWidgetWithPriceAndVolume);
        listWidgetWithPriceAndVolume->addItem (listWidgetItem);
        priceWidgetItem = new QPriceWidgetItem(currentPlugin,currentItems.at(i));
        connect(priceWidgetItem,SIGNAL(signalDeleteThisItem(QPriceWidgetItem*)),this,SLOT(slotDeleteItem(QPriceWidgetItem*)));

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
            priceAndVolumeInfoForDelite.push_back(priceWidgetItem->infoAboutWidget);
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

    PriceAndVolumeInfo tempPriceAndvolumeInfo;
    tempPriceAndvolumeInfo.volumeId = -1;
    tempPriceAndvolumeInfo.priceId = -1;
    priceWidgetItem = new QPriceWidgetItem(currentPlugin,tempPriceAndvolumeInfo);
    //priceWidgetItem->setTitle(QString::number(listWidgetWithPriceAndVolume->count()));
    connect(priceWidgetItem,SIGNAL(signalDeleteThisItem(QPriceWidgetItem*)),this,SLOT(slotDeleteItem(QPriceWidgetItem*)));

    //Making sure that the listWidgetItem has the same size as the TheWidgetItem
    listWidgetItem->setSizeHint (priceWidgetItem->sizeHint ());
    listWidgetWithPriceAndVolume->setItemWidget(listWidgetItem, priceWidgetItem);

}

void QPriceWidget::slotFillingComboBoxInWidgetItem()
{
    priceWidgetItem->fillingVolumeComboBox(allVolume);
}


void QPriceWidget::btnSaveClicked() {

    QVector<PriceAndVolumeInfo> priceAndVolumeInfo;
    priceAndVolumeInfo = getListInfoAboutPriceAndVolume();

    int valid = true;

    for (int i=0;i<priceAndVolumeInfo.size();i++) {
        if (!priceAndVolumeInfo.at(i).valid) {
            valid=false;
        }
    }




    if (valid) {
          emit signalAcceptedPrices(priceAndVolumeInfo,priceAndVolumeInfoForDelite);

    } else (qDebug()<<"not valid");

}
