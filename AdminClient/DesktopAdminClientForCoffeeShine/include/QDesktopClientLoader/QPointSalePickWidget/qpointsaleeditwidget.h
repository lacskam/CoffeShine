#ifndef QPOINTSALEEDITWIDGET_H
#define QPOINTSALEEDITWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"

class QPointSaleEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QPointSaleEditWidget(QCoffeeClientPlugin *plugin_, QWidget *parent = nullptr);

private:
    QLabel *name;
    QLabel *description;

    QPushButton *btnDelete;
    QPushButton *btnSave;
    QPushButton *btnCancel;


signals:
};

#endif // QPOINTSALEEDITWIDGET_H
