#ifndef QMARKWIDGETITEM_H
#define QMARKWIDGETITEM_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDebug>
#include <QMouseEvent>
#include "../../commondbtabtypes.h"

class QMarkWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit QMarkWidgetItem(QWidget *parent = nullptr);
    explicit QMarkWidgetItem(WidgetToMarkItemInfo info,QWidget *parent = nullptr);


    WidgetToMarkItemInfo getInfoAboutWidget();

protected:
    virtual void mouseReleaseEvent(QMouseEvent*);

private:
    WidgetToMarkItemInfo infoAboutWidget;

    QHBoxLayout *mainLayout;
    QLabel *itemName;
    QCheckBox *itemStatus;

    void createForm();
    void checkBoxChangedState(bool);
signals:
    void signalCheckBoxWasPressed();

public slots:
    void slotChangeStateForChechBoxOnTrue();
    void slotChangeStateForChechBoxOnFalse();

private slots:
    void slotCheckBoxChanged();
};

#endif // QMARKWIDGETITEM_H
