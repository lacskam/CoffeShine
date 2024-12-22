#include "qeditdrinkwidget.h"

QEditDrinkWidget::QEditDrinkWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget (parent)
{
    currentPlugin = plugin_;
    currentEditedDrink.id = -1;
    createForm();
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(slotCloseEditing()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSaveChanges()));
    connect(this,SIGNAL(signalCloseEditDrink()),parent,SLOT(slotCloseDrinkEditWidget()));}

QEditDrinkWidget::QEditDrinkWidget(QCoffeeDrinkInfo* info,QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent)
{
    currentEditedDrink = *info;
    currentPlugin = plugin_;
    createForm();
    fillingFormByDrinkInfo();
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(slotCloseEditing()));
    connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSaveChanges()));
    connect(this,SIGNAL(signalCloseEditDrink()),parent,SLOT(slotCloseDrinkEditWidget()));
}


void QEditDrinkWidget::paintEvent(QPaintEvent *e)
{
    this->resize(parentWidget()->width(),this->height());
}

void QEditDrinkWidget::createForm()
{
    mainLayout = new QVBoxLayout(this);

    QLabel * drinkNameTitle = new QLabel("Название:");
    mainLayout->addWidget(drinkNameTitle);
    drinkName = new QLineEdit();
    mainLayout->addWidget(drinkName);
    mainLayout->addSpacing(10);

    QLabel * drinkDescriptionTitle = new QLabel("Описание:");
    mainLayout->addWidget(drinkDescriptionTitle);
    drinkDescription = new QTextEdit();
    mainLayout->addWidget(drinkDescription);
    drinkDescription->setMaximumHeight(50);
    drinkDescription->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addSpacing(10);

    QLabel * drinkColorTitle = new QLabel("Цвет:");
    mainLayout->addWidget(drinkColorTitle);
    colorWidget = new QColorWidget(this);
    mainLayout->addWidget(colorWidget);
    mainLayout->addSpacing(10);

    // фото виджет
    QLabel * drinkFotoTitle = new QLabel("Фото:");
    mainLayout->addWidget(drinkFotoTitle);
    pictureWidget = new QPictureWidget(currentPlugin->getPicture(currentEditedDrink.idPicture),this);
    pictureWidget->setPictureList(currentPlugin->getListPicture());
    mainLayout->addWidget(pictureWidget);

    // создание виджета с категориями
    categoryItemsWidget = new QMarkWidget();
    categoryItemsWidget->setTitle("Добавление категорий");
    categoryItemsWidget->setMinAndMaxHeight(300,500);

    mainLayout->addWidget(categoryItemsWidget);
    connect(categoryItemsWidget,SIGNAL(signalNumberOfCategoriesUsedHasChanged()),this,SLOT(updateCategories()));

    // для цен *
    QLabel * priceLabel = new QLabel(tr("Цены для:"));
    mainLayout->addWidget(priceLabel);
    mainLayout->addSpacing(15);
    QHBoxLayout *hblForPickPriceData = new QHBoxLayout;
    drinkCategoryComboBox = new QComboBox();
    drinkPointSaleComboBox = new QComboBox();
    connect(drinkPointSaleComboBox,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=]() {


                categoryItemsWidget->setItems(createCategoryInfoListForWidget());

                  updateCategories();
    });

     hblForPickPriceData->addWidget(drinkCategoryComboBox);
      hblForPickPriceData->addWidget(drinkPointSaleComboBox);


    mainLayout->addLayout(hblForPickPriceData);
    QPushButton *openPriceDialog = new QPushButton(tr("Изменить"));
    connect(openPriceDialog,&QPushButton::clicked,this,[=]() {
        QVBoxLayout *vblPrice = new QVBoxLayout;

        QVector<VolumeForComboBoxInfo> volumesForCb = createListVolumeForPriceWidget();
        QVector<PriceAndVolumeInfo> priceAndVolume = getPriceAndVolumeInfoForCurrentDrink();
        currentPriceAndVolumeInfo = priceAndVolume;
        QVector<CategoryForComboBoxInfo> categoryForCb = getCategoryInfoForCurrentDrink();

        QDialog *dio1 = new QDialog(this);
        QPriceWidget *priceWg = new QPriceWidget(currentPlugin,categoryForCb, priceAndVolume,volumesForCb,dio1);
        connect(priceWg,&QPriceWidget::signalAcceptedPrices,this,[=](const QVector<PriceAndVolumeInfo> &priceAndVol,const QVector<PriceAndVolumeInfo> &priceAndVolForDelete) {
            tempReturnedPriceAndVolumeInfo = priceAndVol;
            PriceAndVolumeInfoForDelete = priceAndVolForDelete;
            delete priceWg;
            delete dio1;
            for (int i =0;i<tempReturnedPriceAndVolumeInfo.count();i++) {
                qDebug()<<i<<" "<<tempReturnedPriceAndVolumeInfo.at(i).volumeId<<" "<<tempReturnedPriceAndVolumeInfo.at(i).volume<<" "<<tempReturnedPriceAndVolumeInfo.at(i).priceId<<" "<<tempReturnedPriceAndVolumeInfo.at(i).price;
            }

            for (int i =0;i<PriceAndVolumeInfoForDelete.count();i++) {
                qDebug()<<i<<" for delete "<<PriceAndVolumeInfoForDelete.at(i).volumeId<<" "<<PriceAndVolumeInfoForDelete.at(i).volume<<" "<<PriceAndVolumeInfoForDelete.at(i).priceId<<" "<<PriceAndVolumeInfoForDelete.at(i).price;
            }

            qDebug()<<checkChangesForPrices();

        });
        vblPrice->addWidget(priceWg);
        dio1->setLayout(vblPrice);
        dio1->setModal(true);
        dio1->show();
    });
    mainLayout->addWidget(openPriceDialog);

    updatePointSales();

    updateCategories();

    categoryItemsWidget->setItems(createCategoryInfoListForWidget());

    mainLayout->addSpacing(20);


    //удаление
    QLabel * deleteLabel = new QLabel(tr("Удаление"));
    mainLayout->addWidget(deleteLabel);
    mainLayout->addSpacing(5);
    QPushButton *openDeleteDialog = new QPushButton(tr("Удалить напиток"));
    mainLayout->addWidget(openDeleteDialog);
    mainLayout->addSpacing(20);
    deleteDialog = new QDeleteDialog();



    connect(openDeleteDialog,&QPushButton::clicked,this,[=]() {
        deleteDialog->setIdDrink(currentEditedDrink.id);
        deleteDialog->setModal(true);
        deleteDialog->show();
    });
    connect(deleteDialog,&QDeleteDialog::accepted,this,[=]() {
        deleteDialog->hide();
        slotDeleteDrink(deleteDialog->getIdDrink());

    });
    connect(deleteDialog,&QDeleteDialog::rejected,this,[=]() {
        deleteDialog->hide();

    });



