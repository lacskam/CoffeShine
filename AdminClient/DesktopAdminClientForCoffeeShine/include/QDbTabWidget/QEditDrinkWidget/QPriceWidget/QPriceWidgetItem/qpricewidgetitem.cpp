#include "qpricewidgetitem.h"

QPriceWidgetItem::QPriceWidgetItem(QWidget *parent) : QWidget(parent)
{

}

QPriceWidgetItem::QPriceWidgetItem(PriceAndVolumeInfo info,QWidget *parent) : QWidget(parent)
{
    createForm();


    infoAboutWidget = info;
    fillingForm();


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
    priceTitle->setText("Цена:");
    layForPrice->addWidget(priceTitle);
    priceLine = new QLineEdit();
      layForPrice->addSpacing(10);
    layForPrice->addWidget(priceLine,1,Qt::AlignLeft);
    priceLine->setMaximumWidth(50);



    layForVolumeTitle = new QHBoxLayout();
    layForGroupBox->addLayout(layForVolumeTitle,0);
    volumeTitle = new QLabel();
    volumeTitle->setText("Объём:");
    layForVolumeTitle->addWidget(volumeTitle);
    volemeLine = new QLineEdit();

    volemeLine->setMaximumWidth(50);


      layForVolumeTitle->addWidget(volemeLine,1,Qt::AlignLeft);




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
        infoAboutWidget.price = priceLine->text().toDouble();

        qDebug()<<"edited price";
    });

    connect(volemeLine,&QLineEdit::textChanged,this,[=]() {

        infoAboutWidget.volume = volemeLine->text().toInt();
        qDebug()<<"edited volume";
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
    emit signalDeleteThisItem(this);
}

void QPriceWidgetItem::paintEvent(QPaintEvent *e)
{
   // mainGroupBox->resize(parentWidget()->width()-20,mainGroupBox->height());
}
