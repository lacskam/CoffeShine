#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    createForm();
}

Widget::Widget(QSettings *settings_, QUnNetProtokolCLient *client_, QCoffeeClientPlugin *plugin_, QWidget *parent) : QWidget (parent)
{
    settings = settings_;
    client = client_;
    plugin = plugin_;
    createForm();
    loadSettings();
}

Widget::~Widget()
{
    qDebug()<<"Widget::~Widget";
    saveSettings();
}

void Widget::createForm()
{
    this->setMinimumSize(640,480);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    tabWidget = new QHTabWidget(this);
    tabWidget->setTabPosition(QHTabWidget :: West);
    tabWidget->setIconSize(QSize(48,48));
    mainLayout->addWidget(tabWidget);

    tabWidget->addTab(dbTabWidget = new QDbTabWidget(plugin),QIcon(":/icons/mainTabs/Database.png"),tr("База данных"));
    tabWidget->addTab(statTabWidget = new QStatisticTabWidget(plugin),QIcon(":/icons/mainTabs/Statistic.png"),tr("Статистика"));
    tabWidget->addTab(nnTabWigdet = new QNnTabWidget(plugin),QIcon(":/icons/mainTabs/Nn.png"),tr("Анализ/Прогноз"));
    tabWidget->addTab(new QWidget(),QIcon(":/icons/mainTabs/Settings.png"),tr("Настройки"));
}

void Widget::loadSettings()
{
    int width= settings->value("/MainForm/FormWidth").toInt();
    int height= settings->value("/MainForm/FormHeight").toInt();
    if ((width>0)&&(height>0))
    {
        this->resize(width,height);
    }

    int X = settings->value("/MainForm/FormX").toInt();
    int Y = settings->value("/MainForm/Formy").toInt();

    if ((X>0)&&(Y>0))
    {
        QPoint center = QPoint(X,Y); //получаем координаты центра экрана
        this->move(center);
    }
}

void Widget::saveSettings()
{
    qDebug()<<"setAttribute(Qt::WA_DeleteOnClose)";

    if ((!this->isMaximized())&&(this->width()>0)&&(this->height()>0)&&(this->x()>0)&&(this->y()>0))
    {
        settings->setValue("/MainForm/FormWidth",this->width());
        settings->setValue("/MainForm/FormHeight",this->height());
        settings->setValue("/MainForm/FormX",this->x());
        settings->setValue("/MainForm/Formy",this->y());
    }
}

void Widget::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("Выход"),
                                  tr("Вы уверены что хотите закрыть программу?"),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        emit signalCloseProgramm();
        client->disconnectFromHost();
        client->waitForDisconnected();
        delete this;
       event->accept();
    } else event->ignore();


}
