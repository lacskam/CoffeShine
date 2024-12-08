#ifndef QSENTOLANEDEBUGWIDGET_H
#define QSENTOLANEDEBUGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QFont>
#include <QTextEdit>
#include <QSettings>
#include <QResizeEvent>
#include "../QSentoLanE/qsentolane.h"


class QSentoLanEDebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSentoLanEDebugWidget(QSentoLanE *device,QWidget *parent = nullptr);
    explicit QSentoLanEDebugWidget(QSentoLanE *device,QSettings *settings,QWidget *parent = nullptr);
    ~QSentoLanEDebugWidget();

private:
    void resizeEvent(QResizeEvent *event);

    QSettings *currentSettings;
    QVBoxLayout *mainLayout;
    QTextEdit *text;

    void loadSettings();
    void saveSettings();

    QString fromCP1251 (QByteArray *input);
    QByteArray toCP1251 (QString *input);

signals:

public slots:

private slots:
    void slotTxData (QByteArray data);
    void slotRxData (QByteArray data);
};

#endif // QSENTOLANEDEBUGWIDGET_H
