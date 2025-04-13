#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    settings = new QSettings("Homer","CoffeeShineServer");

    loader = new QServerLoader(settings,this);
    connect(loader,SIGNAL(signalServerStarted()),this,SLOT(slotServerStarted()));
    connect(loader,SIGNAL(signalServerStoped()),this,SLOT(slotServerStoped()));
    connect(loader,SIGNAL(signalDebugMessage(QString)),this,SLOT(slotDebugMessage(QString)));



    createForm();
}

Widget::~Widget()
{

}

void Widget::createForm()
{
    mainLayout = new QHBoxLayout(this);
    layoutSettings = new QVBoxLayout;
    layoutSettings->setMargin(0);
    mainLayout->addLayout(layoutSettings);

    groupSettings = new QGroupBox(tr("Settings"),this);
    layoutSettings->addWidget(groupSettings);

    layoutGroupSettings = new QGridLayout(groupSettings);
    labelHostDB = new QLabel(tr("Host DB"),groupSettings);
    layoutGroupSettings->addWidget(labelHostDB,0,0,1,1);
    editHostDB = new QLineEdit(groupSettings);
    editHostDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editHostDB->setText("127.0.0.1");
    layoutGroupSettings->addWidget(editHostDB,0,1,1,1);

    labelPortDB = new QLabel(tr("Port DB"),groupSettings);
    layoutGroupSettings->addWidget(labelPortDB,1,0,1,1);
    editPortDB = new QLineEdit(groupSettings);
    editPortDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editPortDB->setText("3306");
    layoutGroupSettings->addWidget(editPortDB,1,1,1,1);

    labelMainDB = new QLabel(tr("Main DB"),groupSettings);
    layoutGroupSettings->addWidget(labelMainDB,2,0,1,1);
    editMainDB = new QLineEdit(groupSettings);
    editMainDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editMainDB->setText("db_coffeeShine");
    layoutGroupSettings->addWidget(editMainDB,2,1,1,1);

    labelUnDB = new QLabel(tr("Users DB"),groupSettings);
    layoutGroupSettings->addWidget(labelUnDB,3,0,1,1);
    editUnDB = new QLineEdit(groupSettings);
    editUnDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editUnDB->setText("bd_users");
    layoutGroupSettings->addWidget(editUnDB,3,1,1,1);

    labelUserDB = new QLabel(tr("User DB"),groupSettings);
    layoutGroupSettings->addWidget(labelUserDB,4,0,1,1);
    editUserDB = new QLineEdit(groupSettings);
    editUserDB->setText("root");
    editUserDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    layoutGroupSettings->addWidget(editUserDB,4,1,1,1);

    labelPasswordDB = new QLabel(tr("Password DB"),groupSettings);
    layoutGroupSettings->addWidget(labelPasswordDB,5,0,1,1);
    editPasswordDB = new QLineEdit(groupSettings);
    editPasswordDB->setEchoMode(QLineEdit::Password);
    editPasswordDB->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editPasswordDB->setText("1756");
    layoutGroupSettings->addWidget(editPasswordDB,5,1,1,1);

    labelPort = new QLabel(tr("Server port"),groupSettings);
    layoutGroupSettings->addWidget(labelPort,6,0,1,1);
    editPort = new QLineEdit(groupSettings);
    editPort->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    editPort->setText("7777");
    layoutGroupSettings->addWidget(editPort,6,1,1,1);

    layoutButtons = new QHBoxLayout();
    layoutGroupSettings->addLayout(layoutButtons,10,0,1,2);

    buttonStart = new QPushButton(tr("Start"),groupSettings);
    connect(buttonStart,SIGNAL(pressed()),this,SLOT(slotPushStart()));
    layoutButtons->addWidget(buttonStart);
    buttonsStop = new QPushButton(tr("Stop"),groupSettings);
    buttonsStop->setEnabled(false);
    connect(buttonsStop,SIGNAL(pressed()),this,SLOT(slotPushStop()));
    layoutButtons->addWidget(buttonsStop);

    layoutSettings->addStretch();

    layoutLog = new QVBoxLayout;
    layoutLog->setMargin(0);
    mainLayout->addLayout(layoutLog);
    labelLog = new QLabel(tr("Log"),this);
    layoutLog->addWidget(labelLog);
    editLog = new QTextEdit(this);
    editLog->setReadOnly(true);
    layoutLog->addWidget(editLog);
}

void Widget::slotPushStart()
{
    buttonStart->setEnabled(false);
    editHostDB->setEnabled(false);
    editMainDB->setEnabled(false);
    editPort->setEnabled(false);
    editPortDB->setEnabled(false);
    editUnDB->setEnabled(false);
    editUserDB->setEnabled(false);
    editPasswordDB->setEnabled(false);


    loader->setServerPort(editPort->text().toInt());

    QUnDataBaseSettings A;
    A.dataBaseName = editUnDB->text();
    A.host = editHostDB->text();
    A.login = editUserDB->text();
    A.password = editPasswordDB->text();
    A.port = editPortDB->text().toInt();
    A.type = "QMYSQL";
    loader->setUnDBSettings(A);

    loader->startServer();
}

void Widget::slotPushStop()
{
    buttonsStop->setEnabled(false);
    loader->stopServer();
}

void Widget::slotServerStarted()
{
    editLog->insertPlainText(tr("Server started\r\n"));
    buttonsStop->setEnabled(true);
}

void Widget::slotServerStoped()
{
    editLog->insertPlainText(tr("Server stoped\r\n"));

    buttonStart->setEnabled(true);
    editHostDB->setEnabled(true);
    editMainDB->setEnabled(true);
    editPort->setEnabled(true);
    editPortDB->setEnabled(true);
    editUnDB->setEnabled(true);
    editUserDB->setEnabled(true);
    editPasswordDB->setEnabled(true);
}

void Widget::slotDebugMessage(QString message)
{
    editLog->insertPlainText(message+"\r\n");
}
