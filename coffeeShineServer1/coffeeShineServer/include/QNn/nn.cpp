#include "nn.h"
//#include "plot.h"
#include "../QNnFileProcessing/QWeather/weather.h"

const int input_size = 6;
const int output_size = 1;
const int hidden_size = 256;
const int num_epochs = 1000;
const double learning_rate = 0.001;


std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor,torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> normalize_data(
    const std::vector<std::tuple<float,float,float,float,float,float,float,float>>& data) {
    std::vector<float> ids, days, months, temp, hum, os, wind;
    std::vector<float> sales;

    std::transform(data.begin(), data.end(), std::back_inserter(ids), [](const auto& tup) { return std::get<0>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(days), [](const auto& tup) { return std::get<1>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(months), [](const auto& tup) { return std::get<2>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(sales), [](const auto& tup) { return std::get<3>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(temp), [](const auto& tup) { return std::get<4>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(hum), [](const auto& tup) { return std::get<5>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(os), [](const auto& tup) { return std::get<6>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(wind), [](const auto& tup) { return std::get<7>(tup); });

    std::cout << "ids_tensor: " << data.size() << std::endl;


    torch::Tensor ids_tensor = torch::tensor(ids, torch::kFloat32);
    torch::Tensor days_tensor = torch::tensor(days, torch::kFloat32);
    torch::Tensor months_tensor = torch::tensor(months, torch::kFloat32);
    torch::Tensor sales_tensor = torch::tensor(sales, torch::kFloat32);
    torch::Tensor temp_tensor = torch::tensor(temp, torch::kFloat32);
    torch::Tensor hum_tensor = torch::tensor(hum, torch::kFloat32);
    torch::Tensor os_tensor = torch::tensor(os, torch::kFloat32);
    torch::Tensor wind_tensor = torch::tensor(wind, torch::kFloat32);
    std::cout << "Sizes before normalization:" << std::endl;

        std::cout << "ids_tensor: " << ids_tensor.sizes() << std::endl;
        std::cout << "days_tensor: " << days_tensor.sizes() << std::endl;
        std::cout << "months_tensor: " << months_tensor.sizes() << std::endl;
        std::cout << "sales_tensor: " << sales_tensor.sizes() << std::endl;
        std::cout << "temp_tensor: " << temp_tensor.sizes() << std::endl;
        std::cout << "hum_tensor: " << hum_tensor.sizes() << std::endl;
        std::cout << "os_tensor: " << os_tensor.sizes() << std::endl;
        std::cout << "wind_tensor: " << wind_tensor.sizes() << std::endl;

    auto z_normalize = [](torch::Tensor& tensor) {
        auto mean = tensor.mean().item<float>()+ 1e-8;
        auto std = tensor.std().item<float>() + 1e-8;

        tensor = (tensor - mean) / std;

        std::cout << "Normalized values:" << std::endl;
        std::cout << tensor << std::endl;
    };

    z_normalize(days_tensor);
    z_normalize(months_tensor);
    z_normalize(sales_tensor);
    z_normalize(temp_tensor);
    z_normalize(hum_tensor);
    z_normalize(os_tensor);
    z_normalize(wind_tensor);

    return std::make_tuple(ids_tensor, days_tensor, months_tensor, sales_tensor,temp_tensor,hum_tensor,os_tensor,wind_tensor);
}

/*std::vector<std::vector<torch::Tensor>> normalize_dataFlex(
    const std::vector<std::vector<float>>& data) {

    if (data.empty()) {
        return 0;
    }

    std::vector<std::vector<float>> preTensors;


    for (int i =0;i<data.at(0).size();i++) {
        std::transform(data.begin(), data.end(), std::back_inserter(preTensors[i]), [](const auto& tup) { return std::get<i>(tup); });
    }



    std::cout << "ids_tensor: " << data.size() << std::endl;
    std::vector<torch::Tensor> tensors;
    for (int i=0;i<preTensors.size();i++) {
        tensors.push_back(torch::tensor(preTensors[i], torch::kFloat32));

        std::cout << "tensor "<<i<<":  " << tensors[0].sizes() << std::endl;
    }


    // Z-нормализация
    auto z_normalize = [](torch::Tensor& tensor) {
        auto mean = tensor.mean().item<float>()+ 1e-8;
        auto std = tensor.std().item<float>() + 1e-8;

        tensor = (tensor - mean) / std;

        std::cout << "Normalized values:" << std::endl;
        std::cout << tensor << std::endl;
    };


    for (int i=0;i<tensors.size();i++) {
        z_normalize(tensors[i]);
    }



    return tensors;
}*/

bool learn(int* prod) {
    SalesPredictionModel model;
    torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(learning_rate));
    torch::nn::MSELoss loss;
    //std::vector<std::tuple<int, int, int, float>> data;
    std::vector<std::tuple<float,float,float,float,float,float,float,float>> data1;

   // data = getfile(prod);
    data1 = getDataById(*prod);

   // auto [id, days, months, sales] = normalize_data(data);
    auto [id, days, months, sales, temp, hum, os, wind] = normalize_data(data1);
    auto dataset = torch::data::datasets::TensorDataset({ id, days, months, sales, temp, hum, os, wind });
    auto data_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
        std::move(dataset), torch::data::DataLoaderOptions().batch_size(64));
    float d;
    int epoch=0;
    do  {
        for (auto& batch : *data_loader) {
            auto id_batch = batch.data()[0];
            auto days_batch = batch.data()[1];
            auto months_batch = batch.data()[2];
            auto sales_batch1 = batch.data()[3];
            auto temp_batch = batch.data()[4];
            auto hum_batch = batch.data()[5];
            auto os_batch = batch.data()[6];
            auto wind_batch = batch.data()[7];
            auto sales_batch = torch::stack({ sales_batch1 }).unsqueeze(2);


            optimizer.zero_grad();

            auto predictions = model.forward(id_batch, days_batch, months_batch, temp_batch, hum_batch, os_batch, wind_batch);
            auto l = loss(predictions, sales_batch);
           if (std::isnan(l.item<float>())){
               return 1;
            }
            l.backward();
            optimizer.step();

            std::cout << "epoch: " << epoch << "  loss:" << l.item<float>() << std::endl;
            d=l.item<float>();


    }
         epoch++;
} while (d>0.0001);
    std::cout << "Training finished, save? (1,0)" << std::endl;
    bool h=1;
   // std::cin >> h;
    if (h == 1) {
        torch::serialize::OutputArchive archive;
        model.save(archive);
        QDir dirForModels;
        QString nameDir = "models_"+ QDateTime::currentDateTime().toString();
        dirForModels.exists(nameDir);
        QString path = nameDir+"/" + QString::number(*prod) + "model.pt";
        archive.save_to(path.toStdString());
    }

      return 0;
}


bool retrain(int* prod) {

    if (prod == nullptr) {
        qDebug() << "Ошибка: указатель на продукт nullptr.";
        return false;
    }

    SalesPredictionModel model;

    QString modelPath = "../../NnModels/models/" + QString::number(*prod) + "model.pt";
    if (!QFile::exists(modelPath)) {
        qDebug() << "Ошибка: файл модели не найден по пути:" << modelPath;
        return false;
    }

    try {
        qDebug() << "Загрузка модели из файла:" << modelPath;
        torch::serialize::InputArchive archive;
        archive.load_from(modelPath.toStdString());
        model.load(archive);
        model.train();
    } catch (const std::exception& e) {
        qDebug() << "Ошибка загрузки или инициализации модели:" << e.what();
        return false;
    }

    std::vector<std::tuple<float,float,float,float,float,float,float,float>> new_data;

    // data = getfile(prod);
    new_data = getDataById(*prod);

    torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(learning_rate));
    torch::nn::MSELoss loss;

    auto [id, days, months, sales, temp, hum, os, wind] = normalize_data(new_data);

    auto dataset = torch::data::datasets::TensorDataset({ id, days, months, sales, temp, hum, os, wind });
    auto data_loader = torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
        std::move(dataset), torch::data::DataLoaderOptions().batch_size(64));

    int epoch = 0;
    float d;
    do {
        for (auto& batch : *data_loader) {
            auto id_batch = batch.data()[0];
            auto days_batch = batch.data()[1];
            auto months_batch = batch.data()[2];
            auto sales_batch1 = batch.data()[3];
            auto temp_batch = batch.data()[4];
            auto hum_batch = batch.data()[5];
            auto os_batch = batch.data()[6];
            auto wind_batch = batch.data()[7];
            auto sales_batch = torch::stack({ sales_batch1 }).unsqueeze(2);

            optimizer.zero_grad();

            auto predictions = model.forward(id_batch, days_batch, months_batch, temp_batch, hum_batch, os_batch, wind_batch);
            auto l = loss(predictions, sales_batch);

            if (std::isnan(l.item<float>())) {
                qDebug() << " потери NaN, прерывание обучения.";
                return false;
            }

            l.backward();
            optimizer.step();

            std::cout << "epoch: " << epoch << "  loss:" << l.item<float>() << std::endl;
            d = l.item<float>();
        }
        epoch++;
    } while (d > 0.0001);

    try {
        QString currentDate = QDate::currentDate().toString("yyyy_MM_dd");
        QString newDir = "../../NnModels/models_" + currentDate;

        QDir dir;
        if (!dir.exists(newDir)) {
            if (!dir.mkpath(newDir)) {
                qDebug() << "не удалось создать директорию для сохранения модели:" << newDir;
                return false;
            }
        }

        QString newModelPath = newDir + "/" + QString::number(*prod) + "model.pt";

        qDebug() << "сохранение модели в директорию:" << newDir;
        torch::serialize::OutputArchive archive;
        model.save(archive);
        archive.save_to(newModelPath.toStdString());
    } catch (const std::exception& e) {
        qDebug() << "Ошибка при сохранении модели:" << e.what();
        return false;
    }

    return true;
}





float denormalize_value(float normalized_value, float mean, float std) {
    return (normalized_value * std) + mean;
}

std::tuple<torch::Tensor, torch::Tensor, torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor> normalize_input_data(
    torch::Tensor id_tensor, torch::Tensor days_tensor, torch::Tensor months_tensor,torch::Tensor temp_tensor, torch::Tensor hum_tensor, torch::Tensor os_tensor, torch::Tensor wind_tensor,
    float id_mean, float id_std, float days_mean, float days_std, float months_mean,float months_std, float temp_std,float temp_mean,
        float hum_std,float hum_mean,float os_std,float os_mean,float wind_std,float wind_mean) {

    auto normalized_id = id_tensor;
    auto normalized_days = (days_tensor - days_mean) / days_std;
    auto normalized_months = (months_tensor - months_mean) / months_std;
    auto normalized_temp = (temp_tensor - temp_mean) / temp_std;
    auto normalized_hum = (hum_tensor - hum_mean) / hum_std;
    auto normalized_os = (os_tensor - os_mean) / os_std;
    auto normalized_wind = (wind_tensor - wind_mean) / wind_std;


    std::cout << "Normalized input values:" << std::endl;
    std::cout << "Normalized ID: " << normalized_id << std::endl;
    std::cout << "Normalized Days: " << normalized_days << std::endl;
    std::cout << "Normalized Months: " << normalized_months << std::endl;
    std::cout << "Normalized temp: " << normalized_temp << std::endl;
    std::cout << "Normalized hum: " << normalized_hum << std::endl;
    std::cout << "Normalized os: " << normalized_os << std::endl;
    std::cout << "Normalized wind: " << normalized_wind << std::endl;

    return std::make_tuple(normalized_id, normalized_days, normalized_months,normalized_temp,normalized_hum,normalized_os,normalized_wind);
}

std::tuple<float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float> getStdAndMean(
    const std::vector<std::tuple<float,float,float,float,float,float,float,float>>& data);

QMap<QDateTime, float> prediction(QList<QDate>* endDate, qint32* pickedprod) {
    QMap<QDateTime, float> result;

    qDebug() << "Начало выполнения функции prediction";

    try {
        qDebug() << "Проверка входных данных";
        if (endDate == nullptr || pickedprod == nullptr) {
            qDebug() << "Ошибка: указатели на данные nullptr.";
            return result;
        }
        if (endDate->size() < 2) {
            qDebug() << "Ошибка: список дат должен содержать начальную и конечную дату.";
            return result;
        }

        qDebug() << "Инициализация модели";
        SalesPredictionModel model;


        QString path = "../../NnModels/models/" + QString::number(*pickedprod) + "model.pt";
        if (!QFile::exists(path)) {
            qDebug() << "Ошибка: файл модели не найден по пути:" << path;
            return result;
        }

        try {
            qDebug() << "Загрузка модели из файла:" << path;
            torch::serialize::InputArchive archive;
            archive.load_from(path.toStdString());
            model.load(archive);
            model.eval();
        } catch (const std::exception& e) {
            qDebug() << "Ошибка загрузки или инициализации модели:" << e.what();
            return result;
        }

        qDebug() << "Получение данных для продукта";
        std::vector<std::tuple<float, float, float, float, float, float, float, float>> data;
        try {
            data = getDataById(*pickedprod);
            if (data.empty()) {
                qDebug() << "Ошибка: данные для продукта пустые.";
                return result;
            }
        } catch (const std::exception& e) {
            qDebug() << "Ошибка получения данных файла:" << e.what();
            return result;
        }

        qDebug() << "Получение стандартных отклонений и средних значений для данных";
        float idmean, idstd, daysmean, daysstd, moumean, moustd, salesmean, salesstd, tempmean, tempstd, hummean, humstd, osmean, osstd, windmean, windstd;
        try {
            std::tie(idmean, idstd, daysmean, daysstd, moumean, moustd, salesmean, salesstd, tempmean, tempstd, hummean, humstd, osmean, osstd, windmean, windstd) = getStdAndMean(data);
        } catch (const std::exception& e) {
            qDebug() << "Ошибка получения стандартизации данных:" << e.what();
            return result;
        }

        qDebug() << "Инициализация цикла прогноза";
        QDate tempdate = endDate->at(0);
        int n = 1;

        while (tempdate <= endDate->at(1)) {
            try {
                qDebug() << "Прогнозирование для даты:" << tempdate.toString();

                qDebug() << "Получение погодных данных";
                Weather weather(&tempdate, 1);

                qDebug() << "Подготовка входных данных для модели";
                torch::Tensor test_id = torch::tensor({ *pickedprod }, torch::kFloat32);
                torch::Tensor test_day = torch::tensor({ tempdate.day() }, torch::kFloat32);
                torch::Tensor test_month = torch::tensor({ tempdate.month() }, torch::kFloat32);
                torch::Tensor test_temp = torch::tensor({ weather.get_temperature() }, torch::kFloat32);
                torch::Tensor test_hum = torch::tensor({ weather.get_humidity() }, torch::kFloat32);
                torch::Tensor test_os = torch::tensor({ weather.getOs() }, torch::kFloat32);
                torch::Tensor test_wind = torch::tensor({ weather.getWindSpeed() }, torch::kFloat32);

                qDebug() << "Нормализация входных данных";
                auto [normalized_id, normalized_days, normalized_months, normalized_temp, normalized_hum, normalized_os, normalized_wind] =
                    normalize_input_data(test_id, test_day, test_month, test_temp, test_hum, test_os, test_wind, idmean, idstd, daysmean, daysstd, moumean, moustd, tempmean, tempstd, hummean, humstd, osmean, osstd, windmean, windstd);

                qDebug() << "Запуск модели для прогнозирования";
                torch::Tensor predicted_output = model.forward(normalized_id, normalized_days, normalized_months, normalized_temp, normalized_hum, normalized_os, normalized_wind);

                qDebug() << "Денормализация выходных данных";
                float denormalized_output = denormalize_value(predicted_output.item<float>(), salesmean, salesstd);

                qDebug() << "Добавление результата прогноза в итоговую карту";
                result.insert(QDateTime(tempdate.startOfDay()), std::abs(denormalized_output));
            } catch (const std::exception& e) {
                qDebug() << "Ошибка во время прогнозирования для даты" << tempdate.toString() << ":" << e.what();
            }

            qDebug() << "Переход к следующей дате";
            tempdate = tempdate.addDays(1);
        }

        qDebug() << "Цикл прогноза завершён";
    } catch (const std::exception& e) {
        qDebug() << "Общая ошибка в функции prediction:" << e.what();
    } catch (...) {
        qDebug() << "Неизвестная ошибка в функции prediction";
    }

    qDebug() << "Завершение выполнения функции prediction";
    return result;
}

std::tuple<float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float> getStdAndMean(
    const std::vector<std::tuple<float,float,float,float,float,float,float,float>>& data) {
    std::vector<float> ids, days, months, temp, hum, os, wind;
    std::vector<float> sales;

    std::transform(data.begin(), data.end(), std::back_inserter(ids), [](const auto& tup) { return std::get<0>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(days), [](const auto& tup) { return std::get<1>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(months), [](const auto& tup) { return std::get<2>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(sales), [](const auto& tup) { return std::get<3>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(temp), [](const auto& tup) { return std::get<4>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(hum), [](const auto& tup) { return std::get<5>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(os), [](const auto& tup) { return std::get<6>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(wind), [](const auto& tup) { return std::get<7>(tup); });

    std::cout << "ids_tensor: " << data.size() << std::endl;


    torch::Tensor ids_tensor = torch::tensor(ids, torch::kFloat32);
    torch::Tensor days_tensor = torch::tensor(days, torch::kFloat32);
    torch::Tensor months_tensor = torch::tensor(months, torch::kFloat32);
    torch::Tensor sales_tensor = torch::tensor(sales, torch::kFloat32);
    torch::Tensor temp_tensor = torch::tensor(temp, torch::kFloat32);
    torch::Tensor hum_tensor = torch::tensor(hum, torch::kFloat32);
    torch::Tensor os_tensor = torch::tensor(os, torch::kFloat32);
    torch::Tensor wind_tensor = torch::tensor(wind, torch::kFloat32);

         float idmean = ids_tensor.mean().item<float>()+ 1e-8;
        float idstd = ids_tensor.std().item<float>() + 1e-8;

        float daysmean = days_tensor.mean().item<float>()+ 1e-8;
       float daysstd = days_tensor.std().item<float>() + 1e-8;

       float moumean = months_tensor.mean().item<float>()+ 1e-8;
      float moustd = months_tensor.std().item<float>() + 1e-8;

      float salesmean = sales_tensor.mean().item<float>()+ 1e-8;
     float salesstd = sales_tensor.std().item<float>() + 1e-8;

     float tempmean = temp_tensor.mean().item<float>()+ 1e-8;
    float tempstd = temp_tensor.std().item<float>() + 1e-8;

    float hummean = hum_tensor.mean().item<float>()+ 1e-8;
   float humstd = hum_tensor.std().item<float>() + 1e-8;

   float osmean = os_tensor.mean().item<float>()+ 1e-8;
  float osstd = os_tensor.std().item<float>() + 1e-8;

  float windmean = wind_tensor.mean().item<float>()+ 1e-8;
 float windstd = wind_tensor.std().item<float>() + 1e-8;
    return std::make_tuple(idmean,idstd,daysmean,daysstd,moumean,moustd,salesmean,salesstd,tempmean,tempstd,hummean,humstd,osmean,osstd,windmean,windstd);
}

/*std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> normalize_data(
    const std::vector<std::tuple<int, int, int, float>>& data) {
    std::vector<int> ids, days, months;
    std::vector<float> sales;

    std::transform(data.begin(), data.end(), std::back_inserter(ids), [](const auto& tup) { return std::get<0>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(days), [](const auto& tup) { return std::get<1>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(months), [](const auto& tup) { return std::get<2>(tup); });
    std::transform(data.begin(), data.end(), std::back_inserter(sales), [](const auto& tup) { return std::get<3>(tup); });

    torch::Tensor ids_tensor = torch::tensor(ids, torch::kFloat32);
    torch::Tensor days_tensor = torch::tensor(days, torch::kFloat32);
    torch::Tensor months_tensor = torch::tensor(months, torch::kFloat32);
    torch::Tensor sales_tensor = torch::tensor(sales, torch::kFloat32);

    // Z-нормализация
    auto z_normalize = [](torch::Tensor& tensor) {
        auto mean = tensor.mean().item<float>()+ 1e-8;
        auto std = tensor.std().item<float>() + 1e-8;

        tensor = (tensor - mean) / std;

        std::cout << "Normalized values:" << std::endl;
        std::cout << tensor << std::endl;
    };

    z_normalize(days_tensor);
    z_normalize(months_tensor);
    z_normalize(sales_tensor);

    return std::make_tuple(ids_tensor, days_tensor, months_tensor, sales_tensor);
}*/



SalesPredictionModel::SalesPredictionModel() {
    lstm1 = register_module("lstm1", torch::nn::LSTM(torch::nn::LSTMOptions(input_size, hidden_size).num_layers(1).batch_first(true)));
    lstm2 = register_module("lstm2", torch::nn::LSTM(torch::nn::LSTMOptions(hidden_size, hidden_size).num_layers(1).batch_first(true)));
    linear2 = register_module("linear2", torch::nn::Linear(hidden_size, output_size));
}

/*torch::Tensor SalesPredictionModel::forward(torch::Tensor id_tensor, torch::Tensor days_tensor, torch::Tensor months_tensor) {
    auto input_tensor = torch::stack({ days_tensor, months_tensor }).unsqueeze(0).transpose(1, 2);

    auto lstm_output1 = lstm1->forward(input_tensor);
    auto lstm_out1 = std::get<0>(lstm_output1).squeeze(0);

 \

    return linear2->forward(lstm_out1.unsqueeze(0));
}*/

torch::Tensor SalesPredictionModel::forward(torch::Tensor id_tensor, torch::Tensor days_tensor, torch::Tensor months_tensor,
                                            torch::Tensor temp_tensor,torch::Tensor hum_tensor,torch::Tensor os_tensor,torch::Tensor wind_tensor) {
    auto input_tensor = torch::stack({ days_tensor, months_tensor, temp_tensor, hum_tensor, os_tensor, wind_tensor}).unsqueeze(0).transpose(1, 2); // тут какая то хуета

    auto lstm_output1 = lstm1->forward(input_tensor);
    auto lstm_out1 = std::get<0>(lstm_output1).squeeze(0);

 \

    return linear2->forward(lstm_out1.unsqueeze(0));
}


