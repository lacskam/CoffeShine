#ifndef QPICTUREWIDGET_H
#define QPICTUREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QDebug>
#include "QPrictureDialog/qpicturedialog.h"

class QPictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPictureWidget(QCoffeePictureInfo, QWidget *parent = nullptr);

    void setPictureList(QVector<QCoffeePictureInfo>);
    QCoffeePictureInfo getPictureInfo();
    QCoffeePictureInfo getInfoAboutNewPicture();
protected:
    virtual void mouseReleaseEvent(QMouseEvent*);

private:
    QLabel *labelForIcon;
    QPixmap currentPicture;

    QVector<QCoffeePictureInfo> pictureList;
    QCoffeePictureInfo newPicture;
    QCoffeePictureInfo pictureInfo;
signals:

public slots:
};

#endif // QPICTUREWIDGET_H
