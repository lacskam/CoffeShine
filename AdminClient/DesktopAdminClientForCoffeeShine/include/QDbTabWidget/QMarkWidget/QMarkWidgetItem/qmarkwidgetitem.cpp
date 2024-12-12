#include "qmarkwidgetitem.h"

QMarkWidgetItem::QMarkWidgetItem(QWidget *parent) : QWidget(parent)
{

}

QMarkWidgetItem::QMarkWidgetItem(WidgetToMarkItemInfo info, QWidget *parent) : QWidget(parent)
{
    infoAboutWidget = info;
    createForm();
}

WidgetToMarkItemInfo QMarkWidgetItem::getInfoAboutWidget()
{
    return infoAboutWidget;
}

void QMarkWidgetItem::createForm()
{
    mainLayout = new QHBoxLayout(this);
    itemStatus = new QCheckBox();

    connect(itemStatus,SIGNAL(clicked()),this,SLOT(slotCheckBoxChanged()));
    mainLayout->addWidget(itemStatus);
    itemName = new QLabel();
    mainLayout->addWidget(itemName,1);

    itemStatus->setChecked(infoAboutWidget.isItemMarked);
    itemName->setText(infoAboutWidget.itemName);
}

void QMarkWidgetItem::checkBoxChangedState(bool state)
{
    if (state != itemStatus->isChecked())
    {
        itemStatus->setChecked(state);
        infoAboutWidget.isItemMarked = itemStatus->isChecked();
        infoAboutWidget.itemWasChanged = !infoAboutWidget.itemWasChanged;
    }
    emit signalCheckBoxWasPressed();
}

void QMarkWidgetItem::mouseReleaseEvent(QMouseEvent *)
{
    checkBoxChangedState(!itemStatus->isChecked());
}

void QMarkWidgetItem::slotChangeStateForChechBoxOnTrue()
{
    checkBoxChangedState(true);
}

void QMarkWidgetItem::slotChangeStateForChechBoxOnFalse()
{
    checkBoxChangedState(false);
}

void QMarkWidgetItem::slotCheckBoxChanged()
{
    infoAboutWidget.isItemMarked = itemStatus->isChecked();
    infoAboutWidget.itemWasChanged = !infoAboutWidget.itemWasChanged;
    emit signalCheckBoxWasPressed();
}


