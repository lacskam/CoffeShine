#include "qnnsettingswidget.h"

void QNnSettingsWidget::updateVersionsCb(QList<QString> &versions,QMap<QString,QString> &config,qint32 &progressLearn) {

    firstConfig = config;
    confComboBox->clear();
    confComboBox->addItems(versions);

    try {
         confComboBox->setCurrentIndex(confComboBox->findText(config.value("currentModel")));
    } catch(...) {
        qDebug()<<"currentModel не найдена";
    }


    try {
        regComboBox->setCurrentIndex(regComboBox->findText(config.value("learnRegularity")));
    } catch(...) {
        qDebug()<<"learnRegularity не найдена";
    }


    try {
        QTime time = QTime::fromString(config.value("timeRegBegin"), "hh:mm");
        timeEditBegin->setTime(time);
    } catch(...) {
        qDebug()<<"timeRegBegin не найдена";
    }


    try {
        QTime time = QTime::fromString(config.value("timeRegEnd"), "hh:mm");
        timeEditEnd->setTime(time);
    } catch(...) {
        qDebug()<<"timeRegEnd не найдена";
    }

    if (progressLearn==-1) {

    } else {
        statusNn->setText("Статус: Идет обучение...");
        btnLearn->setEnabled(false);
        progressBarLearn->show();
        progressBarLearn->setValue(progressLearn * 10);

    }



}

QNnSettingsWidget::QNnSettingsWidget(QCoffeeClientPlugin *plugin, QWidget *parent)  {

    currentPlugin = plugin;

    connect(currentPlugin,&QCoffeeClientPlugin::signalSendStatus,this,[=](qint32 &status) {

         qDebug()<<"status"<<status;
        if (status==-1) {
            statusNn->setText("Статус: Обучение не требуется");
            btnLearn->setEnabled(true);
        } else if (status==-2) {
            statusNn->setText("Статус: Идет обучение...");
              progressBarLearn->show();
        } else if (status==-3) {
            statusNn->setText("Статус: Обученно");
            progressBarLearn->hide();
            progressBarLearn->setValue(0);
             btnLearn->setEnabled(true);

        } else {
            statusNn->setText("Статус: Идет обучение...");




            int value = progressBarLearn->value();
            if (value < 100) {
                progressBarLearn->setValue(value + 10);
            } else {
                progressBarLearn->setValue(100);

            }

        }




    });
    connect(currentPlugin,&QCoffeeClientPlugin::signalNnVersiomsGetted,this,&QNnSettingsWidget::updateVersionsCb);
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
     connect(btnLearn,&QPushButton::clicked,this,&QNnSettingsWidget::startLearn);
     QString stasus = "Обучено";
    statusNn = new QLabel("Статус: "+stasus);

    progressBarLearn = new QProgressBar();
     progressBarLearn->setRange(0,100);
    progressBarLearn->setValue(0);
    vblGroupBox->addWidget(groupStartLearn);
    vblStartLearn->insertWidget(0,statusNn);
     vblStartLearn->insertWidget(1,progressBarLearn);
    progressBarLearn->hide();
    vblStartLearn->insertWidget(2,btnLearn);


    buttonslayout = new QHBoxLayout;
    btnSave = new QPushButton("Сохранить");
    connect(btnSave,&QPushButton::clicked,this,&QNnSettingsWidget::sendNnSettings);

    btnCancel = new QPushButton("Отмена");
    connect(btnCancel,&QPushButton::clicked,this,&QNnSettingsWidget::close);

    buttonslayout->addWidget(btnSave);
    buttonslayout->addWidget(btnCancel);

    mainLayout->addLayout(buttonslayout);

    setMinimumSize(300,400);
    currentPlugin->getListNnVersions();

}

bool QNnSettingsWidget::chekChanges() {

    if (firstConfig["currentModel"]!=confComboBox->currentText()) {
        return true;
    } else if (firstConfig["learnRegularity"]!=regComboBox->currentText()) {
        return true;
    } else if (firstConfig["timeRegBegin"]!=timeEditBegin->time().toString("hh:mm")) {
        return true;
    }  else if (firstConfig["timeRegEnd"]!=timeEditEnd->time().toString("hh:mm")) {
    return true;
    }

    return false;

}
void QNnSettingsWidget::sendNnSettings() {


    QMap<QString,QString> config;

    if (chekChanges()) {
        config["currentModel"] = confComboBox->currentText();
        config["learnRegularity"] = regComboBox->currentText();
        config["timeRegBegin"] = timeEditBegin->time().toString("hh:mm");
        config["timeRegEnd"] = timeEditEnd->time().toString("hh:mm");

        currentPlugin->sendNnConfig(config);
    }


    this->close();

}


void QNnSettingsWidget::startLearn() {
    btnLearn->setEnabled(false);
     statusNn->setText("Статус: Начало обучения...");
    currentPlugin->sendRelearnRequest();
}
