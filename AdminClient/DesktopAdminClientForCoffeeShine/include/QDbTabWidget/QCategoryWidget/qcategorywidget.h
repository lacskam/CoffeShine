#ifndef QCATEGORYWIDGET_H
#define QCATEGORYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QFont>
#include <QPushButton>
#include <QTabWidget>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "../QDrinkWidgetItem//qdrinkwidgetitem.h"
#include <QDialog>

class QCategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCategoryWidget(QCoffeeClientPlugin *plugin_, QCoffeeCategoryInfo *currentCat, QWidget *parent = nullptr);

private:
    QCoffeeClientPlugin *currentPlugin;

    QCoffeeCategoryInfo *currentCategory;
     QVBoxLayout *mainLayout;
    QHBoxLayout *hblCategory;
     QPushButton *btnSave;
    QPushButton *btnCancel;
     QDrinkWidgetItem *drinkWidgetItem;
     QScrollArea *drinkScrol;
    QListWidget *drinkListWidget;
     QLineEdit *nameCategoryEdit;
    QTextEdit *descriptionCategoryEdit;
     QPushButton *buttonAddNewDrinkToCategory;


     QString nameFirst;
     QString descriptionFirst;
     QVector<QCoffeeDrinkInfo> linkedDrinksFirst;

    int cId;
     QVector<QCoffeeDrinkInfo> cDrinks;
    QVector<QDrinkWidgetItem*> *vDrinkAddres;
     QLabel *lDrinkPick;
     QHBoxLayout *hblDPick;
     QVBoxLayout *vblDPick;
     QHBoxLayout *hblDPickListW ;
     QPushButton *bntA;
     QPushButton *btnD;
     QDialog *drinkPickDialog;
     QScrollArea *drinkPickScrolarea;
     QListWidget *drinPickkListWidget;
       QDrinkWidgetItem *drinkPickWidgetItemDio;


      bool checkChanges();
signals:

private slots:
    void updateDrinkItems();
    void sendCategory();
    void createDrinkPickDialog();
    void drinkPickDialogAccepted();




protected:
    virtual void paintEvent(QPaintEvent *e);
};
#endif // QCATEGORYWIDGET_H
