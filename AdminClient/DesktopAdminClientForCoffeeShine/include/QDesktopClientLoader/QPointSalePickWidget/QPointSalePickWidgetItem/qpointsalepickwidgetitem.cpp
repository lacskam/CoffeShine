#include "qpointsalepickwidgetitem.h"




void QPointSalePickWidgetItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(123,12,46)));
    QRectF rectangle(5,70,width()-5,height()-70);
    painter.drawRoundedRect(rectangle, 10, 10);
}

void QPointSalePickWidgetItem::sizeScale(QSize sizeT) {


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

}

void QPointSalePickWidgetItem::enterEvent(QEvent *e) {

        if (originalSize.isEmpty()) {
            originalSize = size();
        }


        sizeScale(QSize(originalSize.width() + 8, originalSize.height() + 8));


    QWidget::enterEvent(e);
}

void QPointSalePickWidgetItem::leaveEvent(QEvent *e) {


        if (!isAnimating) {
            sizeScale(originalSize);
        } else {

            QTimer::singleShot(70, this, [this]() {
                sizeScale(originalSize);
            });
        }


    QWidget::leaveEvent(e);
}

QPointSalePickWidgetItem::QPointSalePickWidgetItem(QCoffeeClientPlugin *currentPlugin_, QCoffeePointSale pointSaleInfo_,QWidget *parent) : QWidget(parent)
{
    isTabWidget=0;
    parent_ = parent;
    pointSaleInfo = pointSaleInfo_;
    currentPlugin = currentPlugin_;

    QVBoxLayout *lay = new QVBoxLayout(this);

    pointSaleNamef = new QLabel();
    pointSaleNamef->setAlignment(Qt::AlignCenter);

    pointSaleNamef->setMinimumHeight(60);
    lay->addWidget(pointSaleNamef);
    pointSaleNamef->setText(pointSaleInfo_.name);
    pointSaleNamef->setFont(QFont("Sans",11));

    pointSaleNamef->setWordWrap(true);

    pointSaleName = new QLabel();
    pointSaleName->setAlignment(Qt::AlignCenter);
    pointSaleName->setText(pointSaleInfo.name);
    lay->addWidget(pointSaleName);
    pointSaleName->setWordWrap(true);
    pointSaleName->setMinimumSize(130,120);

    QPixmap  picture(":/icons/other/pointSale.png");

    picture = picture.scaled(120,120,Qt::KeepAspectRatio);
    pointSaleName->setPixmap(picture);



    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(slotShowMenuContext(QPoint)));

}





QPointSalePickWidgetItem::QPointSalePickWidgetItem(QCoffeeClientPlugin *currentPlugin_, QCoffeePointSale pointSaleInfo_,bool isTabWidget,QWidget *parent) : QWidget(parent) {
    this->isTabWidget=1;
    parent_ = parent;
    pointSaleInfo = pointSaleInfo_;
    currentPlugin = currentPlugin_;


    parent_ = parent;
    pointSaleInfo = pointSaleInfo_;
    currentPlugin = currentPlugin_;

    QVBoxLayout *lay = new QVBoxLayout(this);

    pointSaleNamef = new QLabel();
    pointSaleNamef->setAlignment(Qt::AlignCenter);

    pointSaleNamef->setMinimumHeight(60);
    lay->addWidget(pointSaleNamef);
    pointSaleNamef->setText(pointSaleInfo_.name);
    pointSaleNamef->setFont(QFont("Sans",11));

    pointSaleNamef->setWordWrap(true);

    pointSaleName = new QLabel();
    pointSaleName->setAlignment(Qt::AlignCenter);
    pointSaleName->setText(pointSaleInfo.name);
    lay->addWidget(pointSaleName);
    pointSaleName->setWordWrap(true);
    pointSaleName->setMinimumSize(130,120);

    QPixmap  picture(":/icons/other/pointSale.png");

    picture = picture.scaled(120,120,Qt::KeepAspectRatio);
    pointSaleName->setPixmap(picture);



    setContextMenuPolicy(Qt::CustomContextMenu);



}
void QPointSalePickWidgetItem::slotShowMenuContext(QPoint pos)
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

// void QPointSalePickWidgetItem::mouseDoubleClickEvent(QMouseEvent *)
// {
//     emit signalOpenEditDrinkWidget(drinkInfo);
// }

void QPointSalePickWidgetItem::mousePressEvent(QMouseEvent*) {
    slotmousePressEvent();

}




void QPointSalePickWidgetItem::slotmousePressEvent() {
    if (!isTabWidget) {
        if (isPicked) {
            qDebug() << isPicked;
            isPicked = false;
            pointSaleNamef->setFont(QFont("Sans", 11));
            pointSaleNamef->setStyleSheet("color: rgb(255, 255, 255)");
        } else {
            qDebug() << isPicked;
            isPicked = true;
            pointSaleNamef->setFont(QFont("Sans", 12));
            pointSaleNamef->setStyleSheet("color: rgb(0, 204, 255)");
        }

        update();
    }
}



void QPointSalePickWidgetItem::mouseDoubleClickEvent(QMouseEvent*) {
    if (isTabWidget) {
        emit signalOpenEditPickWidget(pointSaleInfo);

    }
}


