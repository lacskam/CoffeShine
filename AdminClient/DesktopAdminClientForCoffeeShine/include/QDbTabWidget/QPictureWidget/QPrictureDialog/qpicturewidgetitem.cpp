#include "qpicturewidgetitem.h"

QPictureWidgetItem::QPictureWidgetItem(QCoffeePictureInfo info, QWidget *parent) : QWidget (parent)
{
    infoAdoutPicture = info;
    setCursor(Qt::ArrowCursor);
    createForm();
}

void QPictureWidgetItem::createForm()
{
    QVBoxLayout * layForPicture = new QVBoxLayout(this);
    labelForIcon = new QLabel(this);
    labelForIcon->setAlignment(Qt::AlignCenter);
    layForPicture->addWidget(labelForIcon);
    labelForIcon->setFixedSize(150,150);
    QPixmap  picture;
    picture.loadFromData(infoAdoutPicture.data);
    picture = picture.scaled(128,128,Qt::KeepAspectRatio);
    labelForIcon->setPixmap(picture);
}

QCoffeePictureInfo QPictureWidgetItem::getPictureInfo()
{
    return infoAdoutPicture;
}

bool QPictureWidgetItem::isItemSelected()
{
    return itemSelected;
}

void QPictureWidgetItem::mousePressEvent(QMouseEvent *)
{
    this->setStyleSheet("background-color: rgba(51, 255, 255, 200); ");  // aqua
}

void QPictureWidgetItem::mouseReleaseEvent(QMouseEvent *)
{
    emit signalUpdateBackgroundColor(infoAdoutPicture.id);
}

void QPictureWidgetItem::slotUpdateBackgroundColor(int id)
{
    if (id == infoAdoutPicture.id)
    {
        this->setStyleSheet("background-color: rgba(51, 255, 255, 100); ");  // aqua
        itemSelected = true;
    }
    else
    {
        this->setStyleSheet("background-color: transparent; ");
        itemSelected = false;
    }

}

