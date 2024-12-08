#ifndef QPRICTUREDIALOG_H
#define QPRICTUREDIALOG_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPixmap>
#include <QDialog>
#include <QBuffer>
#include <QMessageBox>
#include "../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h"
#include "qpicturewidgetitem.h"

class QPictureDialog : public QDialog
{
    Q_OBJECT
public:
    static QCoffeePictureInfo getPicture(QVector<QCoffeePictureInfo>*, bool *ok=nullptr, QWidget *parent=nullptr);

    QCoffeePictureInfo getSelectedPicture();
    QVector<QCoffeePictureInfo> getSelectedPictures();
private:
    QPictureDialog(QVector<QCoffeePictureInfo>*, QWidget *parent=nullptr);

    QVector<QCoffeePictureInfo>* currentPictureList;
    QVector<QCoffeePictureInfo> selectedPictures;
    QCoffeePictureInfo selectedPicture;


    void createForm();
    void fillingForm();

    QListWidget * listWidget;
    QListWidgetItem* listWidgetItem;
    QPictureWidgetItem *pictureWidgetItem;


    QHBoxLayout *layoutButtons;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

signals:
    void signalUpdateItemsBackgroudColor(int);

public slots:
private slots:
    void slotPushOk();
    void slotPushCancel();
};

#endif // QPRICTUREDIALOG_H
