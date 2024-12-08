#include "qhtabwidget.h"

QHTabWidget::QHTabWidget(QWidget *parent)
{
    this->setParent(parent);
    TabBar = new QHTabBar(this);
    this->setTabBar(TabBar);
}

QHTabWidget::~QHTabWidget()
{
    if (TabBar!=NULL) delete TabBar;
}

void QHTabWidget::paintEvent(QPaintEvent *event)
{
    QTabWidget::paintEvent(event);
}

int QHTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
    int output = QTabWidget::insertTab(index,widget,icon,label);
    return output;
}


