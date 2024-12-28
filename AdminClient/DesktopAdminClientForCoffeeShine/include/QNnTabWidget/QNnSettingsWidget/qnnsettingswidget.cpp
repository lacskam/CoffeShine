#include "qnnsettingswidget.h"



QNnSettingsWidget::QNnSettingsWidget(QWidget *parent)  {
    mainLayout = new QVBoxLayout(this);

    mainScrol = new QScrollArea(this);
     mainGroupBox = new QGroupBox();
    mainGroupBox->setTitle("Параметры модели");
    vblGroupBox = new QVBoxLayout(mainGroupBox);



    mainLayout->addWidget(mainGroupBox);


    groupBoxConf = new QGroupBox();
    vblConf = new QVBoxLayout(groupBoxConf);
    groupBoxConf->setTitle("Конфигурация модели");
    confComboBox = new QComboBox;

    vblConf->addWidget(confComboBox);

    vblGroupBox->addWidget(groupBoxConf);



    groupAutoLearn = new QGroupBox();
    vblAutoLearn = new QVBoxLayout(groupAutoLearn);
    groupAutoLearn->setTitle("Автообучение");
    vblGroupBox->addWidget(groupAutoLearn);


    hblAotuLearnReg = new QHBoxLayout;
    regTitle = new QLabel("Регулярность: ");
    regComboBox = new QComboBox;

    regComboBox->addItems({"Нет","Ежедневно","Еженедельно","Ежемесячно"});

    hblAotuLearnReg->addWidget(regTitle);
    hblAotuLearnReg->addWidget(regComboBox,Qt::AlignLeft);

    vblAutoLearn->addLayout(hblAotuLearnReg);

    hblAotuLearnTime = new QHBoxLayout;
    timeBeginLabel = new QLabel("От: ");
    timeEditBegin = new QTimeEdit;
    timeEndLabel = new QLabel("До: ");
    timeEditEnd = new QTimeEdit;

    hblAotuLearnTime->addWidget(timeBeginLabel);
    hblAotuLearnTime->addWidget(timeEditBegin,Qt::AlignLeft);
    hblAotuLearnTime->addWidget(timeEndLabel);
    hblAotuLearnTime->addWidget(timeEditEnd,Qt::AlignLeft);

    vblAutoLearn->addLayout(hblAotuLearnTime);


    groupStartLearn = new QGroupBox();
     vblStartLearn = new QVBoxLayout(groupStartLearn);
    btnLearn = new QPushButton("Запустить обучение");

    vblGroupBox->addWidget(groupStartLearn);

    vblStartLearn->addWidget(btnLearn);


    buttonslayout = new QHBoxLayout;
    btnSave = new QPushButton("Сохранить");
    btnCancel = new QPushButton("Отмена");

    buttonslayout->addWidget(btnSave);
    buttonslayout->addWidget(btnCancel);

    mainLayout->addLayout(buttonslayout);

    setMinimumSize(300,400);

}
