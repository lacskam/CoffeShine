#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QSettings>

#include "include/qserverloader.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QSettings *settings;
    QServerLoader *loader;

    void createForm();
    QHBoxLayout *mainLayout;
    QVBoxLayout *layoutSettings;
    QGroupBox *groupSettings;
    QGridLayout *layoutGroupSettings;
    QLabel *labelHostDB;
    QLineEdit *editHostDB;
    QLabel *labelUserDB;
    QLineEdit *editUserDB;
    QLabel *labelPasswordDB;
    QLineEdit *editPasswordDB;
    QLabel *labelPortDB;
    QLineEdit *editPortDB;
    QLabel *labelMainDB;
    QLineEdit *editMainDB;
    QLabel *labelUnDB;
    QLineEdit *editUnDB;
    QLabel *labelPort;
    QLineEdit *editPort;
    QHBoxLayout *layoutButtons;
    QPushButton *buttonStart;
    QPushButton *buttonsStop;

    QVBoxLayout *layoutLog;
    QLabel *labelLog;
    QTextEdit *editLog;

private slots:
    void slotPushStart();
    void slotPushStop();

    void slotServerStarted();
    void slotServerStoped();

    void slotDebugMessage(QString message);
};

#endif // WIDGET_H
