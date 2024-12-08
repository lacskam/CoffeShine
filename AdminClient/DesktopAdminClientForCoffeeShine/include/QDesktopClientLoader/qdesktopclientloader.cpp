#include "qdesktopclientloader.h"

QDesktopClientLoader::QDesktopClientLoader(QObject *parent) : QObject(parent)
{
    bool y = false;
    showSplashScreen();
    setSplashMessage(tr("Запуск программы"));

    settings = new QSettings("Homer","DesktopAdminClient");
    loadSettings();

    client = new QUnNetProtokolCLient("CoffeeShine",settings,this);

    connect(client,SIGNAL(signalConnected()),this,SLOT(slotConnected()));
    connect(client,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()));
    connect(client,SIGNAL(signalAuthorized()),this,SLOT(slotAuthorized()));
    connect(client,SIGNAL(signalUserInfoReceived()),this,SLOT(slotUserInfoReceived()));
    connect(client,SIGNAL(signalError(UnErr)),this,SLOT(slotError(UnErr)));

    setSplashMessage(tr("Проверка настроек сервера..."));
    y = requestServer();
    if (client->countHosts()==0) //Проверяем есть ли хосты
    {
        setSplashMessage(tr("Получение настроек сервера..."));
        y = requestServer();
    } else
    {
        setSplashMessage(tr("Получение настроек сервера...OK"));
        y = true;
    }


    //Выдаем апрос на авторизацию
    setSplashMessage(tr("Введите логин и пароль..."));
    y = requestLogin();

    if (y) //Производим подключение
    {
        setSplashMessage(tr("Подключение к серверу..."));
        //qDebug()<<"Socket connect="<<
        client->connectToHost();
    }

    if (y)
    {
        QThread::sleep(3);
    }

 //   Widget *w = new Widget();
  //  w->show();
}

void QDesktopClientLoader::setSplashMessage (QString message)
{
    if (splashScreen !=NULL)
    splashScreen->showMessage(message,Qt::AlignBottom | Qt::AlignCenter,Qt::black);
}

void QDesktopClientLoader::showMainWindow()
{
    Widget *w = new Widget(settings,client,currentPlugin);
    w->show();
}

void QDesktopClientLoader::showSplashScreen()
{
    splashScreen = new QSplashScreen(QPixmap(":/images/CoffeeSplashScreen.png"));
    splashScreen->setWindowFlag(Qt::WindowStaysOnTopHint);

    QFont font;
    font.setPixelSize(24);
    splashScreen->setFont(font);

    splashScreen->show();
}

void QDesktopClientLoader::hideSplashScreen()
{
    delete splashScreen;
    splashScreen = NULL;
}

void QDesktopClientLoader::slotMessageSplashScreen(QString message)
{
    setSplashMessage(message);
}


void QDesktopClientLoader::loadSettings()
{
    saveLogin = settings->value("/QDesktopClientLoader/saveLogin").toBool();
    currentLogin = settings->value("/QDesktopClientLoader/currentLogin").toString();
    currentPassword = settings->value("/QDesktopClientLoader/currentPassword").toString();
}

void QDesktopClientLoader::saveSettings()
{
    settings->setValue("/QDesktopClientLoader/saveLogin",saveLogin);
    if (saveLogin)
    {
        settings->setValue("/QDesktopClientLoader/currentLogin",currentLogin);
        //settings->setValue("/QDesktopClientLoader/currentPassword",currentPassword);
    }
        else
    {
        settings->setValue("/QDesktopClientLoader/currentPassword","");
        settings->setValue("/QDesktopClientLoader/currentLogin","");
    }

}

void QDesktopClientLoader::slotConnected()
{
    qDebug()<<"QDesktopClientLoader::slotConnected";
}

void QDesktopClientLoader::slotAuthorized()
{
    qDebug()<<"QDesktopClientLoader::slotAuthorized";
}

void QDesktopClientLoader::slotDisconnected()
{
    //qDebug()<<"slotDisconnected";
    qDebug()<<"QDesktopClientLoader::slotDisconnected";
}

void QDesktopClientLoader::slotUserInfoReceived()
{
    currentPlugin = new QCoffeeClientPlugin(client,0x06,this);

    connect(currentPlugin,SIGNAL(signalBeginSynchronization()),
            this,SLOT(slotBeginSynchronization()));
    connect(currentPlugin,SIGNAL(signalMessageSplashScreen(QString)),
            this,SLOT(slotMessageSplashScreen(QString)));
    connect(currentPlugin,SIGNAL(signalEndSynchronization()),
            this,SLOT(slotEndSynchronization()));

    currentPlugin->startSynchronization();

}

void QDesktopClientLoader::slotBeginSynchronization()
{
    qDebug()<<"начало синхронизации";
}

void QDesktopClientLoader::slotEndSynchronization()
{
    qDebug()<<"окончание синхронизации";
    hideSplashScreen();
    showMainWindow();
}

void QDesktopClientLoader::slotError (UnErr err)
{
    //qDebug()<<"slotSocketError"<<err;
    switch (err) {
    case UnErr::errAuthBlock:
    {
        QMessageBox::critical(splashScreen,tr("Error"),tr("The account is blocked"));
        bool y = requestLogin();
        if (y) client->connectToHost();
    }
        break;
    case UnErr::errAuthLogin:
    {
        QMessageBox::critical(splashScreen,tr("Error"),tr("The login incorrect"));
        bool y = requestLogin();
        if (y) client->connectToHost();
    }
        break;
    case UnErr::errAuthPassword:
    {
        QMessageBox::critical(splashScreen,tr("Error"),tr("The password incorrect"));
        bool y = requestLogin();
        if (y) client->connectToHost();
    }
        break;
    case UnErr::errAuthServWork:
    {
        QMessageBox::critical(splashScreen,tr("Error"),tr("Technical works on the server"));
        close();
    }
        break;

    }
}

bool QDesktopClientLoader::requestServer()
{
    bool Output;

    bool ok=false;
    HostInfo newHost = QHostsDialog::getHost(&ok);
    QVector<HostInfo> A;
    A.push_back(newHost);
    if (ok)
    {
        client->setHosts(A);
        Output=true;
        setSplashMessage(tr("Get server settings...OK"));
    } else
    {
   //     setSplashMessage(tr("Get server settings...ERROR"));
   //     QMessageBox::critical(splashScreen,tr("Error"),tr("No server settings"));
    //    close();
        Output = false;
    }

    return Output;
}

bool QDesktopClientLoader::requestLogin()
{
    bool Output;
    bool ok;
    UserAccount acc = QUserAccDialog::getUserAcc(currentLogin,&ok,splashScreen);
    if (ok)
    {
        currentLogin = acc.login;
        currentPassword = acc.password;
        saveLogin = acc.saveLogin;

        saveSettings();
        client->setAccount(currentLogin,currentPassword);
        Output = true;
        setSplashMessage(tr("Get login and password...OK"));
    } else
    {
        setSplashMessage(tr("Get login and password...ERROR"));
        QMessageBox::critical(splashScreen,tr("Error"),tr("No account"));
        close();
        Output = false;
    }
    return Output;
}

void QDesktopClientLoader::close()
{
    QTimer::singleShot(1,qApp,SLOT(quit()));
}
