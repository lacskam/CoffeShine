#ifndef QUSERDIALOG_H
#define QUSERDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

struct UserAccount
{
    QString login;
    QString password;
    bool saveLogin;
};

class QUserAccDialog : public QDialog
{
    Q_OBJECT
public:
    static UserAccount getUserAcc(bool *ok=NULL, QWidget *parent=NULL);
    static UserAccount getUserAcc(QString login,bool *ok=NULL, QWidget *parent=NULL);
    ~QUserAccDialog();

private:
    QUserAccDialog(QWidget *parent = NULL);
    void createForm();

    QGridLayout *mainLayout;
    QLabel *labelLogin;
    QLineEdit *editLogin;
    QLabel *labelPassword;
    QLineEdit *editPassword;

    QHBoxLayout *layoutButtons;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;
    QPushButton *buttonHosts;

    QCheckBox *checkSaveLogin;
};

#endif // QUSERDIALOG_H
