#include "qhostsdialog.h"

QHostsDialog::QHostsDialog(QWidget *parent) : QDialog(parent)
{
    setParent(parent);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    createForm();
}

QHostsDialog::QHostsDialog(HostInfo host,QWidget *parent)
{
    setParent(parent);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    createForm();

    editAddress->setText(host.address);
    editPort->setText(QString::number(host.port));
}

QHostsDialog::~QHostsDialog ()
{

}

void QHostsDialog::createForm()
{
    mainlayout = new QGridLayout(this);
    labelAddress = new QLabel(tr("Address"),this);
    mainlayout->addWidget(labelAddress,0,0,1,1);
    editAddress = new QLineEdit(this);
    editAddress->setText("127.0.0.1");

    mainlayout->addWidget(editAddress,0,1,1,1);
    labelPort = new QLabel(tr("Port"),this);
    mainlayout->addWidget(labelPort,1,0,1,1);
    editPort = new QLineEdit(this);
    editPort->setText("7777");
    editPort->setInputMask("99999");

    mainlayout->addWidget(editPort);
    layoutButtons = new QHBoxLayout;
    layoutButtons->setMargin(0);
    mainlayout->addLayout(layoutButtons,2,0,1,2);
    buttonOk = new QPushButton(tr("Ok"),this);
    connect(buttonOk,SIGNAL(pressed()),this,SLOT(accept()));
    layoutButtons->addWidget(buttonOk);
    buttonCancel = new QPushButton(tr("Cancel"),this);
    connect(buttonCancel,SIGNAL(pressed()),this,SLOT(reject()));
    layoutButtons->addWidget(buttonCancel);
    setWindowFlag(Qt::WindowStaysOnTopHint);

}

HostInfo QHostsDialog::getHost(bool *ok, QWidget *parent)
{
    HostInfo Output;
    QHostsDialog *dlg = new QHostsDialog(parent);
    dlg->setModal(true);
    dlg->setWindowTitle(tr("Enter new host"));
    int ret = dlg->exec();
    if (ret == QDialog::Accepted)
    {
        if (ok!=NULL) *ok=true;
        Output.address = dlg->editAddress->text();
        Output.port = dlg->editPort->text().toInt();
    } else *ok=false;
    delete dlg;
    return Output;
}

HostInfo QHostsDialog::editHost(HostInfo host,bool *ok, QWidget *parent)
{
    HostInfo Output;
    QHostsDialog *dlg = new QHostsDialog(host,parent);
    dlg->setModal(true);
    dlg->setWindowTitle(tr("Edit host"));
    int ret = dlg->exec();
    if (ret == QDialog::Accepted)
    {
        if (ok!=NULL) *ok=true;
        Output.address = dlg->editAddress->text();
        Output.port = dlg->editPort->text().toInt();
    } else *ok=false;
    delete dlg;
    return Output;
}
