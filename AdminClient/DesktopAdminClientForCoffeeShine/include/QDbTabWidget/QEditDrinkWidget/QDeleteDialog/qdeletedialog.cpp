#include "qdeletedialog.h"

QDeleteDialog::QDeleteDialog() {

    btnBox = new QDialogButtonBox;

    vd = new QVBoxLayout(this);



    ac = new QPushButton("Удалить");
    de = new QPushButton("Отмена");

     btnBox->addButton(ac,QDialogButtonBox::ButtonRole::AcceptRole);
    btnBox->addButton(de,QDialogButtonBox::ButtonRole::RejectRole);
     connect(ac, &QPushButton::clicked, this, &QDialog::accept);
     connect(de, &QPushButton::clicked, this, &QDialog::reject);
    vd->addWidget(new QLabel("Удалить напиток?"));
    vd->addWidget(btnBox);





}


void QDeleteDialog::setIdDrink(qint32 id) {
    this->id = id;
}

qint32 QDeleteDialog::getIdDrink() {
    return this->id;
}
