#ifndef QPICTUREWIDGETITEM_H
#define QPICTUREWIDGETITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include <QMouseEvent>

class QPictureWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QPictureWidgetItem(QCoffeePictureInfo info,QWidget *parent = nullptr);

    QCoffeePictureInfo getPictureInfo();
    bool isItemSelected();
protected:
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
signals:
    void signalUpdateBackgroundColor(int);

public slots:
    void slotUpdateBackgroundColor(int);

private:
    QLabel* labelForIcon;

    void createForm();

    bool itemSelected = false;
    QCoffeePictureInfo infoAdoutPicture;
};

#endif // QPICTUREWIDGETITEM_H
