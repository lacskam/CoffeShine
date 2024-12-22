#include "qpricewidgetitem.h"

QPriceWidgetItem::QPriceWidgetItem(QWidget *parent) : QWidget(parent)
{

}

QPriceWidgetItem::QPriceWidgetItem(QCoffeeClientPlugin *plugin_, PriceAndVolumeInfo info, QWidget *parent) : QWidget(parent)
{
    currentPlugin = plugin_;

      infoAboutWidget = info;
    createForm();



    fillingForm();
    fillComboBoxVolumes();


    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(slotShowMenuContext(QPoint)));

}

void QPriceWidgetItem::setNewInfoAboutWg() {

}

PriceAndVolumeInfo QPriceWidgetItem::getInfoAboutPriceAndVolume()
{
    return infoAboutWidget;
}

void QPriceWidgetItem::setNewPriceAndVolumeInfo(PriceAndVolumeInfo info)
{
    infoAboutWidget = info;
    fillingForm();
}

void QPriceWidgetItem::createForm()
{
    mainLayout = new QVBoxLayout(this);
    mainGroupBox = new QGroupBox(this);
    mainLayout->addWidget(mainGroupBox);

    layForGroupBox = new QVBoxLayout();
    mainGroupBox->setLayout(layForGroupBox);

    layForPrice = new QHBoxLayout();
    layForGroupBox->addLayout(layForPrice,0);

    priceTitle = new QLabel();
    priceLine = new QLineEdit();
    priceTitle->setText("Цена:");





    priceLine->setMaximumWidth(50);



    layForVolumeTitle = new QHBoxLayout();
    layForGroupBox->addLayout(layForVolumeTitle,0);
    volumeTitle = new QLabel();
    volumeTitle->setText("Объём:");




    volumeComboBox =  new QComboBox;
    volemeLine = new QLineEdit();



    layForVolumeName = new QHBoxLayout;
    layForNewUnits = new QHBoxLayout;


    newNameVolume = new QLineEdit;
    newVolumeUnits = new QLineEdit;


    nameVolumeLabel = new QLabel("Название:");
    unitsLabel = new QLabel("Единицы:");


    layForVolumeName->addWidget(nameVolumeLabel);

    layForVolumeName->addWidget(newNameVolume);

    layForNewUnits->addWidget(unitsLabel);
    layForNewUnits->addSpacing(5);

    layForNewUnits->addWidget(newVolumeUnits);

    volemeLine->setMaximumWidth(50);

    if (infoAboutWidget.volumeId<0) {

        layForPrice->addWidget(priceTitle);
          layForPrice->addSpacing(27);
         layForPrice->addWidget(priceLine,1,Qt::AlignLeft);

          layForVolumeTitle->addWidget(volumeTitle);
         layForVolumeTitle->addSpacing(17);
        layForVolumeTitle->addWidget(volemeLine,1,Qt::AlignLeft);

        layForGroupBox->addLayout(layForVolumeName);
        layForGroupBox->addLayout(layForNewUnits);


    } else {
        layForPrice->addWidget(priceTitle);
        layForPrice->addSpacing(10);
        layForPrice->addWidget(priceLine,1,Qt::AlignLeft);

        layForVolumeTitle->addWidget(volumeTitle);
          layForVolumeTitle->addWidget(volumeComboBox,1,Qt::AlignLeft);
    }










    layForVolume = new QVBoxLayout();
    layForGroupBox->addLayout(layForVolume);
    // volumeListComboBox = new QComboBox();
    // volumeListComboBox->setMinimumHeight(20);
    // layForVolume->addWidget(volumeListComboBox,1,Qt::AlignVCenter | Qt::AlignLeft);
    // connect(volumeListComboBox,SIGNAL(activated(int)),this,SLOT(slotChangeComboBox(int)));
    // volumeListComboBox->setMinimumWidth(120);

    // widgetForNewVolume = new QWidget();
    // layForNewVolume = new QHBoxLayout();
    // layForVolume->addWidget(widgetForNewVolume,1,Qt::AlignVCenter);
    // widgetForNewVolume->setLayout(layForNewVolume);

    // newVolumeValue = new QLineEdit();
    // layForNewVolume->addWidget(newVolumeValue,Qt::AlignLeft);
    // newVolumeValue->setMaximumWidth(50);
    // newVolumeValue->setToolTip("Значение");

    // newVolumeUnits = new QLineEdit();
    // layForNewVolume->addWidget(newVolumeUnits,1,Qt::AlignLeft);
    // newVolumeUnits->setMaximumWidth(50);
    // newVolumeUnits->setToolTip("Единицы измерения");

}

void QPriceWidgetItem::fillComboBoxVolumes() {
    QVector<QCoffeeVolumeDrinkInfo> allvolumes = currentPlugin->getListVolume();
    for (int i=0;i<allvolumes.count();i++) {
        volumeComboBox->addItem(allvolumes.at(i).name+" "+QString::number(allvolumes.at(i).volume)+allvolumes.at(i).units);
    }
    volumeComboBox->setCurrentIndex(infoAboutWidget.volumeId-1);

}

