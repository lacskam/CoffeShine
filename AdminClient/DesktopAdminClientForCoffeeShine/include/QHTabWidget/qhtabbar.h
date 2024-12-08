#ifndef QHTABBAR_H
#define QHTABBAR_H
#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTabV3>
#include <QDebug>


class QHTabBar : public QTabBar
{
public:
    explicit QHTabBar(QWidget* parent=0);
    ~QHTabBar();
    void setFont(const QFont &font);

protected:
    QSize tabSizeHint(int index) const;
    void paintEvent(QPaintEvent *event);

};

#endif // QHTABBAR_H
