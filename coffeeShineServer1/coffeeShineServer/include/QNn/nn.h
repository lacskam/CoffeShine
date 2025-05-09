#ifndef NN_H
#define NN_H
#include"../QNnFileProcessing/QNnLocalProcessing.h"
#pragma push_macro("slots")
#undef slots
#include <torch/torch.h>
#pragma pop_macro("slots")
#include <QDir>

class SalesPredictionModel : public torch::nn::Module {

public:
    SalesPredictionModel();

    torch::Tensor forward(torch::Tensor id_tensor, torch::Tensor days_tensor, torch::Tensor months_tensor,
                          torch::Tensor temp_tensor,torch::Tensor hum_tensor,torch::Tensor os_tensor,torch::Tensor wind_tensor);


    friend bool learn(int *prod);

    friend bool retrain(int *prod);
    friend QMap<QDateTime,float> prediction(QList<QDate> *endDate,qint32 *pickedprod,QMap<QString,QString> &config);

private:
    torch::nn::LSTM lstm1 = nullptr;
    torch::nn::LSTM lstm2 = nullptr;

    torch::nn::Linear linear1 = nullptr;
    torch::nn::Linear linear2 = nullptr;


};


bool learn(int *prod);
bool retrain(int* prod);


QMap<QDateTime,float> prediction(QList<QDate> *endDate,qint32 *pickedprod,QMap<QString,QString> &config);


std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> normalize_data(
    const std::vector<std::tuple<int, int, int, float>>& data);



#endif // NN_H
