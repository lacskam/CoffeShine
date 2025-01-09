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
#include "../../QDesktopClientLoader/QPointSalePickWidget/qpointsalepickwidget.h"

class QCategoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QCategoryWidget(QCoffeeClientPlugin *plugin_, QCoffeeCategoryInfo *currentCat, QWidget *parent = nullptr, int pointSaleId=0);
    ~QCategoryWidget();


private:
    QCoffeeClientPlugin *currentPlugin;
    QPointSalePickWidget *PsPw;
    int currentPointSaleId;
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
    QPushButton *buttonDelete;

     QLabel *labelDelete;
    QPushButton *buttonDelete1;
     QLabel *labelUnlink;
    QPushButton *buttonUnlink;



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

     QGroupBox * gruopBoxPick;

         QGroupBox * gruopBoxPicked;


        QVBoxLayout  *hblgruopBoxPick;
        QVBoxLayout  *hblgruopBoxPicked;

     QDialog *drinkPickDialog;
     QScrollArea *drinkPickScrolarea;
     QListWidget *drinPickkListWidget;
       QDrinkWidgetItem *drinkPickWidgetItemDio;

     QDialog *drinkPickDialogPicked;
     QScrollArea *drinkPickScrolareaPicked;
     QListWidget *drinPickkListWidgetPicked;
     QDrinkWidgetItem *drinkPickWidgetItemDioPicked;
     QLineEdit *searchLine;
     QLabel* labelForIcon;
        QPixmap *searchIcon;
     QHBoxLayout *hblforSearch;

        QLineEdit *searchLine2;
        QLabel* labelForIcon2;
        QPixmap *searchIcon2;
        QHBoxLayout *hblforSearch2;





       QDialog *delDio;
     QDialogButtonBox *btnBox;
     QVBoxLayout *vd;
     QHBoxLayout *hd;
     QPushButton *ac;
     QPushButton *de;
     qint32 id;





      bool checkChanges();
signals:

private slots:

    void slotSearchInAllDrinks(QString searchText);



    void slotSearchInCategoryDrinks(QString searchText);
    void replaceDrinkItems();
    void updateDrinkItems();
    void sendCategory();
    void createDrinkPickDialog();
    void updateDrinkPickDialog();
    void drinkPickDialogAccepted();
    void slotDeleteCategory();



protected:
    virtual void paintEvent(QPaintEvent *e);
};
#endif // QCATEGORYWIDGET_H
