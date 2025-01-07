#ifndef QNNSETTINGSWIDGET_H
#define QNNSETTINGSWIDGET_H

#include <QPushButton>
#include <QTimeEdit>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include "../../../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include <QProgressBar>

class QNnSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNnSettingsWidget(QCoffeeClientPlugin *plugin,QWidget *parent=nullptr);


private:
    QCoffeeClientPlugin *currentPlugin;
    QMap<QString,QString> firstConfig;
    QVBoxLayout *mainLayout;
    QGroupBox *mainGroupBox;
    QVBoxLayout *vblGroupBox;

    QScrollArea *mainScrol;


    QVBoxLayout *vblConf;
    QGroupBox *groupBoxConf;
    QComboBox *confComboBox;

    QGroupBox *groupAutoLearn;
    QVBoxLayout *vblAutoLearn;

    QHBoxLayout *hblAotuLearnReg;
    QLabel *regTitle;
    QComboBox *regComboBox;
    QHBoxLayout *hblAotuLearnTime;
    QLabel *timeBeginLabel;
    QTimeEdit *timeEditBegin;
    QLabel *timeEndLabel;
    QTimeEdit *timeEditEnd;

    QVBoxLayout *vblStartLearn;
    QGroupBox *groupStartLearn;
    QPushButton *btnLearn;

    QLabel *statusNn;
    QProgressBar *progressBarLearn;

     QHBoxLayout *buttonslayout;
    QPushButton *btnSave;
    QPushButton *btnCancel;

    bool chekChanges();



private slots:
    void updateVersionsCb(QList<QString> &versions, QMap<QString, QString> &config,qint32 &progressLearn);
    void sendNnSettings();
    void startLearn();


};
#endif // QNNSETTINGSWIDGET_H
