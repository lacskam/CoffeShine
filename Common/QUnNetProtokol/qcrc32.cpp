#include "qcrc32.h"

QCrc32::QCrc32(QObject *parent) : QObject(parent)
{
    quint32 crc;

    // initialize CRC table
    for (int i = 0; i < 256; i++)
    {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

        crc_table[i] = crc;
    }
}

void QCrc32::pushData(QByteArray data)
{
    for (int i=0;i<data.length();i++)
        resultCRC32 = crc_table[(resultCRC32 ^ data.at(i)) & 0xFF] ^ (resultCRC32 >> 8);
}

quint32 QCrc32::getResult()
{
    return resultCRC32;
}

void QCrc32::reset()
{
    resultCRC32 = 0xFFFFFFFFUL;
}
