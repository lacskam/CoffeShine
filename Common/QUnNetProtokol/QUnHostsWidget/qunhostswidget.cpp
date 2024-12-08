#include "qunhostswidget.h"

QUnHostsWidget::QUnHostsWidget(QUnNetProtokolCLient *client, QWidget *parent)
    : QListWidget(parent)
{
    iconHost = new QIcon(":/hostsWidget/icons/host.png");
    iconActiveHost = new QIcon(":/hostsWidget/icons/hostActive.png");

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(slotShowMenuContext(QPoint)));

    currentClient = client;
    connect(client,SIGNAL(signalCurrentHostIndexChanged(int)),
            this,SLOT(slotHostIndexChanged(int)));
    currentHostIndex = client->currentHostIndex();

    updateHostsList();
}

QUnHostsWidget::~QUnHostsWidget()
{

}

void QUnHostsWidget::updateHostsList()
{
    for (int i=0;i<listHosts.length();i++) {
        delete listHosts.at(i).item;
    }

    listHosts.clear();
    clear();

    QVector<HostInfo> buf = currentClient->hosts();

    listHostsItem currentItem;
    for (int i=0;i<buf.length();i++) {
        currentItem.host = buf.at(i).address;
        currentItem.port = buf.at(i).port;
        currentItem.description = buf.at(i).description;

        if (i==currentHostIndex)
            currentItem.item= new QListWidgetItem(*iconActiveHost,
                                                  currentItem.host + "(" +
                                                  QString::number(currentItem.port) +
                                                  ")",this);
            else
            currentItem.item= new QListWidgetItem(*iconHost,
                                                  currentItem.host + "(" +
                                                  QString::number(currentItem.port) +
                                                  ")",this);

        if (!currentItem.description.isEmpty())
            currentItem.item->setToolTip(currentItem.description);

        addItem(currentItem.item);
        listHosts.push_back(currentItem);
    }


}

void QUnHostsWidget::slotHostIndexChanged(int index)
{
    currentHostIndex = index;
    updateHostsList();
}

void QUnHostsWidget::slotShowMenuContext(QPoint pos)
{
    QMenu contextMenu(tr("Menu hosts"),this);

    actionAddHost = new QAction(tr("Add host"),this);
    connect(actionAddHost,SIGNAL(triggered(bool)),this,SLOT(slotAddHost()));
    contextMenu.addAction(actionAddHost);

    actionEditHost = new QAction(tr("Edit host"),this);
    connect(actionEditHost,SIGNAL(triggered(bool)),this,SLOT(slotEditHost()));
    contextMenu.addAction(actionEditHost);

   actionDeleteHost = new QAction(tr("Delete host"),this);
   connect(actionDeleteHost,SIGNAL(triggered(bool)),this,SLOT(slotDeleteHost()));
   contextMenu.addAction(actionDeleteHost);

    contextMenu.exec(mapToGlobal(pos));
}

void QUnHostsWidget::slotAddHost()
{
    bool ok;
    HostInfo host = QUnHostsDialog::addHost(&ok,this);
    if (ok) {
        currentClient->setHost(host);
        updateHostsList();
    }
}

void QUnHostsWidget::slotEditHost()
{
    qDebug()<<"slotEditHost";
    listHostsItem currentHostItem = listHosts.at(currentIndex().row());
    bool ok;
    HostInfo oldHost;
    oldHost.address = currentHostItem.host;
    oldHost.port = currentHostItem.port;
    oldHost.description = currentHostItem.description;
    HostInfo host = QUnHostsDialog::editHost(oldHost,&ok,this);

    if (ok) {
        currentClient->editHost(currentIndex().row(),host);
        updateHostsList();
    }
}

void QUnHostsWidget::slotDeleteHost()
{
    qDebug()<<"slotDeleteHost";
    currentClient->deleteHost(currentIndex().row());
    updateHostsList();
}
