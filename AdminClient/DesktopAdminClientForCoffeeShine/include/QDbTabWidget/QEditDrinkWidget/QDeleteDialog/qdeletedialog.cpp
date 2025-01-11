#include "qdeletedialog.h"

QDeleteDialog::QDeleteDialog(QString labelText,QWidget *parent)  {

    btnBox = new QDialogButtonBox;

    vd = new QVBoxLayout(this);



    ac = new QPushButton("Да");
    de = new QPushButton("Отмена");

     btnBox->addButton(ac,QDialogButtonBox::ButtonRole::AcceptRole);
    btnBox->addButton(de,QDialogButtonBox::ButtonRole::RejectRole);
     connect(ac, &QPushButton::clicked, this, &QDialog::accept);
     connect(de, &QPushButton::clicked, this, &QDialog::reject);
       vd->addSpacing(10);
    vd->addWidget(new QLabel(labelText));
    vd->addSpacing(30);

    vd->addWidget(btnBox);





}


void QDeleteDialog::setIdDrink(qint32 id) {
    this->id = id;
}

qint32 QDeleteDialog::getIdDrink() {
    return this->id;
}
