#ifndef QPRICEWIDGETITEM_H
#define QPRICEWIDGETITEM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPaintEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QGroupBox>
#include "../../../commondbtabtypes.h"

class QPriceWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QPriceWidgetItem(QWidget *parent = nullptr);
    explicit QPriceWidgetItem(PriceAndVolumeInfo info, QWidget *parent = nullptr);

    PriceAndVolumeInfo getInfoAboutPriceAndVolume();
    void setNewPriceAndVolumeInfo(PriceAndVolumeInfo);
    void fillingVolumeComboBox(QVector<VolumeForComboBoxInfo>*);
    void setTitle(QString);

protected:
    virtual void paintEvent(QPaintEvent* e);

private:
    QVBoxLayout *mainLayout;
    QHBoxLayout *layForPrice;
    QVBoxLayout *layForGroupBox;
    QVBoxLayout *layForVolume;
    QHBoxLayout *layForVolumeTitle;
    QHBoxLayout *layForNewVolume;

    QGroupBox* mainGroupBox;
    QLabel *priceTitle;
    QLabel *volumeTitle;
    QLabel *newVolumeValueTitle;
    QLineEdit *priceLine;
    QLineEdit *volemeLine;
    QLineEdit *newVolumeValue;
    QLineEdit *newVolumeUnits;

    QComboBox * volumeListComboBox;
    QCheckBox * newVolume;
    QWidget * widgetForNewVolume;

    PriceAndVolumeInfo infoAboutWidget;

    void createForm();
    void fillingForm();


signals:
    void signalDeleteThisItem(QPriceWidgetItem*);
    void signalFillingVolumeComboBox();

public slots:

private slots:
    void slotChangeComboBox(int);
    void slotShowMenuContext(QPoint pos);
    void slotCheckBoxChanged();
    void slotDeleteThisItem();
};

#endif // QPRICEWIDGETITEM_H
