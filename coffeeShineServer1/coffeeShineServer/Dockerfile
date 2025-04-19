FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    libcurl4-openssl-dev \
    qtbase5-dev \
    qt5-qmake \
    qttools5-dev \
    qttools5-dev-tools \
    libqt5sql5 \
    libqt5sql5-sqlite \
    libqt5network5 \
    libgl1-mesa-dev \
    libx11-xcb1 \
    wget \
    unzip \
    git

# Установка CMake 3.26.4
RUN wget https://github.com/Kitware/CMake/releases/download/v3.26.4/cmake-3.26.4-linux-x86_64.sh && \
    chmod +x cmake-3.26.4-linux-x86_64.sh && \
    ./cmake-3.26.4-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.26.4-linux-x86_64.sh

# Копирование проекта
WORKDIR /app
COPY . .

ENV Torch_DIR=/app/include/libtorch/share/cmake/Torch

# Сборка проекта
RUN cmake -DCMAKE_BUILD_TYPE=Release . && make -j$(nproc)

# Проброс порта (замени на нужный)
EXPOSE 8080

CMD ["./coffeeShineServerNN"]
