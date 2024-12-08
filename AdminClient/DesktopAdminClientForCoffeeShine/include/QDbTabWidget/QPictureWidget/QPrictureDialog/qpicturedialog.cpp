#include "qpicturedialog.h"

QPictureDialog::QPictureDialog(QVector<QCoffeePictureInfo>* inputList,QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint );
   // setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumSize(800,600);
    currentPictureList = inputList;
    createForm();
    fillingForm();
}

QCoffeePictureInfo QPictureDialog::getPicture(QVector<QCoffeePictureInfo>* inputList, bool *ok, QWidget *parent)
{
    QCoffeePictureInfo Output;

    QPictureDialog *dlg = new QPictureDialog(inputList, parent);
    // set input data, not need yet

    int ret = dlg->exec();
    if (ret == QDialog::Accepted)
    {
        // create output date
        Output = dlg->getSelectedPicture();

        if (ok!=nullptr)
        {
            *ok = true;
        }
    }

    if (ret==QDialog::Rejected)
    {
        if (ok!=nullptr)
        {
            *ok = false;
        }
    }

    dlg->deleteLater();
    return Output;
}

QCoffeePictureInfo QPictureDialog::getSelectedPicture()
{
    return selectedPicture;
}

QVector<QCoffeePictureInfo> QPictureDialog::getSelectedPictures()
{
    return selectedPictures;
}

void QPictureDialog::createForm()
{
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    listWidget = new QListWidget(this);
    listWidget->setFlow(QListView::LeftToRight);    //Lays out horizontally instead of vertically
    listWidget->setResizeMode(QListView::Adjust);   //Dynamically adjust contents
    listWidget->setGridSize(QSize(160, 160));         //This is an arbitrary value, but it forces the layout into a grid
    listWidget->setSpacing(5);                     //As an alternative to using setGridSize(), set a fixed spacing in the layout:
    listWidget->setViewMode(QListView::IconMode);
    mainLayout->addWidget(listWidget,1);
    listWidget -> setMovement(QListView::Static);
    listWidget->setSelectionMode(QListWidget::SingleSelection);

    layoutButtons = new QHBoxLayout();
    mainLayout->addLayout(layoutButtons);
    layoutButtons->addStretch(1);
    buttonOk = new QPushButton(tr("Ok"),this);
    connect(buttonOk,SIGNAL(pressed()),this,SLOT(slotPushOk()));
    buttonOk->setMaximumWidth(80);
    layoutButtons->addWidget(buttonOk);
    layoutButtons->addSpacing(10);
    buttonCancel = new QPushButton(tr("Cancel"),this);
    connect(buttonCancel,SIGNAL(pressed()),this,SLOT(slotPushCancel()));
    buttonCancel->setMaximumWidth(80);
    layoutButtons->addWidget(buttonCancel);
}

void QPictureDialog::fillingForm()
{
    for (int i =0;i< currentPictureList->count();i++)
    {
        listWidgetItem = new QListWidgetItem(listWidget);
        listWidget->addItem (listWidgetItem);

        pictureWidgetItem = new QPictureWidgetItem(currentPictureList->at(i), listWidget);
        connect(pictureWidgetItem,SIGNAL(signalUpdateBackgroundColor(int)),this,SIGNAL(signalUpdateItemsBackgroudColor(int)) );
        connect(this,SIGNAL(signalUpdateItemsBackgroudColor(int)),pictureWidgetItem, SLOT(slotUpdateBackgroundColor(int)));
        listWidgetItem->setSizeHint (pictureWidgetItem->sizeHint ());
        listWidget->setItemWidget(listWidgetItem, pictureWidgetItem);
    }
}

void QPictureDialog::slotPushOk()
{
    bool result = false;

    for (int i=0;i< listWidget->count();i++)
    {
        pictureWidgetItem = dynamic_cast<QPictureWidgetItem*>(listWidget->itemWidget(listWidget->item(i)));
        if (pictureWidgetItem->isItemSelected())
        {
             selectedPicture = pictureWidgetItem->getPictureInfo();
             result = true;
             break;
        }
    }

    if (result)
    {
        emit accept();
    }
    else
    {
        QMessageBox::information(this,
                                 tr("Ошибка"),
                                 tr("Проверьте выделили ли вы картинку"));
    }
}

void QPictureDialog::slotPushCancel()
{
    emit reject();
}