//*
    QHBoxLayout * layForEndButton = new QHBoxLayout();
    mainLayout->addLayout(layForEndButton);
    saveButton = new QPushButton("Сохранить");
    cancelButton = new QPushButton("Закрыть");
    saveButton->setMinimumWidth(65);
    cancelButton->setMinimumWidth(65);
    layForEndButton->addWidget(saveButton);
    layForEndButton->addSpacing(20);
    layForEndButton->addWidget(cancelButton);
}

void QEditDrinkWidget::fillingFormByDrinkInfo()
{
    drinkName->setText(currentEditedDrink.name);
    drinkDescription->setText(currentEditedDrink.description);
    QColor color(currentEditedDrink.color.red(),currentEditedDrink.color.green(),currentEditedDrink.color.blue());
    colorWidget->setColor(color);
}

QVector<VolumeForComboBoxInfo> QEditDrinkWidget::createListVolumeForPriceWidget()
{
    QVector<VolumeForComboBoxInfo> Output;
    QVector<QCoffeeVolumeDrinkInfo> list = currentPlugin->getListVolume();

    for(int i=0;i<list.count();i++)
    {
        VolumeForComboBoxInfo info;
        info.idVolume = list.at(i).id;
        info.nameVolume = QString::number(list.at(i).volume) +" "+ list.at(i).units;
        Output.push_back(info);
    }

    return Output;
}

QVector<WidgetToMarkItemInfo> QEditDrinkWidget::createCategoryInfoListForWidget()     //сюда из базы взять
{
    QVector<WidgetToMarkItemInfo> Output;
    QVector<int> listIdC = currentPlugin->getCategoryIdForPointSale(currentPoinstSales.at(drinkPointSaleComboBox->currentIndex()).id);
    QVector<QCoffeeCategoryInfo> list;
    for (int i=0;i<listIdC.count();i++) {
        list.push_back(currentPlugin->getCategoryInfo(listIdC.at(i)));
    }

    QVector<int> listIdCategoriesForDrink;

    if (currentEditedDrink.id > 0)
    {
       listIdCategoriesForDrink = currentPlugin->getIdCategoriesForDrink(currentEditedDrink.id);
    }
    for (int i=0;i<list.count();i++)
    {
        WidgetToMarkItemInfo info;
        for (int j=0;j<listIdCategoriesForDrink.count();j++)
        {
            if (listIdCategoriesForDrink.at(j)==list.at(i).id)
            {
                info.isItemMarked = true;
                break;
            }
            else
            {
               info.isItemMarked = false;
            }
        }

        info.itemName = list.at(i).name;
        info.itemWasChanged = false;

        Output.push_back(info);
    }

    return Output;
}

