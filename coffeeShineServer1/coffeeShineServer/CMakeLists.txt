cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(coffeeShineServerNN LANGUAGES CXX)

set(Torch_DIR include/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Sql Charts Network)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(coffeeShineServerNN
     main.cpp
        widget.cpp
    ../../Common/QUnNetProtokol/qcrc32.cpp
    ../../Common/QUnNetProtokol/qunaccountmanager.cpp
    ../../Common/QUnNetProtokol/qunclienthandler.cpp
    ../../Common/QUnNetProtokol/qundatabasesettings.cpp
    ../../Common/QUnNetProtokol/qunnetprotokolserver.cpp
    ../../Common/QUnNetProtokol/qunpacketmanager.cpp
    include/qserverloader.cpp
    ../../Common/QPlugins/qserverplugin.cpp
    ../../Common/QPlugins/QCoffeePlugin/qcoffeeserverplugin.cpp
    ../../Common/QDataBaseSettings/qdatabasesettings.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeDrinkInfo/qcoffeedrinkinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeCategoryInfo/qcoffeecategoryinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePointSale/qcoffeepointsale.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeTypeDrink/qcoffeetypedrink.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeUserInfo/qcoffeeuserinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeReceiptInfo/qcoffeereceiptinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePriceInfo/qcoffeepriceinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeShiftInfo/qcoffeeshiftinfo.cpp
    ../../Common/QPlugins/QCoffeePlugin/QColorConsole/qcolorconsole.cpp
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePictureInfo/qcoffeepictureinfo.cpp
    ../../Common/QUnNetProtokol/qundbpacketmanager.cpp
    ../../Common/QUnNetProtokol/qunaccountinfo.cpp
    include/QNnFileProcessing/QNnDbProcessing.cpp
    include/QNnFileProcessing/QNnLocalProcessing.cpp
    include/QNnFileProcessing/QWeather/weather.cpp
    include/QNn/nn.cpp


        widget.h
    ../../Common/QUnNetProtokol/commonunnetworktypes.h
    ../../Common/QUnNetProtokol/qcrc32.h
    ../../Common/QUnNetProtokol/qunaccountmanager.h
    ../../Common/QUnNetProtokol/qunclienthandler.h
    ../../Common/QUnNetProtokol/qundatabasesettings.h
    ../../Common/QUnNetProtokol/qunnetprotokolserver.h
    ../../Common/QUnNetProtokol/qunpacketmanager.h
    include/qserverloader.h
    ../../Common/QPlugins/qserverplugin.h
    ../../Common/QPlugins/QCoffeePlugin/qcoffeeserverplugin.h
    ../../Common/QDataBaseSettings/qdatabasesettings.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeDrinkInfo/qcoffeedrinkinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeCategoryInfo/qcoffeecategoryinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePointSale/qcoffeepointsale.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeTypeDrink/qcoffeetypedrink.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeUserInfo/qcoffeeuserinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeReceiptInfo/qcoffeereceiptinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePriceInfo/qcoffeepriceinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeShiftInfo/qcoffeeshiftinfo.h
    ../../Common/QPlugins/QCoffeePlugin/QColorConsole/qcolorconsole.h
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePictureInfo/qcoffeepictureinfo.h
    ../../Common/QUnNetProtokol/qundbpacketmanager.h
    ../../Common/QUnNetProtokol/qunaccountinfo.h
    include/nlohmann/json.hpp
    include/QNnFileProcessing/QNnDbProcessing.h
     include/QNnFileProcessing/QNnLocalProcessing.h
     include/QNnFileProcessing/QWeather/weather.h

    include/QNn/nn.h
    ../../Common/QPlugins/QCoffeePlugin/QNnPredictionInfo/qnnpredictioninfo.cpp ../../Common/QPlugins/QCoffeePlugin/QNnPredictionInfo/qnnpredictioninfo.h
    include/QNnFileProcessing/files/allprod1.sex include/QNnFileProcessing/files/weather.sex
)


target_link_libraries(coffeeShineServerNN PRIVATE ${CURL_LIBRARIES})
target_link_libraries(coffeeShineServerNN PRIVATE ${TORCH_LIBRARIES})
target_link_libraries(coffeeShineServerNN PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Charts Qt${QT_VERSION_MAJOR}::Network)

set_property(TARGET coffeeShineServerNN PROPERTY CXX_STANDARD 17)
