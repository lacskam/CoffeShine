 #include "QCustomScrollArea.h"



CustomScrollArea::CustomScrollArea(QChartView *chartView, QWidget *parent)
    : QScrollArea(parent), m_chartView(chartView), m_ctrlPressed(false) {
    setWidget(chartView);
    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    m_chartView->setDragMode(QGraphicsView::ScrollHandDrag);
}

void CustomScrollArea::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_ctrlPressed = true;
    }
    QScrollArea::keyPressEvent(event);
}

void CustomScrollArea::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_ctrlPressed = false;
    }
    QScrollArea::keyReleaseEvent(event);
}

void CustomScrollArea::wheelEvent(QWheelEvent *event) {
    if (m_ctrlPressed) {

        QValueAxis *axisX = qobject_cast<QValueAxis *>(m_chartView->chart()->axes(Qt::Horizontal).first());
        QValueAxis *axisY = qobject_cast<QValueAxis *>(m_chartView->chart()->axes(Qt::Vertical).first());

        if (axisX) {
            qreal factor = (event->angleDelta().y() > 0) ? 0.9 : 1.1;
            qreal newMinX = axisX->min() * factor;
            qreal newMaxX = axisX->max() * factor;
            axisX->setRange(0, newMaxX);
        }
        // if (axisY) {
        //     qreal factor = (event->angleDelta().y() > 0) ? 0.9 : 1.1;
        //     qreal newMinY = axisY->min() * factor;
        //     qreal newMaxY = axisY->max() * factor;
        //     axisY->setRange(newMinY, newMaxY);
        // }
    } else {
        // Прокрутка области
        QScrollArea::wheelEvent(event);
    }
}
