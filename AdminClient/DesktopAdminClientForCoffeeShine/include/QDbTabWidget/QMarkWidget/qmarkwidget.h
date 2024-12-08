#ifndef QMARKWIDGET_H
#define QMARKWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDebug>
#include "QMarkWidgetItem/qmarkwidgetitem.h"

class QMarkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMarkWidget(QWidget *parent = nullptr);

    void setItems(QVector<WidgetToMarkItemInfo>);
    QVector<WidgetToMarkItemInfo> getListInfoAboutWidgets();
    void setTitle(QString);
    void setMinAndMaxHeight(int,int);

private:
    QHBoxLayout * mainLayout;
    QVBoxLayout * layForGroupBox;
    QHBoxLayout * layForSearchWidget;
    QHBoxLayout * layForButtons;

    QListWidget * listWidget;
    QListWidgetItem *listWidgetItem;
    QMarkWidgetItem *markWidgetItem;

    QGroupBox * mainGroupBox;
    QLineEdit * searchLine;
    QLabel * labelForIcon;
    QPixmap * searchIcon;
    QPushButton * selectAllButton;
    QPushButton * unselectAllButton;

    void createForm();
signals:
    void signalNumberOfCategoriesUsedHasChanged();

public slots:
private slots:
    void slotSearchTextChanged(QString);
};

#endif // QMARKWIDGET_H
