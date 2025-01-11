#ifndef QCUSTOMSCROLAREA_H
#define QCUSTOMSCROLAREA_H


#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QScrollArea>
#include <QKeyEvent>

#include <QWheelEvent>


using namespace QtCharts;

class CustomScrollArea : public QScrollArea {
    Q_OBJECT

public:
    explicit CustomScrollArea(QChartView *chartView, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    QChartView *m_chartView;
    bool m_ctrlPressed;
};

#endif // QCUSTOMSCROLAREA_H

