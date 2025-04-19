#ifndef QPOINTSALEEDITWIDGET_H
#define QPOINTSALEEDITWIDGET_H
#include <QWidget>
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
#include <QLineEdit>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include <QMessageBox>

class QPointSaleEditWidget : public QWidget
{
    Q_OBJECT
public:

    explicit QPointSaleEditWidget(QCoffeePointSale &info, QCoffeeClientPlugin *plugin_, QWidget *parent = nullptr);
    explicit QPointSaleEditWidget(QCoffeeClientPlugin *,QWidget *parent = nullptr);



protected:
    virtual void paintEvent(QPaintEvent* e);


private:


    QCoffeeClientPlugin *currentPlugin;
    QCoffeePointSale currentPointSale;

    QVBoxLayout *mainLayout;
    QLabel *labelName;
    QLineEdit *editName;
    QLabel *labelDrinks;
    QListWidget *listDrinks;
    QLabel *labelCategoryes;
    QListWidget *listCategoryes;

    QHBoxLayout *hblForButtons;
    QPushButton * saveButton;
    QPushButton * cancelButton;
    QPushButton * deleteButton;


    void createForm();

private slots:
    void slotSendpointSale(QCoffeePointSale &newPointSale);
    void slotDeletePointSale();


signals:
    void signaleClose();
    void signalAddNew(QCoffeePointSale &newPointSale);
    void signalSave();
};









#endif //QPOINTSALEEDITWIDGET_H
