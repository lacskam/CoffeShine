#include "qpointsaleeditwidget.h"



void QPointSaleEditWidget::paintEvent(QPaintEvent *e)
{
    this->resize(parentWidget()->width(),this->height());
}

QPointSaleEditWidget::QPointSaleEditWidget(QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget (parent)
{
    currentPlugin = plugin_;
    currentPointSale.id=-1;
    createForm();
    // connect(cancelButton,SIGNAL(clicked()),this,SLOT(slotCloseEditing()));
    // connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSaveChanges()));
    // connect(this,SIGNAL(signalCloseEditDrink()),parent,SLOT(slotCloseDrinkEditWidget()));
}

QPointSaleEditWidget::QPointSaleEditWidget(QCoffeePointSale &info,QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget(parent)
{
    currentPointSale = info;
    currentPlugin = plugin_;
    createForm();

    // connect(cancelButton,SIGNAL(clicked()),this,SLOT(slotCloseEditing()));
    // connect(saveButton,SIGNAL(clicked()),this,SLOT(slotSaveChanges()));
    // connect(this,SIGNAL(signalCloseEditDrink()),parent,SLOT(slotCloseDrinkEditWidget()));
}



void QPointSaleEditWidget::createForm() {
    mainLayout = new QVBoxLayout(this);

    labelName = new QLabel("Название");
    editName = new QLineEdit;
    mainLayout->addWidget(labelName);
    mainLayout->addWidget(editName);
    editName->setText(currentPointSale.name);


    hblForButtons = new QHBoxLayout;
    cancelButton = new QPushButton("Отмена");
    connect(cancelButton,&QPushButton::clicked,this,[=](){
        emit signaleClose();
    });
    saveButton = new QPushButton("Сохранить");
    connect(saveButton,&QPushButton::clicked,this,[=](){


        if (currentPointSale.id==-1) {
            currentPointSale.name=editName->text();
            currentPointSale.idCategories = {};
            currentPointSale.idDrinks = {};
              slotSendpointSale(currentPointSale);
        }
         emit signaleClose();

    });
    hblForButtons->addWidget(saveButton);
    hblForButtons->addWidget(cancelButton);


    mainLayout->addLayout(hblForButtons);




}



void QPointSaleEditWidget::slotSendpointSale(QCoffeePointSale &newPointSale) {
    currentPlugin->crudPointSaleInfo(newPointSale,0x01);
}
