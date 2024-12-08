#include "qhtabbar.h"

QHTabBar::QHTabBar(QWidget *parent)  : QTabBar(parent)
{
    this->setParent(parent);
    setIconSize(QSize(18, 18));
    this->setExpanding(true);
}

QHTabBar::~QHTabBar()
{

}


QSize QHTabBar::tabSizeHint(int index) const
{
    QSize A = QTabBar::tabSizeHint(index);

    if (A.height()<this->font().pointSize()) A.setHeight(this->font().pointSize());
     A.setHeight(A.height()/2);
     QFontMetrics fm(this->font());
     A.setWidth(fm.width(this->tabText(index))+this->iconSize().width()+fm.width(this->tabText(index))/3+35);

     if (fm.height()>this->iconSize().height())
     {
         A.setHeight(fm.height()*1.5);
     } else A.setHeight(this->iconSize().height()*1.5);

     return A;
}


void QHTabBar::paintEvent(QPaintEvent *event)
    {

        QStylePainter p(this);
        for (int index = 0; index < count(); index++)
        {
            QStyleOptionTabV3 tab;
            initStyleOption(&tab, index);
            QIcon tempIcon = tab.icon;

            QString tempText = tab.text;
            tab.icon = QIcon();
            tab.text = QString();
            p.drawControl(QStyle::CE_TabBarTab, tab);
            QPainter painter;
            painter.begin(this);
            QRect tabrect = tabRect(index);
            tabrect.adjust(10, 8, 0, -8);
            //painter.drawText();
            //painter.drawText();
            QFontMetrics fm(this->font());

            int w=fm.width(tempText);
            int h=fm.height();
            int x=tabrect.left()+this->iconSize().width()+fm.width("  ");
            int y=tabrect.center().y()-h/2;

            painter.drawText(x,y,w,h,Qt::AlignRight ,tempText);
            //painter.drawText(tabrect, Qt::AlignCenter | Qt::AlignHCenter, tempText);
            tempIcon.paint(&painter, 10, tabrect.center().y()-iconSize().height()/2, tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter);
            painter.end();


        }
    }

void QHTabBar::setFont(const QFont &font)
{
    qDebug()<<"QHTabBar Set font";
    QTabBar::setFont(font);

}
