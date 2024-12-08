#ifndef QPRICEWIDGET_H
#define QPRICEWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include "QPriceWidgetItem/qpricewidgetitem.h"

class QPriceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPriceWidget(QWidget *parent = nullptr);

    void createNewItems(QVector<CategoryForComboBoxInfo>, QVector<PriceAndVolumeInfo>);
    void updateCategories(QVector<CategoryForComboBoxInfo>);
    void setListExistenVolumes(QVector<VolumeForComboBoxInfo>);
    QVector<PriceAndVolumeInfo> getListInfoAboutPriceAndVolume();

private:
    QBoxLayout *mainLayout;

    QListWidget * listWidgetWithPriceAndVolume;
    QListWidgetItem *listWidgetItem;
    QPriceWidgetItem * priceWidgetItem;
    QVector<VolumeForComboBoxInfo> * allVolume;
    void refreshTitleForAllItems();
signals:

public slots:
    void slotDeleteItem(QPriceWidgetItem*);


private slots:
    void slotAddPriceAndVolumeItem();
    void slotFillingComboBoxInWidgetItem();
};

#endif // QPRICEWIDGET_H
