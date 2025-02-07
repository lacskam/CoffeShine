QT += quick
QT += core gui
QT += sql
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    interlayerforqml.cpp \
    DrinkImageProvider/drinkimageprovider.cpp \
    ../../Common/QUnNetProtokol/qcrc32.cpp \
    ../../Common/QUnNetProtokol/qunnetprotokolclient.cpp \
    ../../Common/QUnNetProtokol/qunpacketmanager.cpp \
    ../../Common/QUnNetProtokol/qunaccountmanager.cpp \
    ../../Common/QUnNetProtokol/QUnHostsDialog/qunhostsdialog.cpp \
    ../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.cpp \
    ../../Common/QUnNetProtokol/qundbpacketmanager.cpp \
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
    startapp.cpp

RESOURCES += qml.qrc \
    images.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    interlayerforqml.h \
    DrinkImageProvider/drinkimageprovider.h \
    ../../Common/QUnNetProtokol/commonunnetworktypes.h \
    ../../Common/QUnNetProtokol/qcrc32.h \
    ../../Common/QUnNetProtokol/qunnetprotokolclient.h \
    ../../Common/QUnNetProtokol/qunpacketmanager.h \
    ../../Common/QUnNetProtokol/qunaccountmanager.h \
    ../../Common/QUnNetProtokol/QUnHostsDialog/qunhostsdialog.h \
    ../../Common/QUnNetProtokol/QUnHostsWidget/qunhostswidget.h \
    ../../Common/QUnNetProtokol/qundbpacketmanager.h \
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
    startapp.h

DISTFILES += \
    Data/list.csv \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
