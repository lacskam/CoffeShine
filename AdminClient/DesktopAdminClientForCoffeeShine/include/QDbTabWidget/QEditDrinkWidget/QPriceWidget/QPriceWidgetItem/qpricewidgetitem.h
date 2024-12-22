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
#include "../../../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"


class QPriceWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QPriceWidgetItem(QWidget *parent = nullptr);
    explicit QPriceWidgetItem(QCoffeeClientPlugin *plugin_,PriceAndVolumeInfo info, QWidget *parent = nullptr);

    PriceAndVolumeInfo getInfoAboutPriceAndVolume();
    void setNewPriceAndVolumeInfo(PriceAndVolumeInfo);
    void fillingVolumeComboBox(QVector<VolumeForComboBoxInfo>*);
    void setTitle(QString);

    PriceAndVolumeInfo infoAboutWidget;
protected:
    virtual void paintEvent(QPaintEvent* e);

private:

    QCoffeeClientPlugin *currentPlugin;
    bool validName = true;
    bool validUnits = true;
    bool validPrice = true;
    bool validVolume = true;

    QVBoxLayout *mainLayout;
    QHBoxLayout *layForPrice;
    QVBoxLayout *layForGroupBox;
    QVBoxLayout *layForVolume;
    QHBoxLayout *layForVolumeTitle;
    QHBoxLayout *layForNewVolume;

    QHBoxLayout *layForVolumeName;
    QHBoxLayout *layForNewUnits;

    QGroupBox* mainGroupBox;

    QLabel *priceTitle;
    QLabel *volumeTitle;
    QLabel *newVolumeValueTitle;
    QLabel *nameVolumeLabel;
    QLabel *unitsLabel;

    QLineEdit *priceLine;
    QLineEdit *volemeLine;
    QLineEdit *newNameVolume;
    QLineEdit *newVolumeUnits;

    QComboBox * volumeListComboBox;
    QCheckBox * newVolume;
    QWidget * widgetForNewVolume;


    QComboBox *volumeComboBox;


    void createForm();
    void fillingForm();
    void fillComboBoxVolumes();


signals:
    void signalDeleteThisItem(QPriceWidgetItem*);
    void signalFillingVolumeComboBox();

public slots:

private slots:
      void setNewInfoAboutWg();
    void slotChangeComboBox(int);
    void slotShowMenuContext(QPoint pos);
    void slotCheckBoxChanged();
    void slotDeleteThisItem();
};

#endif // QPRICEWIDGETITEM_H
