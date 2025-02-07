#include "drinkimageprovider.h"

DrinkImageProvider::DrinkImageProvider(ImageType type,QObject *parent, Flags flags) :
     QQuickImageProvider(type,flags)
{

}

QImage DrinkImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    emit signalGetImageFromDB(id.toInt());

    return newImage;
}

void DrinkImageProvider::slotGetImageFromDB(QImage &image)
{
    newImage = image;
}

DrinkImageProvider::~DrinkImageProvider()
{

}
