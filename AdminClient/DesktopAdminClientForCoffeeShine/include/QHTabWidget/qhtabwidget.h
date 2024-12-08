#ifndef QHTABWIDGET_H
#define QHTABWIDGET_H
#include <QTabWidget>
#include <QPaintEvent>
#include <QDebug>
#include "qhtabbar.h"

class QHTabWidget : public QTabWidget
{
public:
    QHTabWidget(QWidget *parent=NULL);
    ~QHTabWidget();
    QHTabBar *TabBar=NULL;
    //void setFont(const QFont &font);
    void paintEvent(QPaintEvent *event);
    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

private:

};

#endif // QHTABWIDGET_H
