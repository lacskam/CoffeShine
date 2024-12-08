#ifndef QEDITDRINKWIDGET_H
#define QEDITDRINKWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QPushButton>
#include <QTextEdit>
#include <QPaintEvent>
#include <QComboBox>
#include <QListWidget>
#include <QMessageBox>
#include "../QColorWidget/qcolorwidget.h"
#include "QPriceWidget/qpricewidget.h"
#include "../QMarkWidget/qmarkwidget.h"
#include "../QPictureWidget/qpicturewidget.h"
#include "../commondbtabtypes.h"
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "QDeleteDialog//qdeletedialog.h"

class QEditDrinkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QEditDrinkWidget(QCoffeeClientPlugin *,QWidget *parent = nullptr);
    explicit QEditDrinkWidget(QCoffeeDrinkInfo*,QCoffeeClientPlugin *,QWidget *parent = nullptr);
    QDeleteDialog *deleteDialog;

    QVector<PriceAndVolumeInfo> getPriceAndVolumeInfoForCurrentDrink();
    QVector<CategoryForComboBoxInfo> getCategoryInfoForCurrentDrink();

protected:
    virtual void paintEvent(QPaintEvent* e);

private:
    QBoxLayout *mainLayout;
    QLabel *label;
    QGroupBox *editDrinksGroupBox;
    QScrollArea *scrollArea;

    QLineEdit * drinkName;
    QTextEdit * drinkDescription;
    QColorWidget * colorWidget;
    QPictureWidget *pictureWidget;

    QPushButton * saveButton;
    QPushButton * cancelButton;

    QPriceWidget * priceWidget;
    QMarkWidget *  categoryItemsWidget;
    QComboBox * drinkCategoryComboBox;

    void createForm();
    void fillingFormByDrinkInfo();
    QVector<VolumeForComboBoxInfo> createListVolumeForPriceWidget();
    bool checkChanges();  // true если есть изменения
    QVector<WidgetToMarkItemInfo> createCategoryInfoListForWidget();

    QCoffeeDrinkInfo currentEditedDrink;
    QCoffeeClientPlugin * currentPlugin;
signals:
    void signalCloseEditDrink();
    void signalNumberOfCategoriesUsedHasChanged();
    void updateListWidget();

public slots:
     void slotDeleteDrink(qint32 id);
private slots:
    void updateCategories();
    void slotCloseEditing();
    void slotSaveChanges();

};

#endif // QEDITDRINKWIDGET_H
