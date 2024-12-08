#ifndef QUNHOSTSWIDGET_H
#define QUNHOSTSWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVector>
#include <QIcon>
#include <QMenu>
#include <QAction>

#include <QDebug>

#include "../qunnetprotokolclient.h"
#include "../QUnHostsDialog/qunhostsdialog.h"

class QUnHostsWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QUnHostsWidget(QUnNetProtokolCLient *client,QWidget *parent = nullptr);
    ~QUnHostsWidget();

private:
    QUnNetProtokolCLient *currentClient;
    struct listHostsItem
    {
        QString host;
        int port;
        QListWidgetItem *item;
        QString description;
    };

    int currentHostIndex=0;
    QVector<listHostsItem> listHosts;
    void updateHostsList();

    QIcon *iconHost;
    QIcon *iconActiveHost;

    QAction *actionAddHost;
    QAction *actionEditHost;
    QAction *actionDeleteHost;

signals:

public slots:

private slots:
    void slotShowMenuContext(QPoint pos);

    void slotAddHost();
    void slotEditHost();
    void slotDeleteHost();

    void slotHostIndexChanged(int index);
};

#endif // QUNHOSTSWIDGET_H
