#include "qsentolanedebugwidget.h"

QSentoLanEDebugWidget::QSentoLanEDebugWidget(QSentoLanE *device, QWidget *parent) : QWidget(parent)
{
    currentSettings = NULL;
    connect(device,SIGNAL(signalRxData(QByteArray)),this,SLOT(slotRxData(QByteArray)));
    connect(device,SIGNAL(signalTxData(QByteArray)),this,SLOT(slotTxData(QByteArray)));

    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    text = new QTextEdit(this);
    text->setReadOnly(true);
    mainLayout->addWidget(text);
    setAttribute(Qt::WA_DeleteOnClose);

    text->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    text->setTextColor(QColor(Qt::green));

    loadSettings();
}

QSentoLanEDebugWidget::QSentoLanEDebugWidget(QSentoLanE *device,QSettings *settings,QWidget *parent) : QWidget(parent)
{
    currentSettings = settings;
    connect(device,SIGNAL(signalRxData(QByteArray)),this,SLOT(slotRxData(QByteArray)));
    connect(device,SIGNAL(signalTxData(QByteArray)),this,SLOT(slotTxData(QByteArray)));

    mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    text = new QTextEdit(this);
    text->setReadOnly(true);
    mainLayout->addWidget(text);
    setAttribute(Qt::WA_DeleteOnClose);

    text->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
    text->setTextColor(QColor(Qt::green));

    loadSettings();
}

QSentoLanEDebugWidget::~QSentoLanEDebugWidget()
{
    saveSettings();
}

void QSentoLanEDebugWidget::resizeEvent(QResizeEvent *event)
{
    saveSettings();
}

void QSentoLanEDebugWidget::loadSettings()
{
    if (currentSettings != NULL)
    {
        quint16 width= currentSettings->value("/debugWindow/FormWidth").toInt();
        quint16 height= currentSettings->value("/debugWindow/FormHeight").toInt();
        if ((width>0)&&(height>0))
        {

            this->resize(width,height);
        }

        quint16 X = currentSettings->value("/debugWindow/FormX").toInt();
        quint16 Y = currentSettings->value("/debugWindow/Formy").toInt();

        if ((X>0)&&(Y>0))
        {
            QPoint center = QPoint(X,Y); //получаем координаты центра экрана
            this->move(center);
        }
    }

}

void QSentoLanEDebugWidget::saveSettings()
{
    if (currentSettings != NULL)
    {
        if ((!this->isMaximized())&&(this->width()>0)&&(this->height()>0)&&(this->x()>0)&&(this->y()>0))
        {
            currentSettings->setValue("/debugWindow/FormWidth",this->width());
            currentSettings->setValue("/debugWindow/FormHeight",this->height());
            currentSettings->setValue("/debugWindow/FormX",this->x());
            currentSettings->setValue("/debugWindow/Formy",this->y());
        }
    }
}

void QSentoLanEDebugWidget::slotTxData (QByteArray data)
{
    QString Output = "TX>";
    quint8 currentByte;
    QByteArray buf;
    for (int i=0;i<data.length();i++)
    {
        currentByte = data.at(i);
        switch (currentByte) {
        case QSentoLanE::cbyteENQ: Output = Output + "<ENQ>";
            break;
        case QSentoLanE::cbyteACK: Output = Output + "<ACK>";
            break;
        case QSentoLanE::cbyteNAK: Output = Output + "<NAK>";
            break;
        case QSentoLanE::cbyteSTX: Output = Output + "<STX>";
            break;
        case QSentoLanE::cbyteETX: Output = Output + "<ETX>";
            break;
        case QSentoLanE::cbyteETB: Output = Output + "<ETB>";
            break;
        case QSentoLanE::cbyteEOT: Output = Output + "<EOT>";
            break;
        case QSentoLanE::cbyteWACK: Output = Output + "<WACK>";
            break;
        case QSentoLanE::cbyteRVI: Output = Output + "<RVI>";
            break;
        default:
            {
                buf.clear();
                buf = data.mid(i,1);
                Output = Output + fromCP1251(&buf);
                Output.replace(" ",".");
            }
            break;

        }
    }

    text->append(Output);
}


void QSentoLanEDebugWidget::slotRxData (QByteArray data)
{
    QString Output = "RX>";
    quint8 currentByte;
    QByteArray buf;
    for (int i=0;i<data.length();i++)
    {
        currentByte = data.at(i);
        switch (currentByte) {
        case QSentoLanE::cbyteENQ: Output = Output + "<ENQ>";
            break;
        case QSentoLanE::cbyteACK: Output = Output + "<ACK>";
            break;
        case QSentoLanE::cbyteNAK: Output = Output + "<NAK>";
            break;
        case QSentoLanE::cbyteSTX: Output = Output + "<STX>";
            break;
        case QSentoLanE::cbyteETX: Output = Output + "<ETX>";
            break;
        case QSentoLanE::cbyteETB: Output = Output + "<ETB>";
            break;
        case QSentoLanE::cbyteEOT: Output = Output + "<EOT>";
            break;
        case QSentoLanE::cbyteWACK: Output = Output + "<WACK>";
            break;
        case QSentoLanE::cbyteRVI: Output = Output + "<RVI>";
            break;
        default:
            {
                buf.clear();
                buf = data.mid(i,1);
                Output = Output + fromCP1251(&buf);
                Output.replace(" ",".");
            }
            break;
        }
    }


    text->append(Output);
}

QString QSentoLanEDebugWidget::fromCP1251 (QByteArray *input)
{
    QString Output;

    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    Output = codec->toUnicode(*input);
    return Output;
}

QByteArray QSentoLanEDebugWidget::toCP1251 (QString *input)
{
    QByteArray Output;

    QByteArray buf(input->toStdString().c_str());
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    Output = codec->fromUnicode(buf);

    return Output;
}
