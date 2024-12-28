#include "qloadingwidget.h"
LoadingWidget::LoadingWidget(QWidget *parent): QWidget(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);


    QLabel *loadingLabel = new QLabel(this);
    loadingLabel->setAlignment(Qt::AlignCenter);


    QMovie *movie = new QMovie(":/icons/other/Loading.gif");
     movie->setScaledSize(QSize(200, 200));
    loadingLabel->setMovie(movie);

    movie->start();


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(loadingLabel, 0, Qt::AlignCenter);
    setLayout(layout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}
