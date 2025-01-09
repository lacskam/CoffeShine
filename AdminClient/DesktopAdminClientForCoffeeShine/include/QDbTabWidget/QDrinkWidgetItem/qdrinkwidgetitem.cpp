#include "qdrinkwidgetitem.h"

QCoffeeDrinkInfo QDrinkWidgetItem::getDrinkInfo()
{
    return drinkInfo;
}

void QDrinkWidgetItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(drinkInfo.color.red(),
                                   drinkInfo.color.green(),
                                   drinkInfo.color.blue())));
    QRectF rectangle(5,70,width()-5,height()-70);
    painter.drawRoundedRect(rectangle, 10, 10);
}
void QDrinkWidgetItem::sizeScale(QSize sizeT) {
    if (flagAnim == 1 || flagAnim == 3 || flagAnim == 4) {

        if (size() != sizeT && !isAnimating) {
            isAnimating = true;
            QPropertyAnimation *anim = new QPropertyAnimation(this, "size");
            anim->setDuration(70);
            anim->setStartValue(size());
            anim->setEndValue(sizeT);
            connect(anim, &QPropertyAnimation::finished, this, [this]() {
                isAnimating = false;
            });
            anim->start();
        }
    } else {

        resize(sizeT);
    }
}

void QDrinkWidgetItem::enterEvent(QEvent *e) {
    if (flagAnim == 1 || flagAnim == 3 || flagAnim == 4) {
        if (originalSize.isEmpty()) {
            originalSize = size();
        }


        sizeScale(QSize(originalSize.width() + 8, originalSize.height() + 8));
    } else {

        resize(originalSize.width() + 8, originalSize.height() + 8);
    }

    QWidget::enterEvent(e);
}

void QDrinkWidgetItem::leaveEvent(QEvent *e) {
    if (flagAnim == 1 || flagAnim == 3 || flagAnim == 4) {

        if (!isAnimating) {
            sizeScale(originalSize);
        } else {

            QTimer::singleShot(70, this, [this]() {
                sizeScale(originalSize);
            });
        }
    } else {

        resize(originalSize);
    }

    QWidget::leaveEvent(e);
}

QDrinkWidgetItem::QDrinkWidgetItem(QCoffeeDrinkInfo info, QCoffeeClientPlugin* plugin_, qint32 flagAnim, QListWidget *parentWidget, QWidget *parent) : QWidget(parent)
{
    this->parentWidget = parentWidget;
    this->flagAnim = flagAnim;
    isPickedForCategory=0;
    currentPlugin = plugin_;
    drinkInfo = info;
    if (flagAnim==3) {
        isPickedForCategory=1;
    }
    QVBoxLayout *lay = new QVBoxLayout(this);

    drinkNamef = new QLabel();
    drinkNamef->setAlignment(Qt::AlignCenter);

    drinkNamef->setMinimumHeight(60);
     drinkNamef->setMaximumWidth(170);
    lay->addWidget(drinkNamef);
    drinkNamef->setText(drinkInfo.name);
    drinkNamef->setFont(QFont("Sans",11));

    drinkNamef->setWordWrap(true);

    drinkName = new QLabel();
    drinkName->setAlignment(Qt::AlignCenter);
    drinkName->setText(drinkInfo.name);
    lay->addWidget(drinkName);
    drinkName->setWordWrap(true);
    drinkName->setMinimumSize(130,120);

    QPixmap  picture;
    picture.loadFromData(currentPlugin->getPicture(drinkInfo.idPicture).data);
    picture = picture.scaled(110,120,Qt::KeepAspectRatio);
    drinkName->setPixmap(picture);


    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(slotShowMenuContext(QPoint)));
    setMinimumSize( QSize(152,208));


    if (flagAnim==2) {
        setFixedSize(129,190);

        picture = picture.scaled(20,20,Qt::KeepAspectRatio);
    }


}

void QDrinkWidgetItem::slotShowMenuContext(QPoint pos)
{
  //   QMenu contextMenu(tr("Menu hosts"),this);

  //   // actionAddHost = new QAction(tr("Add host"),this);
  //   // connect(actionAddHost,SIGNAL(triggered(bool)),this,SLOT(slotAddHost()));
  //   // contextMenu.addAction(actionAddHost);

  //   actionEditHost = new QAction(tr("Изменить"),this);
  // // connect(actionEditHost,SIGNAL(triggered(bool)),this,SLOT(signalOpenEditDrinkWidget()));
  //   contextMenu.addAction(actionEditHost);

  //  actionDeleteHost = new QAction(tr("Удалить"),this);
  //   connect(actionDeleteHost,&QAction::triggered,this,[=] () {
  //       deletedialogIt = new QDeleteDialog;
  //        deletedialogIt->setIdDrink(drinkInfo.id);
  //       deletedialogIt->show();
  //  });
  //  connect(deletedialogIt,&QDeleteDialog::accepted,this,[=]() {
  //      deletedialogIt->hide();
  //       slotDeleteDrink(deletedialogIt->getIdDrink());

  //  });
  //  connect(deletedialogIt,&QDeleteDialog::rejected,this,[=]() {
  //      deletedialogIt->hide();

  //  });
  //  contextMenu.addAction(actionDeleteHost);

  //   contextMenu.exec(mapToGlobal(pos));
}

void QDrinkWidgetItem::mouseDoubleClickEvent(QMouseEvent *)
{
    emit signalOpenEditDrinkWidget(drinkInfo);
}

void QDrinkWidgetItem::mousePressEvent(QMouseEvent*) {

    // if (flagAnim==3 ||flagAnim==4) {
    //     if (isPickedForCategory) {
    //         isPickedForCategory=0;
    //         drinkNamef->setFont(QFont("Sans",11));
    //         drinkNamef->setStyleSheet("color: rgb(255, 255, 255)");
    //         isCliced=1;
    //         if (flagAnim==4) {
    //              isCliced=0;
    //         }



    //     } else {
    //         isPickedForCategory=1;
    //         drinkNamef->setFont(QFont("Sans",12));
    //         drinkNamef->setStyleSheet("color: rgb(0, 204, 255)");
    //         isCliced=1;
    //         if (flagAnim==3) {
    //             isCliced=0;
    //         }



    //     }
    // }
    emit signalClicked();

}


void QDrinkWidgetItem::updateCoffeWidget(QCoffeeDrinkInfo)
{

}

void QDrinkWidgetItem::slotDeleteDrink(qint32 id) {
    qDebug()<<"Удаление напитка";
    QCoffeeDrinkInfo Output;
    Output.id = id;
    currentPlugin->crudDrinkInfo(Output,0x03);

   emit signalUpdateListWidget();
}
