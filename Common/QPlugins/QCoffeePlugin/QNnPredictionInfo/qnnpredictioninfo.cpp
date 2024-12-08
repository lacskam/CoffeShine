#include "qnnpredictioninfo.h"

void Command18Worker::process() {
    QDataStream streamIn(&data, QIODevice::ReadOnly);
    QMap<qint32, float> result;
    QMap<QDateTime, float> predictionResults;

    QDate startDate;
    QDate endDate;
    qint32 idstart;
    qint32 idend;

    streamIn >> idstart;
    streamIn >> startDate;
    streamIn >> endDate;
    streamIn >> idend;

    QList<QDate> date = {startDate, endDate};
    float srAr;
    for (int i = idstart; i <= idend; i++) {
        predictionResults = prediction(&date, &i);
        if (predictionResults.isEmpty()) {
            continue;
        }
        srAr = 0;
        for (auto it = predictionResults.begin(); it != predictionResults.end(); ++it) {
            srAr += it.value();
        }
        srAr /= predictionResults.size();
        result.insert(i, srAr);
    }

    QByteArray Output;
    QDataStream streamOut(&Output, QIODevice::WriteOnly);
    streamOut << result;

    emit finished(Output);
}

Command18Worker::Command18Worker(QByteArray data)  {
    this->data=data;
}














