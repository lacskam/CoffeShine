#include "qunhostsdialog.h"

QUnHostsDialog::QUnHostsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    createForm();
}

QUnHostsDialog::QUnHostsDialog(HostInfo host, QWidget *parent)
{
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    createForm();
}

QUnHostsDialog::~QUnHostsDialog()
{

}

HostInfo QUnHostsDialog::addHost (bool *ok, QWidget *parent)
{
    HostInfo Output;
    QUnHostsDialog *dlg = new QUnHostsDialog(parent);
    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        Output.address = dlg->edtHosts->text();
        Output.port = dlg->editPort->text().toInt();
        Output.description = dlg->editDescription->text();

        if (ok!=NULL)
            *ok = true;
    }

    if (ret==QDialog::Rejected) {

        if (ok!=NULL)
            *ok = false;
    }

    return Output;
}

HostInfo QUnHostsDialog::editHost (HostInfo host,bool *ok, QWidget *parent)
{
    HostInfo Output;

    QUnHostsDialog *dlg = new QUnHostsDialog(parent);
    dlg->edtHosts->setText(host.address);
    dlg->editPort->setText(QString::number(host.port));
    dlg->editDescription->setText(host.description);

    int ret = dlg->exec();
    if (ret == QDialog::Accepted) {
        Output.address = dlg->edtHosts->text();
        Output.port = dlg->editPort->text().toInt();
        Output.description = dlg->editDescription->text();

        if (ok!=NULL)
            *ok = true;
    }

    if (ret==QDialog::Rejected) {

        if (ok!=NULL)
            *ok = false;
    }

    return Output;
}

void QUnHostsDialog::createForm()
{
    mainlayout = new QGridLayout(this);
    labelType = new QLabel(tr("Type"),this);
    mainlayout->addWidget(labelType,0,0,1,1);
    comboType = new QComboBox(this);
    mainlayout->addWidget(comboType,0,1,1,1);

    labelHost = new QLabel(tr("Address"),this);
    mainlayout->addWidget(labelHost,1,0,1,1);
    edtHosts = new QLineEdit(this);
    mainlayout->addWidget(edtHosts,1,1,1,1);

    labelPort = new QLabel(tr("Port"),this);
    mainlayout->addWidget(labelPort,2,0,1,1);
    editPort = new QLineEdit(this);
    editPort->setInputMask("99999");
    editPort->setCursorPosition(0);
    mainlayout->addWidget(editPort,2,1,1,1);

    labelDescription = new QLabel(tr("Description"),this);
    mainlayout->addWidget(labelDescription,3,0,1,1);
    editDescription = new QLineEdit(this);
    mainlayout->addWidget(editDescription,3,1,1,1);

    layoutButtons = new QHBoxLayout(this);
    mainlayout->addLayout(layoutButtons,5,0,1,2);
    buttonOk = new QPushButton(tr("Ok"),this);
    connect(buttonOk,SIGNAL(pressed()),this,SLOT(slotPushOk()));
    layoutButtons->addWidget(buttonOk);

    buttonCancel = new QPushButton(tr("Cancel"),this);
    connect(buttonCancel,SIGNAL(pressed()),this,SLOT(slotPushCancel()));
    layoutButtons->addWidget(buttonCancel);

    connect(comboType,SIGNAL(currentIndexChanged(int)),this,SLOT(slotComboTypeChanged(int)));
    updateComboType();
}

void QUnHostsDialog::slotPushOk()
{
    qDebug()<<edtHosts->text();
    bool result = true;
    if (comboType->currentData().toInt() == typeDomain) {
        if (!isDomainName()) {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Enter the correct domain name."));
            result = false;
        }

        if (((editPort->text().toInt() <0) ||
                (editPort->text().toInt() >65535) ||
                        (editPort->text().isEmpty())) && result) {

            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Enter the correct port."));
            result = false;
        }
    }

    if (comboType->currentData().toInt() == typeIPv4) {
        if (!isIpV4Address()) {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Enter the correct ip address."));
            result = false;
        }

        if (((editPort->text().toInt() <0) ||
                (editPort->text().toInt() >65535) ||
                        (editPort->text().isEmpty())) && result) {

            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Enter the correct port."));
            result = false;
        }
    }

    if (result)
        emit accept();
}

void QUnHostsDialog::slotPushCancel()
{
    emit reject();
}

void QUnHostsDialog::slotComboTypeChanged(int index)
{
    switch (comboType->currentData().toInt()) {
    case typeDomain: {
            edtHosts->setInputMask("");
            edtHosts->setCursorPosition(0);
        }
        break;
    case typeIPv4: {
            edtHosts->setInputMask("000.000.000.000;_");
            edtHosts->setCursorPosition(0);
        }
        break;
    }
}

void QUnHostsDialog::updateComboType()
{
    comboType->clear();
    comboType->addItem(tr("Domain name"),typeDomain);
    comboType->addItem(tr("Address IPv4"),typeIPv4);
}

bool QUnHostsDialog::isIpV4Address()
{
    bool Output = false;

    QString buf = edtHosts->text();
    QString regular = "\\d.\\d.\\d.\\d";

    if (buf.indexOf(QRegExp(regular),0)>=0)
        Output = true;

    return Output;
}

bool QUnHostsDialog::isDomainName()
{
    bool Output = false;

    QString buf = edtHosts->text();
    if ((buf.indexOf('.')>0) && (buf.indexOf('.')<buf.length()-1))
        Output=true;

    return Output;
}