QVector<PriceAndVolumeInfo> QEditDrinkWidget::getPriceAndVolumeInfoForCurrentDrink() // сюда из базы
{
    QVector<PriceAndVolumeInfo> Output;

    QVector<QCoffeeVolumeDrinkInfo> listVolume = currentPlugin->getListVolume(currentEditedDrink.id);
       QVector<CategoryForComboBoxInfo> currentCategory = this->getCategoryInfoForCurrentDrink();

       QCoffeePriceInfo tempPrice;
    for(int i=0;i<listVolume.count();i++)
    {

        tempPrice = currentPlugin->getPricceInfo(currentCategory.at(drinkCategoryComboBox->currentIndex()).idCategory, currentEditedDrink.id, listVolume.at(i).id);

        if (currentPlugin->getIdPointSaleForPriceInfo(tempPrice.id).contains(currentPoinstSales.at(drinkPointSaleComboBox->currentIndex()).id)) {
            PriceAndVolumeInfo info;
            info.itemWasChanged = false;
            info.volumeId = listVolume.at(i).id;
            info.volume = listVolume.at(i).volume;
            info.units = listVolume.at(i).units;


            info.priceId = tempPrice.id;
            info.price = tempPrice.value;



            if (info.price>0.1) {
                Output.push_back(info);
            }
        }


    }

    return Output;
}

QVector<CategoryForComboBoxInfo> QEditDrinkWidget::getCategoryInfoForCurrentDrink()
{
    QVector<CategoryForComboBoxInfo> Output;

    for(int i=0;i<categoryItemsWidget->getListInfoAboutWidgets().count();i++)
    {
        if (categoryItemsWidget->getListInfoAboutWidgets().at(i).isItemMarked)
        {
            CategoryForComboBoxInfo info;
            info.nameCaregory=categoryItemsWidget->getListInfoAboutWidgets().at(i).itemName;
            info.idCategory = categoryItemsWidget->getListInfoAboutWidgets().at(i).itemId;
            info.isItemMarked = categoryItemsWidget->getListInfoAboutWidgets().at(i).isItemMarked;

            Output.push_back(info);
        }
    }

    return Output;
}


void QEditDrinkWidget::updateCategories()
{
    QVector<CategoryForComboBoxInfo> currentCategory = this->getCategoryInfoForCurrentDrink();
    drinkCategoryComboBox->clear();
    for (int i = 0; i < currentCategory.count(); ++i)
    {
        if(currentCategory.at(i).isItemMarked)
        {
            drinkCategoryComboBox->addItem(currentCategory.at(i).nameCaregory, QVariant::fromValue(currentCategory.at(i)));
        }
    }
}


void QEditDrinkWidget::updatePointSales()
{

    currentPoinstSales = currentPlugin->getListPointSale();
    drinkPointSaleComboBox->clear();
    for (int i = 0; i < currentPoinstSales.count(); ++i)
    {


            drinkPointSaleComboBox->addItem(currentPoinstSales.at(i).name);

    }
}

void QEditDrinkWidget::slotCloseEditing()
{
    if (checkChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("The drink has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save |  QMessageBox::No  | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save :
            {
                slotSaveChanges();
                emit signalCloseEditDrink();
                break;
            }
            case QMessageBox::No :
            {
                emit signalCloseEditDrink();
                break;
            }
        }
    }
    else
    {
        emit signalCloseEditDrink();
    }
}

bool QEditDrinkWidget::checkChanges()
{
    qDebug()<<"Проверка изменений";

    if (drinkName->text() != currentEditedDrink.name)
        return true;

    if (drinkDescription->toPlainText() != currentEditedDrink.description)
        return true;

    if (colorWidget->colorWasChanged())
        return true;

    if (pictureWidget->getPictureInfo().id != pictureWidget->getInfoAboutNewPicture().id)
        return true;



    return false;
}

bool QEditDrinkWidget::checkChangesForPrices() {
     qDebug()<<"Проверка изменений для цен";
    if (tempReturnedPriceAndVolumeInfo.size()==currentPriceAndVolumeInfo.size()) {
         for (int i=0;i<tempReturnedPriceAndVolumeInfo.count();i++) {
            if (tempReturnedPriceAndVolumeInfo.at(i).volume !=currentPriceAndVolumeInfo.at(i).volume) {
                 return true;
            } else if (tempReturnedPriceAndVolumeInfo.at(i).price !=currentPriceAndVolumeInfo.at(i).price) {
                return true;
            }
         }
    } else {
        return true;
    }

    return false;
}

