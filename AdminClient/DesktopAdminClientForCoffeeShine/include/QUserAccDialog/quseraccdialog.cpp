#include "quseraccdialog.h"

QUserAccDialog::QUserAccDialog(QWidget *parent) : QDialog(parent)
{
    createForm();
}

QUserAccDialog::~QUserAccDialog()
{

}

void QUserAccDialog::createForm()
{
    mainLayout = new QGridLayout(this);
    labelLogin = new QLabel(tr("Login"),this);
    mainLayout->addWidget(labelLogin,0,0,1,1);
    editLogin = new QLineEdit(this);
    mainLayout->addWidget(editLogin,0,1,1,1);
    labelPassword = new QLabel(tr("Password"),this);
    mainLayout->addWidget(labelPassword,1,0,1,1);
    editPassword = new QLineEdit(this);
    editPassword->setText("1756");
    editPassword->setEchoMode(QLineEdit::Password);
    mainLayout->addWidget(editPassword,1,1,1,1);

    checkSaveLogin = new QCheckBox(tr("Save login"),this);
    mainLayout->addWidget(checkSaveLogin,2,1,1,1);

    layoutButtons = new QHBoxLayout;
    mainLayout->addLayout(layoutButtons,3,0,1,2);
    buttonOk = new QPushButton(tr("Ok"),this);
    connect(buttonOk,SIGNAL(pressed()),this,SLOT(accept()));
    layoutButtons->addWidget(buttonOk);
    buttonCancel = new QPushButton(tr("Cancel"),this);
    connect(buttonCancel,SIGNAL(pressed()),this,SLOT(reject()));
    layoutButtons->addWidget(buttonCancel);

    setWindowFlag(Qt::WindowStaysOnTopHint);
}

UserAccount QUserAccDialog::getUserAcc(bool *ok, QWidget *parent)
{
    UserAccount Output;
    QUserAccDialog *dlg = new QUserAccDialog(parent);
    dlg->setModal(true);
    dlg->setWindowTitle(tr("Enter login and password"));
    int ret = dlg->exec();
    if ((ret == QDialog::Accepted)&&
            (!dlg->editLogin->text().isEmpty())&&
            (!dlg->editPassword->text().isEmpty()))
    {
        if (ok!=NULL) *ok=true;
        Output.login = dlg->editLogin->text();
        Output.password = dlg->editPassword->text();
        Output.saveLogin = dlg->checkSaveLogin->isChecked();

    } else *ok=false;
    delete dlg;
    return Output;
}

UserAccount QUserAccDialog::getUserAcc(QString login, bool *ok, QWidget *parent)
{
    UserAccount Output;
    QUserAccDialog *dlg = new QUserAccDialog(parent);
    dlg->setModal(true);
    dlg->setWindowTitle(tr("Enter login and password"));
    if (!login.isEmpty())
    {
        dlg->editLogin->setText(login);
        dlg->checkSaveLogin->setChecked(true);
    }

    int ret = dlg->exec();
    if ((ret == QDialog::Accepted)&&
            (!dlg->editLogin->text().isEmpty())&&
            (!dlg->editPassword->text().isEmpty()))
    {
        if (ok!=NULL) *ok=true;
        Output.login = dlg->editLogin->text();
        Output.password = dlg->editPassword->text();
        Output.saveLogin = dlg->checkSaveLogin->isChecked();

    } else *ok=false;
    delete dlg;
    return Output;
}
