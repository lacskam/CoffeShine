#include "weather.h"
#include <QDebug>
#include <unistd.h>
#include <chrono>
#include <thread>
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *buffer) {
    size_t total_size = size * nmemb;
    buffer->append((char*)contents, total_size);
    return total_size;
}


Weather::Weather(QDate *TargetDate) {
    std::string buffer;
    std::string api_url = "https://archive-api.open-meteo.com/v1/era5?latitude=52.4345&longitude=30.975&start_date=" +
                          TargetDate->toString("yyyy-MM-dd").toStdString() +
                          "&end_date=" + TargetDate->toString("yyyy-MM-dd").toStdString() +
                          "&hourly=temperature_2m&hourly=wind_speed_10m&hourly=relative_humidity_2m&hourly=precipitation";

    std::cout << "API URL: " << api_url << std::endl;

    const int max_retries = 5;
    const int retry_delay_ms = 2000;
    int attempt = 0;

    bool success = false;

    while (attempt < max_retries && !success) {
        attempt++;
        std::cout << "Попытка #" << attempt << " выполнения запроса..." << std::endl;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, ("X-Yandex-API-Key: " + api_key).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                success = true;
                std::cout << "Запрос успешно выполнен на попытке #" << attempt << std::endl;
            } else {
                std::cerr << "Ошибка curl_easy_perform(): " << curl_easy_strerror(res) << std::endl;

                if (attempt < max_retries) {
                    std::cerr << "Повтор попытки через " << retry_delay_ms << " мс..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
                } else {
                    std::cerr << "Максимальное количество попыток исчерпано. Запрос не выполнен." << std::endl;
                }
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Не удалось инициализировать curl." << std::endl;
            break;
        }
    }

    if (!success) {
        std::cerr << "Ошибка: не удалось получить данные с сервера после " << max_retries << " попыток." << std::endl;
        return;
    }


    try {
        auto data = nlohmann::json::parse(buffer);

        temperature = 0;
        humidity = 0;
        os = 0;
        wind = 0;

        for (int i = 0; i < 24; i++) {
            double t = data["hourly"]["temperature_2m"][i];
            int h = data["hourly"]["relative_humidity_2m"][i];
            double o = data["hourly"]["precipitation"][i];
            double w = data["hourly"]["wind_speed_10m"][i];

            temperature += t;
            humidity += h;
            os += o;
            wind += w;
        }

        temperature /= 24;
        humidity /= 24;
        os /= 24;
        wind /= 24;

        std::cout << "Средняя Температура: " << temperature << " °C" << std::endl;
        std::cout << "Средняя Влажность: " << humidity << " %" << std::endl;
        std::cout << "Осадки: " << os * 100 << " %" << std::endl;
        std::cout << "Скорость ветра: " << wind << " м/с" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при обработке JSON: " << e.what() << std::endl;
    }
}



Weather::Weather(QDate *curentDate, int y) {
    std::string buffer;
    std::string api_url = "https://api.open-meteo.com/v1/forecast?latitude=52.4345&longitude=30.975&start_date=" +
                          curentDate->toString("yyyy-MM-dd").toStdString() + "&end_date=" +
                          curentDate->toString("yyyy-MM-dd").toStdString() +
                          "&hourly=temperature_2m&hourly=wind_speed_10m&hourly=relative_humidity_2m&hourly=precipitation";

    std::cout << "Запрос к API: " << api_url << std::endl;

    const int max_retries = 5;
    const int retry_delay_ms = 3000;
    int attempt = 0;

    bool success = false;

    while (attempt < max_retries && !success) {
        attempt++;
        std::cout << "Попытка #" << attempt << " выполнения запроса..." << std::endl;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, ("X-Yandex-API-Key: " + api_key).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            auto start = std::chrono::high_resolution_clock::now();

            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                success = true;
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "Запрос успешно выполнен на попытке #" << attempt << ". Время: " << duration.count()
                          << " секунд." << std::endl;
            } else {
                std::cerr << "curl_easy_perform() не удалось выполнить запрос: " << curl_easy_strerror(res) << std::endl;
                if (attempt < max_retries) {
                    std::cerr << "Повтор попытки через " << retry_delay_ms << " мс..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));
                } else {
                    std::cerr << "Максимальное количество попыток исчерпано. Запрос не выполнен." << std::endl;
                }
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Не удалось инициализировать curl." << std::endl;
            break;
        }
    }

    if (!success) {
        std::cerr << "Ошибка: не удалось получить данные с сервера после " << max_retries << " попыток." << std::endl;
        return;
    }


    try {
        auto data = nlohmann::json::parse(buffer);

        temperature = 0;
        humidity = 0;
        os = 0;
        wind = 0;

        for (int i = 0; i < 24; i++) {
            double t = data["hourly"]["temperature_2m"][i];
            int h = data["hourly"]["relative_humidity_2m"][i];
            double o = data["hourly"]["precipitation"][i];
            double w = data["hourly"]["wind_speed_10m"][i];

            temperature += t;
            humidity += h;
            os += o;
            wind += w;
        }

        temperature /= 24;
        humidity /= 24;
        os /= 24;
        wind /= 24;

        std::cout << "Средняя Температура: " << temperature << " градусов Цельсия" << std::endl;
        std::cout << "Средняя Влажность: " << humidity << "%" << std::endl;
        std::cout << "Осадки: " << os * 100 << "%" << std::endl;
        std::cout << "Ветер: " << wind << " м/c" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при обработке данных JSON: " << e.what() << std::endl;
    }
}


float Weather::get_temperature() {
    return temperature;
}


float Weather::get_humidity() {
    return humidity;
}


float Weather::getOs() {
    return os;
}

float Weather::getWindSpeed() {
    return wind;
}
