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


class QNnSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNnSettingsWidget(QCoffeeClientPlugin *plugin,QWidget *parent=nullptr);


private:
    QCoffeeClientPlugin *currentPlugin;
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


     QHBoxLayout *buttonslayout;
    QPushButton *btnSave;
    QPushButton *btnCancel;





private slots:
    void updateVersionsCb(QList<QString> versions);



};
#endif // QNNSETTINGSWIDGET_H
