#include "qpicturewidget.h"

QPictureWidget::QPictureWidget(QCoffeePictureInfo info,QWidget *parent) : QWidget(parent)  // сюда кидать инфо о пикчуре
{
    this->setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(128,128);
    pictureInfo = info;
    newPicture = pictureInfo;

    labelForIcon = new QLabel(this);
    labelForIcon->setAlignment(Qt::AlignCenter);
    labelForIcon->setFixedSize(128,128);
    currentPicture.loadFromData(pictureInfo.data);
    currentPicture = currentPicture.scaled(this->width(),this->height(),Qt::KeepAspectRatio);
    labelForIcon->setPixmap(currentPicture);
}

void QPictureWidget::setPictureList(QVector<QCoffeePictureInfo> list)
{
    pictureList = list;
}

QCoffeePictureInfo QPictureWidget::getPictureInfo()
{
    return pictureInfo;
}

QCoffeePictureInfo QPictureWidget::getInfoAboutNewPicture()
{
    return newPicture;
}

void QPictureWidget::mouseReleaseEvent(QMouseEvent *)
{
    bool ok;
    QCoffeePictureInfo info = QPictureDialog::getPicture(&pictureList, &ok, this);

    if (ok) {
        // update info
        newPicture = info;
        currentPicture.loadFromData(info.data);
        currentPicture = currentPicture.scaled(this->width(),this->height(),Qt::KeepAspectRatio);
        labelForIcon->setPixmap(currentPicture);
    }
}
