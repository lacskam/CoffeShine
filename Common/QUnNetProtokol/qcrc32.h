#ifndef QCRC32_H
#define QCRC32_H

#include <QObject>

class QCrc32 : public QObject
{
    Q_OBJECT
public:
    explicit QCrc32(QObject *parent = nullptr);
    void pushData(QByteArray data);
    quint32 getResult();
    void reset();

private:
    quint32 crc_table[256];
    quint32 resultCRC32;

signals:

public slots:
};

#endif // QCRC32_H
