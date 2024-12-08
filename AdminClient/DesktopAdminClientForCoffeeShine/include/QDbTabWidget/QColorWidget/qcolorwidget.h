#ifndef QCOLORWIDGET_H
#define QCOLORWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QColorDialog>
//#include <QDialog>

class QColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QColorWidget(QWidget *parent = nullptr);
    explicit QColorWidget(QColor color, QWidget *parent = nullptr);

    void setColor(QColor);
    QColor getColor();
    bool colorWasChanged();
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
signals:

public slots:

private:
    QColor currentColor;
    bool _colorWasChanged;

private slots:

};

#endif // QCOLORWIDGET_H
