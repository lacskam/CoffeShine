#-------------------------------------------------
#
# Project created by QtCreator 2019-05-02T19:28:56
#
#-------------------------------------------------

QT       += core gui charts
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopAdminClientForCoffeeShine
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    ../../Common/QUnNetProtokol/qcrc32.cpp \
    ../../Common/QUnNetProtokol/qunnetprotokolclient.cpp \
    ../../Common/QUnNetProtokol/qunpacketmanager.cpp \
    ../../Common/QUnNetProtokol/qunaccountmanager.cpp \
    ../../Common/QUnNetProtokol/QUnHostsDialog/qunhostsdialog.cpp \
    ../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.cpp \
    ../../Common/QPlugins/qclientplugin.cpp \
    ../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeCategoryInfo/qcoffeecategoryinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeDrinkInfo/qcoffeedrinkinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePointSale/qcoffeepointsale.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePriceInfo/qcoffeepriceinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeTypeDrink/qcoffeetypedrink.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeUserInfo/qcoffeeuserinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeReceiptInfo/qcoffeereceiptinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeShiftInfo/qcoffeeshiftinfo.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QColorConsole/qcolorconsole.cpp \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePictureInfo/qcoffeepictureinfo.cpp \
    ../../Common/QUnNetProtokol/qundbpacketmanager.cpp \
    include/QCustomScrollArea/QCustomScrollArea.cpp \
    include/QDbTabWidget/QCategoryWidget/QCategoryItem/qcategoryitem.cpp \
    include/QDbTabWidget/QCategoryWidget/qcategorywidget.cpp \
    include/QDbTabWidget/QColorWidget/qcolorwidget.cpp \
    include/QDbTabWidget/QDrinkWidgetItem/qdrinkwidgetitem.cpp \
    include/QDbTabWidget/QEditDrinkWidget/QPriceWidget/QPriceWidgetItem/qpricewidgetitem.cpp \
    include/QDbTabWidget/QEditDrinkWidget/QPriceWidget/qpricewidget.cpp \
    include/QDbTabWidget/QEditDrinkWidget/qeditdrinkwidget.cpp \
    include/QDbTabWidget/QEditDrinkWidget/QDeleteDialog/qdeletedialog.cpp \
    include/QDbTabWidget/QMarkWidget/QMarkWidgetItem/qmarkwidgetitem.cpp \
    include/QDbTabWidget/QMarkWidget/qmarkwidget.cpp \
    include/QDbTabWidget/QPictureWidget/QPrictureDialog/qpicturedialog.cpp \
    include/QDbTabWidget/QPictureWidget/QPrictureDialog/qpicturewidgetitem.cpp \
    include/QDbTabWidget/QPictureWidget/qpicturewidget.cpp \
    include/QDbTabWidget/qdbtabwidget.cpp \
    include/QDesktopClientLoader/QPointSalePickWidget/qpointsaleeditwidget.cpp \
    include/QDesktopClientLoader/qdesktopclientloader.cpp \
    include/QHostsDialog/qhostsdialog.cpp \
    include/QHTabWidget/qhtabbar.cpp \
    include/QHTabWidget/qhtabwidget.cpp \
    include/QNnTabWidget/QNnSettingsWidget/qnnsettingswidget.cpp \
    include/QStatisticTabWidget/qstatistictabwidget.cpp \
    include/QUserAccDialog/quseraccdialog.cpp \
    include/QNnTabWidget/qnntabwidget.cpp \
    include/QNnTabWidget/QProductsStatPred/qproductstatisticprediction.cpp \
    include/QNnTabWidget/QLoadingWidget/qloadingwidget.cpp \
    include/QNnFileProcessing/QNnDbProcessing.cpp \
    include/QDesktopClientLoader/QPointSalePickWidget/qpointsalepickwidget.cpp \
    include/QDesktopClientLoader/QPointSalePickWidget/QPointSalePickWidgetItem/qpointsalepickwidgetitem.cpp \
  #  include/QNnFileProcessing/QWeather/weather.cpp \
   # ../../Common/QPlugins/QCoffeePlugin/QNnPredictionInfo/qnnpredictioninfo.cpp  \
    main.cpp \
    widget.cpp

