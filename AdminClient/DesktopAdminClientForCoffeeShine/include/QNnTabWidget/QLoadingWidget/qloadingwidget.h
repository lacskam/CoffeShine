#ifndef QLOADINGWIDGET_H
#define QLOADINGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>

class LoadingWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoadingWidget(QWidget *parent = nullptr);
};

#endif // QLOADINGWIDGET_H
