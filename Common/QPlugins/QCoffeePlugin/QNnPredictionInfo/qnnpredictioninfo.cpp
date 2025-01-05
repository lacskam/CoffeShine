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




void Command17Worker::process() {
    QDataStream streamIn(&data, QIODevice::ReadOnly);

    qint32 id;
    qint32 idWg;
    QDate startDate, endDate;


    streamIn >> id;
    streamIn >> startDate;
    streamIn >> endDate;
    streamIn >> idWg;
    QList<QDate> date = {startDate, endDate};


    QMap<QDateTime, float> predictionResults = prediction(&date, &id);


    QByteArray Output;
    QDataStream streamOut(&Output, QIODevice::WriteOnly);

    streamOut << predictionResults;
    streamOut << idWg;
    streamOut << id;

    emit finished(Output);
}

Command17Worker::Command17Worker(QByteArray data)  {
    this->data=data;
}






void Command22Worker::process() {

    int i;
    for (i=0;i<10;i++) {
        if (retrain(&i)) {

        }
    }


    emit finished();
}


Command22Worker::Command22Worker(QByteArray data)  {
    this->data=data;

}