HEADERS += \
    ../../Common/QUnNetProtokol/commonunnetworktypes.h \
    ../../Common/QUnNetProtokol/qcrc32.h \
    ../../Common/QUnNetProtokol/qunnetprotokolclient.h \
    ../../Common/QUnNetProtokol/qunpacketmanager.h \
    ../../Common/QUnNetProtokol/qunaccountmanager.h \
    ../../Common/QUnNetProtokol/QUnHostsDialog/qunhostsdialog.h \
    ../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.h \
    ../../Common/QPlugins/qclientplugin.h \
    ../../Common/QPlugins/QCoffeePlugin/qcoffeeclientplugin.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeCategoryInfo/qcoffeecategoryinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeDrinkInfo/qcoffeedrinkinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePointSale/qcoffeepointsale.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePriceInfo/qcoffeepriceinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeSoldPositionInfo/qcoffeesoldpositioninfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeTypeDrink/qcoffeetypedrink.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeUserInfo/qcoffeeuserinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeVolumeDrinkInfo/qcoffeevolumedrinkinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeReceiptInfo/qcoffeereceiptinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeeShiftInfo/qcoffeeshiftinfo.h \
    ../../Common/QPlugins/QCoffeePlugin/QColorConsole/qcolorconsole.h \
    ../../Common/QPlugins/QCoffeePlugin/QCoffeePictureInfo/qcoffeepictureinfo.h \
    ../../Common/QUnNetProtokol/qundbpacketmanager.h \
    include/QCustomScrollArea/QCustomScrollArea.h \
    include/QDbTabWidget/QCategoryWidget/QCategoryItem/qcategoryitem.h \
    include/QDbTabWidget/QCategoryWidget/qcategorywidget.h \
    include/QDbTabWidget/QColorWidget/qcolorwidget.h \
    include/QDbTabWidget/QDrinkWidgetItem/qdrinkwidgetitem.h \
    include/QDbTabWidget/QEditDrinkWidget/QPriceWidget/QPriceWidgetItem/qpricewidgetitem.h \
    include/QDbTabWidget/QEditDrinkWidget/QPriceWidget/qpricewidget.h \
    include/QDbTabWidget/QEditDrinkWidget/qeditdrinkwidget.h \
    include/QDbTabWidget/QEditDrinkWidget/QDeleteDialog/qdeletedialog.h \
    include/QDbTabWidget/QMarkWidget/QMarkWidgetItem/qmarkwidgetitem.h \
    include/QDbTabWidget/QMarkWidget/qmarkwidget.h \
    include/QDbTabWidget/QPictureWidget/QPrictureDialog/qpicturedialog.h \
    include/QDbTabWidget/QPictureWidget/QPrictureDialog/qpicturewidgetitem.h \
    include/QDbTabWidget/QPictureWidget/qpicturewidget.h \
    include/QDbTabWidget/commondbtabtypes.h \
    include/QDbTabWidget/qdbtabwidget.h \
    include/QDesktopClientLoader/QPointSalePickWidget/qpointsaleeditwidget.h \
    include/QDesktopClientLoader/qdesktopclientloader.h \
    include/QHostsDialog/qhostsdialog.h \
    include/QHTabWidget/qhtabbar.h \
    include/QHTabWidget/qhtabwidget.h \
    include/QNnTabWidget/QNnSettingsWidget/qnnsettingswidget.h \
    include/QStatisticTabWidget/qstatistictabwidget.h \
    include/QUserAccDialog/quseraccdialog.h \
    include/QNnTabWidget/qnntabwidget.h \
    include/QNnTabWidget/QProductsStatPred/qproductstatisticprediction.h \
    include/QNnTabWidget/QLoadingWidget/qloadingwidget.h \
    include/QNnFileProcessing/QNnDbProcessing.h \
    include/QDesktopClientLoader/QPointSalePickWidget/qpointsalepickwidget.h \
    include/QDesktopClientLoader/QPointSalePickWidget/QPointSalePickWidgetItem/qpointsalepickwidgetitem.h \
  #  ../../Common/QPlugins/QCoffeePlugin/QNnPredictionInfo/qnnpredictioninfo.h \
   #  include/QNnFileProcessing/QWeather/weather.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    images.qrc
