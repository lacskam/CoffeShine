#include "qcolorwidget.h"

QColorWidget::QColorWidget(QWidget *parent) : QWidget(parent)
{
    this->setCursor(Qt::PointingHandCursor);
    this->setMinimumSize(70,20);
    this->setMaximumSize(70,20);
    currentColor = QColor(0, 0, 0);
    _colorWasChanged = false;
}

QColorWidget::QColorWidget(QColor color,QWidget *parent) : QWidget(parent)
{
    this->setCursor(Qt::PointingHandCursor);
    this->setMinimumSize(70,20);
    this->setMaximumSize(70,20);
    currentColor = color;
    _colorWasChanged = false;
}

void QColorWidget::setColor(QColor color)
{
    currentColor = color;
}

QColor QColorWidget::getColor()
{
    return currentColor;
}

bool QColorWidget::colorWasChanged()
{
    return _colorWasChanged;
}

void QColorWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(currentColor));
    painter.drawRect(0,0,width(),height());
}

void QColorWidget::mouseReleaseEvent(QMouseEvent *)
{
    // getting color
    QColor col = QColorDialog::getColor(currentColor,this,tr("Color Dialog"));
    if (col.isValid())
    {
        currentColor = col;
        _colorWasChanged = true;
    }
}
