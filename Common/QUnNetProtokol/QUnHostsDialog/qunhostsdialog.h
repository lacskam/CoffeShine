#ifndef QUNHOSTSDIALOG_H
#define QUNHOSTSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QtWidgets>
#include <QMessageBox>
#include <QRegularExpression>
#include "../qunnetprotokolclient.h"

class QUnHostsDialog : public QDialog
{
    Q_OBJECT
public:
    ~QUnHostsDialog();
    static HostInfo addHost (bool *ok=NULL, QWidget *parent=NULL);
    static HostInfo editHost (HostInfo host,bool *ok=NULL, QWidget *parent=NULL);

private:
    QUnHostsDialog(QWidget *parent=NULL);
    QUnHostsDialog(HostInfo host, QWidget *parent=NULL);

    enum typeHostAddress {
        typeIPv4,
        typeDomain
    };

    void createForm();
    QGridLayout *mainlayout;
    QLabel *labelType;
    QComboBox *comboType;
    QLabel *labelHost;
    QLineEdit *edtHosts;
    QLabel *labelPort;
    QLineEdit *editPort;
    QLabel *labelDescription;
    QLineEdit *editDescription;

    QHBoxLayout *layoutButtons;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void updateComboType();
    void updateHostInfo(HostInfo info);

    bool isIpV4Address();
    bool isDomainName();

private slots:
    void slotPushOk();
    void slotPushCancel();

    void slotComboTypeChanged(int index);

};

#endif // QUNHOSTSDIALOG_H
