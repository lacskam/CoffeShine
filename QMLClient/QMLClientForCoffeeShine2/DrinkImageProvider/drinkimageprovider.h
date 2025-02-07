#ifndef DRINKIMAGEPROVIDER_H
#define DRINKIMAGEPROVIDER_H

#include "QQuickImageProvider"
#include <QDebug>
#include <QImage>

class DrinkImageProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit DrinkImageProvider(ImageType type, QObject *parent = nullptr, Flags flags = Flags());
    ~DrinkImageProvider() override;

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QImage newImage;

signals:
    void signalGetImageFromDB(int);

public slots:
    void slotGetImageFromDB(QImage &image);

};

#endif // DRINKIMAGEPROVIDER_H