void QEditDrinkWidget::slotSaveChanges() {
{


    if (checkChanges()) {
    qDebug()<<"Coхранение изменений";
    QCoffeeDrinkInfo Output;


    QRegExp validReg("^[a-zA-Zа-яА-Я0-9 \"-]*$");
    int valid=0;

    if (!validReg.exactMatch(drinkDescription->toPlainText())) {

        drinkDescription->setStyleSheet("color: rgb(255, 0, 0)");
        connect(drinkDescription, &QTextEdit::textChanged, this, [=]() {drinkDescription->setStyleSheet("color: rgb(255, 255, 255)");});
        valid++;

    }

    if (!validReg.exactMatch(drinkName->text())) {

        drinkName->setStyleSheet("color: rgb(255, 0, 0)");
        connect(drinkName, &QLineEdit::textChanged, this, [=]() {drinkName->setStyleSheet("color: rgb(255, 255, 255)");});
        valid++;


    }
    if (valid>0) {
        return;
    } else {
        Output.id = currentEditedDrink.id;


        Output.name = drinkName->text();
        Output.description = drinkDescription->toPlainText();
        QColor newColor = colorWidget->getColor();
        Output.color.setRed(newColor.red());
        Output.color.setBlue(newColor.blue());
        Output.color.setGreen(newColor.green());
        Output.idPicture = pictureWidget->getInfoAboutNewPicture().id;






        Output.idCategories=currentPlugin->getIdCategoriesForDrink(currentEditedDrink.id);



        if ((int)Output.id > 0) {
            currentPlugin->crudDrinkInfo(Output,0x02);
        } else {
            if (Output.idPicture<0)
            {
                Output.idPicture = 0;
            }

            currentPlugin->crudDrinkInfo(Output,0x01);
             }
         }
    }
        if (checkChangesForPrices()) {
            for (int i=0;i<tempReturnedPriceAndVolumeInfo.count();i++) {
                //проверит что с темпой творится
                slotSendVolumeInfo(tempReturnedPriceAndVolumeInfo.at(i));
            }

            for (int i=0;i<PriceAndVolumeInfoForDelete.count();i++) {
                currentPlugin->unlinkVolumeAndDrink2(PriceAndVolumeInfoForDelete.at(i).volumeId,currentEditedDrink.id);
            }
        }

        emit updateListWidget();
        emit signalCloseEditDrink();
    }
}


void QEditDrinkWidget::slotSendVolumeInfo(const PriceAndVolumeInfo &newVolumeInfo) {


    QCoffeeVolumeDrinkInfo volumeDrinkInfo;
    currentInfo = newVolumeInfo;

    volumeDrinkInfo.id = currentInfo.volumeId;




    if (volumeDrinkInfo.id>-1) {

    } else {
        volumeDrinkInfo.idDrink.push_back(currentEditedDrink.id);
        volumeDrinkInfo.volume = currentInfo.volume;
        volumeDrinkInfo.name = currentInfo.nameVolume;
        volumeDrinkInfo.description = "";
        volumeDrinkInfo.units = currentInfo.units;

        currentPlugin->crudVolumeInfo(volumeDrinkInfo,0x01);

        slotPriceInfo(currentInfo);

    }

}


void QEditDrinkWidget::slotPriceInfo(const PriceAndVolumeInfo &newPriceInfo) {

    QCoffeePriceInfo priceInfo;
    QVector<CategoryForComboBoxInfo> currentCategory = this->getCategoryInfoForCurrentDrink();




    priceInfo.id = newPriceInfo.priceId;
    priceInfo.idDrink = currentEditedDrink.id;
    priceInfo.idDrinkCategory = currentCategory.at(drinkCategoryComboBox->currentIndex()).idCategory;
    priceInfo.idPointSale.push_back(currentPoinstSales.at(drinkPointSaleComboBox->currentIndex()).id);
    priceInfo.idVolume = currentPlugin->getListVolume().last().id+1; //исправить на реагирование по сигналу когда приходит новы обьем
    priceInfo.value = newPriceInfo.price;
    priceInfo.startDate = QDateTime(QDate::currentDate());


    if (priceInfo.id>-1) {
        //хз нужна ли вообще функция добавления
    } else {
        currentPlugin->crudPriceInfo(priceInfo,0x01);
    }




}
void QEditDrinkWidget::slotDeleteDrink(qint32 id) {


     qDebug()<<"Удаление напитка";
     QCoffeeDrinkInfo Output;
    Output.id = id;
     currentPlugin->crudDrinkInfo(Output,0x03);

    emit updateListWidget();
    emit signalCloseEditDrink();

}