void QPriceWidgetItem::fillingForm()
{

    if (infoAboutWidget.priceId>0) {
           priceLine->setText(QString::number(infoAboutWidget.price));
    }

    if (infoAboutWidget.volumeId>0)
    {

        priceLine->setText(QString::number(infoAboutWidget.price));
        volemeLine->setText(QString::number(infoAboutWidget.volume));

    }
    else
    {
      //  newVolumeValue->setText(QString::number(infoAboutWidget.volume));
       // newVolumeUnits->setText(infoAboutWidget.units);
    }
    connect(priceLine,&QLineEdit::textChanged,this,[=]() {
        priceLine->setStyleSheet("color: rgb(255, 255, 255)");




        QRegExp validRegForVolumeAndPricce("^[0-9]*$");




        if (!validRegForVolumeAndPricce.exactMatch(priceLine->text())) {
            validPrice=0;
            priceLine->setStyleSheet("color: rgb(255, 0, 0)");

        } else {
            validPrice=1;
            infoAboutWidget.price = priceLine->text().toDouble();

            qDebug()<<"edited price";

        }
        infoAboutWidget.valid = validUnits && validName && validVolume && validPrice;
    });

    connect(volemeLine,&QLineEdit::textChanged,this,[=]() {
        volemeLine->setStyleSheet("color: rgb(255, 255, 255)");

        QRegExp validRegForVolumeAndPricce("^[0-9]*$");



        if (!validRegForVolumeAndPricce.exactMatch(volemeLine->text())) {
            validVolume=0;
            volemeLine->setStyleSheet("color: rgb(255, 0, 0)");

        } else {
            validVolume=1;
            infoAboutWidget.volume = volemeLine->text().toInt();
            qDebug()<<"edited volume";
        }
        infoAboutWidget.valid = validUnits && validName && validVolume && validPrice;
    });

    connect(newNameVolume,&QLineEdit::textChanged,this,[=]() {
        newNameVolume->setStyleSheet("color: rgb(255, 255, 255)");
        QRegExp validRegForName("^[a-zA-Zа-яА-Я0-9 \"-]*$");


        if (!validRegForName.exactMatch(newNameVolume->text())) {
            validName=0;
            newNameVolume->setStyleSheet("color: rgb(255, 0, 0)");

        } else {
            validName=1;
            infoAboutWidget.nameVolume = newNameVolume->text();

            qDebug()<<"edited nameVolume";
        }
        infoAboutWidget.valid = validUnits && validName && validVolume && validPrice;
    });

    connect(newVolumeUnits,&QLineEdit::textChanged,this,[=]() {
        newVolumeUnits->setStyleSheet("color: rgb(255, 255, 255)");

        QRegExp validRegForUnits("^[a-zA-Zа-яА-Я]*$");


        if (!validRegForUnits.exactMatch(newVolumeUnits->text())) {
            validUnits=0;
            newVolumeUnits->setStyleSheet("color: rgb(255, 0, 0)");

        } else {
            validUnits=1;
            infoAboutWidget.units = newVolumeUnits->text();
            qDebug()<<"edited units";
        }
        infoAboutWidget.valid = validUnits && validName && validVolume && validPrice;
    });

}

void QPriceWidgetItem::fillingVolumeComboBox(QVector<VolumeForComboBoxInfo>* volumeList)
{
    for (int i = 0; i < volumeList->count(); ++i)
    {
        volumeListComboBox->addItem(volumeList->at(i).nameVolume, QVariant::fromValue(volumeList->at(i)));
    }
}

void QPriceWidgetItem::setTitle(QString title)
{
    mainGroupBox->setTitle(title);
}

void QPriceWidgetItem::slotChangeComboBox(int)
{
    fillingForm();
}


void QPriceWidgetItem::slotShowMenuContext(QPoint pos)
{
    QMenu contextMenu(tr("Menu hosts"),this);

    QAction * actionAddHost = new QAction(tr("Удалить"),this);
    connect(actionAddHost,SIGNAL(triggered(bool)),this,SLOT(slotDeleteThisItem()));
    contextMenu.addAction(actionAddHost);

    contextMenu.exec(mapToGlobal(pos));
}

void QPriceWidgetItem::slotCheckBoxChanged()
{
    volumeListComboBox->setHidden(newVolume->isChecked());
    widgetForNewVolume->setHidden(!newVolume->isChecked());
}

void QPriceWidgetItem::slotDeleteThisItem()
{

    infoAboutWidget.forDelite=true;

    emit signalDeleteThisItem(this);
}

void QPriceWidgetItem::paintEvent(QPaintEvent *e)
{
   // mainGroupBox->resize(parentWidget()->width()-20,mainGroupBox->height());
}
