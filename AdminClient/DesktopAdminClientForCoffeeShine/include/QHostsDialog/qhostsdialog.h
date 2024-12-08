#ifndef QHOSTSDIALOG_H
#define QHOSTSDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>

#include "../../Common/QUnNetProtokol/commonunnetworktypes.h"

class QHostsDialog : public QDialog
{
    Q_OBJECT

public:
    static HostInfo getHost(bool *ok=NULL, QWidget *parent=NULL);
    static HostInfo editHost(HostInfo host,bool *ok=NULL, QWidget *parent=NULL);
    ~QHostsDialog();

private:
    QHostsDialog(QWidget *parent = NULL);
    QHostsDialog(HostInfo host,QWidget *parent = NULL);
    void createForm();
    QGridLayout *mainlayout;
    QLabel *labelAddress;
    QLineEdit *editAddress;
    QLabel *labelPort;
    QLineEdit *editPort;
    QHBoxLayout *layoutButtons;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

};

#endif // QHOSTSDIALOG_H
