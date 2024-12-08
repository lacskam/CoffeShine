#ifndef QDELETEDIALOG_H
#define QDELETEDIALOG_H

#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

class QDeleteDialog : public QDialog
{
    Q_OBJECT

    QDialogButtonBox *btnBox;
    QVBoxLayout *vd;
    QHBoxLayout *hd;

    QPushButton *ac;

    QPushButton *de;
    qint32 id;


public:
    explicit QDeleteDialog();
    void setIdDrink(qint32 id);
    qint32 getIdDrink();






};
#endif // QDELETEDIALOG_H
