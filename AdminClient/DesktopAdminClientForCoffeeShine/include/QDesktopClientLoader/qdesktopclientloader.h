#ifndef QDESKTOPCLIENTLOADER_H
#define QDESKTOPCLIENTLOADER_H

#include <QObject>
#include <QSplashScreen>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>

#include "../QHostsDialog/qhostsdialog.h"
#include "../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.h"
#include "../QUserAccDialog/quseraccdialog.h"
#include "../../widget.h"

class QDesktopClientLoader : public QObject
{
    Q_OBJECT
public:
    explicit QDesktopClientLoader(QObject *parent = nullptr);

private:
    QSplashScreen *splashScreen;
    QSettings *settings;
    QCoffeeClientPlugin *currentPlugin;
    QUnNetProtokolCLient *client;

    QString currentLogin;
    QString currentPassword;

    bool saveLogin = false;

    void loadSettings();
    void saveSettings();

    bool requestLogin();
    bool requestServer();

    void setSplashMessage(QString);
    void showMainWindow();
    void showSplashScreen();
    void hideSplashScreen();
    void close();


signals:

public slots:
    void slotMessageSplashScreen(QString);
private slots:
    void slotBeginSynchronization();
    void slotEndSynchronization();


    void slotConnected();
    void slotAuthorized();
    void slotDisconnected();
    void slotUserInfoReceived();
    void slotError (UnErr err);
};

#endif // QDESKTOPCLIENTLOADER_H
