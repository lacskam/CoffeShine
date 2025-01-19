#ifndef QDBTABWIDGET_H
#define QDBTABWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QFont>
#include <QPushButton>
#include <QTabWidget>
#include "QEditDrinkWidget/qeditdrinkwidget.h"
#include "QDrinkWidgetItem/qdrinkwidgetitem.h"
#include "QCategoryWidget/QCategoryItem/qcategoryitem.h"
#include "QCategoryWidget/qcategorywidget.h"
#include "QEditDrinkWidget/QDeleteDialog//qdeletedialog.h"
#include <QStackedWidget>
#include "../QDesktopClientLoader/QPointSalePickWidget/qpointsalepickwidget.h"
#include "QPointSaleEditWidget/qpointsaleeditwidget.h"

class QDbTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDbTabWidget(QCoffeeClientPlugin* plugin_,QWidget *parent = nullptr);

private:
    QCoffeeClientPlugin *currentPlugin;



    QHBoxLayout *mainLayout;
    QLayout *editDrinksLayout;
    QVBoxLayout *drinksWidgetLayout;
    QHBoxLayout *layForSearchWidget;

    QVBoxLayout *categoryWidgetLayout;

    QWidget *drinksWidget;
    QLineEdit *searchLine;
    QLabel *labelForIcon;
    QPixmap *searchIcon;


    QWidget *categoryWidget;
    QWidget *pointSaleWidget;
    QVBoxLayout *vblforPointSale;
    QPushButton *btnForAddPointSale;


    QGroupBox *editOrAddItemGroupBox;
    QScrollArea *scrollAreaForEdit;

    QListWidget *drinkListWidget;
    QListWidget *categoryListWidget;
    QListWidget *pointSaleListWidget;
    QListWidget *userListWidget;
    QDrinkWidgetItem *drinkWidgetItem;
    QCategoryItem *categoryWidgetItem;

    QEditDrinkWidget * editDrinkWidget;
    QVBoxLayout * editLayout;

    QHBoxLayout *hblForButtons;
    QCategoryWidget *catrgoryEdit;
    QStackedWidget *stackedWidget;
    QComboBox *pickPointSaleForCategory;
    QPushButton *btnAddExistenCategory;

    QPointSalePickWidget *pointSalePickWidget;
    QPointSaleEditWidget *pointSaleEditWidget;

    QWidget *wgExCategory;
    QVBoxLayout *vblExCategory;
    QListWidget *categoryListWidgetToAddEx;
    QPushButton *btnExAccept;
    void createForm();
signals:

public slots:
    void updateDrinkItems();
    void updateCategoryItems();
    void updatePointsaleItems();

     void updateCategoryItemsForEx();

private slots:
    void slotOpenDrinkEditWidget(QCoffeeDrinkInfo);
    void slotCloseDrinkEditWidget();
    void slotSearchDrinksTextChanged(QString);
    void slotAddNewDrink();
    void slotOpenCategoryEditWidget(QCoffeeCategoryInfo);
    void slotCloseCategoryEditWidget();
    void slotOpenPointSaleEditWidget(QCoffeePointSale &);
    void slotAddNewPointSale();
    void slotClosePointSaleEditWidget();
    void slotAddNewCategory();
};

#endif // QDBTABWIDGET_H
