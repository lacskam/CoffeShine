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



class QNnSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNnSettingsWidget(QWidget *parent=nullptr);


private:
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









};
#endif // QNNSETTINGSWIDGET_H
