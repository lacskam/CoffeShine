import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import "qrc:/QMLFile/Delegates"
import "qrc:/QMLFile/Delegates/Highlighters"
import "qrc:/QMLFile/Styles"


ApplicationWindow {
    id: mainView
    x: 0
    y: 0

    minimumWidth: mainView.dp(640)
    minimumHeight: mainView.dp(480)

    title: qsTr("Coffee Time Client")
    visible: true
    onClosing: {
        if (interlayerForQML.getOSAndroidOrNot())
        {
            close.accepted = false
        }
    }

    Connections {
           target: interlayerForQML // Указываем целевое соединение

           onCreateTab_Signal: {
               listTabs.append({ name:       qmlProperty.tabsName,
                                 idCategory: qmlProperty.idCategory,
                                 colorCategory: qmlProperty.colorForCategory})
           }

           onFinishCreateTabs_Signal: {
               mainView.setCurrentTab(0)

           }

           onCreateMenu_Signal:{
               listMenu.append({ name:       qmlProperty.name,
                                 idDrink:    qmlProperty.idDrink,
                                 idCategory: qmlProperty.idCategory,
                                 colorCode:  qmlProperty.colorCode })
           }

           onGetSomePropertyForSoldPosition_Signal:{
                      qmlProperty.idDrinkForSoldPosition = orderListModel.get(index).idDrink
                      qmlProperty.idVolumeForSoldPosition = orderListModel.get(index).idVolume
                      qmlProperty.idPriceForSoldPosition = orderListModel.get(index).idPrice
                      qmlProperty.idTypeDrinkForSoldPosition = orderListModel.get(index).idTypeDrink
           }
    }

    Rectangle {
        id: formOrder
        x: 0
        y: 0

        height: mainView.width > mainView.height ? mainView.height : mainView.height / 2
        width:  mainView.width > mainView.height ?
                ((mainView.width / 3 > mainView.dp(500)) ? mainView.dp(500) : mainView.width / 3)
                : mainView.width

        color: "#795548"

        border.width: mainView.dp(1)
        border.color: "white"
        radius: mainView.dp(15)

        Image {
            id: backgroundForOrderView
            anchors.fill: parent
            anchors.margins: mainView.dp(1)

            source: "qrc:/Image/backgroundForOrderView.jpg"

            fillMode: Image.PreserveAspectCrop
            smooth: true

            layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: backgroundForOrderView.width
                        height: backgroundForOrderView.height

                        Rectangle {
                            anchors.centerIn: parent
                            width:  backgroundForOrderView.width
                            height:  backgroundForOrderView.height
                            radius: formOrder.radius
                        }
                    }
                }
        }

        Rectangle {
            id: lcdSum
            anchors.right: parent.right
            anchors.rightMargin: mainView.dp(10)
            y:upperMenu.height+mainView.dp(5)

            height: mainView.dp(35)
            width: parent.width/1.7

            color: "lightgreen"
            visible: totalCost.text !== "0 руб. 0 коп." ? true : false
            border.width: mainView.dp(2)
            border.color: "white"
            radius: mainView.dp(10)

            TextForButton {
                id : totalCost
                anchors{
                    fill: parent
                    rightMargin: mainView.dp(10)
                }

                text: "0 руб. 0 коп."
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
            }
        }
        Rectangle{
            anchors{
                top: lcdSum.top
                bottom: lcdSum.bottom
                right: lcdSum.left
                rightMargin: mainView.dp(5)
                left: parent.left
                leftMargin: mainView.dp(10)
                topMargin: mainView.dp(2)
            }

            color: Qt.rgba(0,0,0,0.7)
            radius: mainView.dp(10)
            visible: lcdSum.visible

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                text: "Наличные"
                color: "white"
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 8
                font.pointSize: 13
            }
        }

        Rectangle {
            id: lcdSum2
            anchors.horizontalCenter: lcdSum.horizontalCenter
            anchors.top: lcdSum.visible ? lcdSum.bottom : parent.top
            anchors.topMargin: lcdSum.visible ? 0 : upperMenu.height+mainView.dp(5)

            height: lcdSum.height
            width: lcdSum.width

            color: lcdSum.color
            visible: totalCost2.text !== "0 руб. 0 коп." ? true : false

            border.width: mainView.dp(2)
            border.color: "white"
            radius: mainView.dp(10)

            TextForButton {
                id : totalCost2
                anchors{
                    fill: parent
                    rightMargin: mainView.dp(10)
                }

                text: "0 руб. 0 коп."
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
            }
        }

        Rectangle {
            anchors{
                top: lcdSum2.top
                bottom: lcdSum2.bottom
                right: lcdSum2.left
                rightMargin: mainView.dp(5)
                left: parent.left
                leftMargin: mainView.dp(10)
                topMargin: mainView.dp(2)
            }

            color: Qt.rgba(0,0,0,0.7)
            radius: mainView.dp(10)
            visible: lcdSum2.visible

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                text: "Безналичные"
                color: "white"
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 8
                font.pointSize: 13
            }
        }

        ListView {
            id: orderListView
            anchors{
                top: lcdSum.visible ? (lcdSum2.visible ? lcdSum2.bottom : lcdSum.bottom) : (lcdSum2.visible ? lcdSum2.bottom : parent.top)
                topMargin: lcdSum.visible || lcdSum2.visible ? 0 : upperMenu.height+mainView.dp(5)
                bottom: btnOrder.top
                left: parent.left
                right: parent.right
            }


            boundsBehavior: Flickable.StopAtBounds
            clip: true

            delegate: MyItemForOrderList{}

            model:ListModel {id: orderListModel}

            add: Transition {
                NumberAnimation {
                    properties: "x,y";
                    from: 100;
                    duration: 1000
                }
            }

            removeDisplaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 1000 }
            }

            remove: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                    NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
                    NumberAnimation { property: "scale"; to: 3; duration: 1000 }
                    NumberAnimation { property: "rotation"; to: 90; duration: 1000}
                }
            }
        }

        Rectangle {
            id: btnOrder
            anchors{
                bottom: formOrder.bottom
                bottomMargin: mainView.dp(3)
                horizontalCenter: formOrder.horizontalCenter
            }

            height: mainView.dp(70)
            width: formOrder.width*0.95

            border.color: "#B6B6B6"
            border.width: mainView.dp(3)
            radius: mainView.dp(20)

            visible: orderListModel.count>0
            // Сглаживание включено
            smooth: true

            TextForButton{
                text: qsTr("Подтверждение заказа")
                height: parent.height
            }

            MouseArea {
                id: btnOrderArea
                anchors.fill: btnOrder

                onClicked: {
                    interlayerForQML.createReceipt_Slot(orderListModel.count)
                    orderListModel.clear()
                    mainView.totalSumOrder()
                }
            }

            scale: btnOrderArea.pressed ? 0.8 : 1.0

            gradient: GradientForButton{}
        }
    }

    Rectangle {
        id: mainLayoutForMenu
        x: (mainView.width > mainView.height ? formOrder.width : 0) + mainView.dp(1)
        y: mainView.width > mainView.height ? 0 : formOrder.height

        height: formOrder.height
        width: (mainView.width > mainView.height ?
                mainView.width-formOrder.width
                : mainView.width) - mainView.dp(1)

        Rectangle {
            id: layoutForTabs
            anchors{
                top: mainLayoutForMenu.top
                left: parent.left
                right: parent.right
                topMargin: mainView.dp(1)
                rightMargin: mainView.dp(1)
            }
            height:  mainView.dp(65)

            ListView {
                id: viewForTabs
                orientation: ListView.Horizontal
                anchors.fill: parent
                cacheBuffer: 200
                boundsBehavior: Flickable.StopAtBounds
                clip: true

                delegate: MyItemForTabs{}

                highlight: HighlightForTabs{}
                highlightFollowsCurrentItem: false

                model: ListModel { id: listTabs }
            }
        }

        Rectangle {
            id: layoutForMenu
            anchors{
                top: layoutForTabs.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                bottomMargin: mainView.dp(1)
                rightMargin: mainView.dp(1)
            }

            color: "#5D4037"
            radius: mainView.dp(15)

                 Rectangle{
                      anchors{
                          top: parent.top
                          left: parent.left
                          right: parent.right
                      }

                      height: parent.radius
                      color: "#5D4037"
                  }

            Image {
                id: backgroundForMenu
                anchors.fill: parent
                anchors.topMargin:  mainView.dp(2)

                source: "qrc:/Image/backgroundForMenu.jpg"

                fillMode: Image.PreserveAspectCrop
                smooth: true

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Item {
                                    width: backgroundForMenu.width
                                    height: backgroundForMenu.height

                                    Rectangle {
                                        anchors.centerIn: parent
                                        width:  backgroundForMenu.width
                                        height:  layoutForMenu.height
                                        radius: layoutForMenu.radius
                                        color: "#5D4037"
                                    }
                                }
                }
            }

            GridView {
                id: gridViewForItemFromMenu
                anchors{
                    fill: parent
                    leftMargin: mainView.dp(6)
                    rightMargin: mainView.dp(6)
                    topMargin: mainView.dp(3)
                    bottomMargin: mainView.dp(3)
                }

                snapMode: GridView.SnapOneRow


                cellHeight:  mainView.getCellHeightForMenuGridView()
                cellWidth: mainView.getCellWidthForMenuGridView()

                flow: GridView.TopToBottom
                clip: true

                delegate: MyItemForMenu{}

                model: ListModel { id: listMenu }
            }
        }
    }

    Loader {
        id: upperMenu
        visible: true

        source: "Views/UpperMenuView.qml"
    }

    Loader {
            id: loaderSelectionForOrder
            anchors.fill: parent
            source: "Views/LoginView.qml"           // первый раз запускаем для логина
            visible: false

            state: "State2"
            states: [
                State {
                    //Обычное состояние меню
                    name: "State1"
                },
                State {
                    //Обычное состояние меню
                    name: "State2"
                    PropertyChanges {
                        target: loaderSelectionForOrder
                        visible: true
                    }
                }
            ]
        }

    Loader {
        id: loaderReportView
        anchors.fill: parent
        source: "Views/ReportView.qml"

        visible: false

        state: "State1"
        states: [
            State {
                //Обычное состояние меню
                name: "State1"
            },
            State {
                //Обычное состояние меню
                name: "State2"
                PropertyChanges {
                    target: loaderReportView
                    visible: true
                }
            }
        ]
    }


    property int previousIndexOfButtonTabs: 0

    function setCurrentTab(index)
    {
        viewForTabs.currentIndex = mainView.previousIndexOfButtonTabs
        viewForTabs.currentItem.state = "State1"

        mainView.previousIndexOfButtonTabs = index
        mainView.openList(listTabs.get(index).idCategory)
        qmlProperty.currentColorCategory = listTabs.get(index).colorCategory

        viewForTabs.currentIndex = index
        viewForTabs.currentItem.state = "State2"
    }


    property int dpi: Screen.pixelDensity * 25.4

    function dp(x){
        if(dpi < 120) {
            return x; // Для обычного монитора компьютера
        } else if (dpi>350)
        {
            return x*(dpi/200)
        }
        else if (dpi>240)
        {
            return x*(dpi/200)
        }
        else if (dpi>200)
        {
            return x*(dpi/160)
        }
        else
        {
             return x*(dpi/120);
        }
    }


    property int wantedHeight: gridViewForItemFromMenu.height/mainView.dp(140)
    property int wantedWidth: gridViewForItemFromMenu.width/mainView.dp(175)

    function getCellHeightForMenuGridView()
    {
        return wantedHeight!=0 ? gridViewForItemFromMenu.height/wantedHeight : gridViewForItemFromMenu.height
    }

    function getCellWidthForMenuGridView()
    {
        return wantedWidth!=0 ? gridViewForItemFromMenu.width/wantedWidth : gridViewForItemFromMenu.width
    }


    function openList(index)
    {
         listMenu.clear();

         interlayerForQML.fillingListMenu_Slot(index);
    }


    signal openSelectionView()

    function prepareToOpenSelectionView(idFromList, idDrink, idCategory, editMode)
    {
        qmlProperty.editMode = editMode
        qmlProperty.idDrinkForCurrentItem = idDrink
        qmlProperty.idCategoryForCurrentItem = idCategory


        if (editMode)
        {
            qmlProperty.idInOrderList = idFromList
            qmlProperty.nameForCurrentItem = orderListModel.get(idFromList).name
            qmlProperty.colorForCurrentItem = orderListModel.get(idFromList).colorCode
            qmlProperty.imageForCurrentItem = orderListModel.get(idFromList).sourceFile
            qmlProperty.idVolumeInItemListVolume = orderListModel.get(idFromList).idVolumeInItemListVolume
            qmlProperty.idPayInItemListOfPay = orderListModel.get(idFromList).idPayInItemListOfPay
            qmlProperty.colorCategoryForCurrentItem = orderListModel.get(idFromList).colorCategory
        }
        else
        {
            qmlProperty.idInListMenu = idFromList
            qmlProperty.nameForCurrentItem = listMenu.get(idFromList).name
            qmlProperty.colorForCurrentItem = listMenu.get(idFromList).colorCode
            qmlProperty.imageForCurrentItem = listMenu.get(idFromList).idDrink
            qmlProperty.idVolumeInItemListVolume = -1
            qmlProperty.idPayInItemListOfPay = -1
        }

        interlayerForQML.fillingListVolume_Slot(qmlProperty.idDrinkForCurrentItem)
        interlayerForQML.fillingListVarietyOfPay_Slot()
        mainView.openSelectionView()
        loaderSelectionForOrder.state = "State2"
    }

    property int rubels
    property int coins
    property string stringCost

    function appendToOrderList(volume, idVolume, nameVolume, volumeUnits, nameVarietyOfPay, currentIdVolume, currentIdOfPay, idTypeDrink)
    {
        stringCost = nameVarietyOfPay
        rubels = 0
        coins = 0
        var haveCoins = interlayerForQML.getPrice(qmlProperty.idCategoryForCurrentItem, qmlProperty.idDrinkForCurrentItem, idVolume)

        if (nameVarietyOfPay !== "Бесплатный")
        {         
            stringCost += " "+qmlProperty.rubels + " руб."
            rubels = qmlProperty.rubels

            if (haveCoins)
            {
                stringCost += " "+qmlProperty.coins + " коп."
                coins = qmlProperty.coins
            }
        }

        orderListModel.append({ name:          qmlProperty.nameForCurrentItem,
                                idDrink:       qmlProperty.idDrinkForCurrentItem,
                                idCategory:    qmlProperty.idCategoryForCurrentItem,
                                colorCategory: qmlProperty.currentColorCategory,
                                idVolume:      idVolume,
                                idPrice:       qmlProperty.idPrice,
                                idTypeDrink:   idTypeDrink,
                                typeDrink:     nameVarietyOfPay,
                                sourceFile:    qmlProperty.imageForCurrentItem,
                                colorCode:     qmlProperty.colorForCurrentItem,
                                stringSize:    nameVolume +" "+ volume +" "+ volumeUnits,
                                stringCost: stringCost,
                                rubels:     rubels,
                                coins:      coins,
                                idVolumeInItemListVolume: currentIdVolume,
                                idPayInItemListOfPay:     currentIdOfPay })

        mainView.totalSumOrder()
    }

    function editOrderList(volume, idVolume, nameVolume, volumeUnits, nameVarietyOfPay, currentIdVolume, currentIdOfPay, idTypeDrink)
    {
        stringCost = nameVarietyOfPay
        rubels = 0
        coins = 0
        var haveCoins = interlayerForQML.getPrice(qmlProperty.idCategoryForCurrentItem, qmlProperty.idDrinkForCurrentItem, idVolume)

        if (nameVarietyOfPay !== "Бесплатный")
        {
            stringCost += " "+qmlProperty.rubels + " руб."
            rubels = qmlProperty.rubels

            if (haveCoins)
            {
                stringCost += " "+qmlProperty.coins + " коп."
                coins = qmlProperty.coins
            }
        }

        orderListModel.remove(qmlProperty.idInOrderList,1)
        orderListModel.insert(qmlProperty.idInOrderList,
                              { name:          qmlProperty.nameForCurrentItem,
                                idDrink:       qmlProperty.idDrinkForCurrentItem,
                                idCategory:    qmlProperty.idCategoryForCurrentItem,
                                colorCategory: qmlProperty.colorCategoryForCurrentItem,
                                idVolume:      idVolume,
                                idPrice:       qmlProperty.idPrice,
                                idTypeDrink:   idTypeDrink,
                                typeDrink:     nameVarietyOfPay,
                                sourceFile:    qmlProperty.imageForCurrentItem,
                                colorCode:     qmlProperty.colorForCurrentItem,
                                stringSize:    nameVolume +" "+ volume +" "+ volumeUnits,
                                stringCost: stringCost,
                                rubels:     rubels,
                                coins:      coins,
                                idVolumeInItemListVolume: currentIdVolume,
                                idPayInItemListOfPay:     currentIdOfPay })

        mainView.totalSumOrder()
    }

    property int rubelsNal: 0
    property int coinsNal: 0
    property int rubelsBeznal: 0
    property int coinsBeznal: 0

    function totalSumOrder(){

        rubels = 0
        coins = 0
        rubelsBeznal = 0
        coinsBeznal = 0
        rubelsNal = 0
        coinsNal = 0

        for(var i=0; i<orderListModel.count;++i)
        {
            rubels += orderListModel.get(i).rubels
            coins  += orderListModel.get(i).coins

            if (orderListModel.get(i).typeDrink === "Безналичные")
            {
                rubelsBeznal += orderListModel.get(i).rubels
                coinsBeznal  += orderListModel.get(i).coins
            }
            else
            {
                rubelsNal += orderListModel.get(i).rubels
                coinsNal  += orderListModel.get(i).coins
            }

            if(i == orderListModel.count-1)
            {
                rubelsNal += coinsNal/100
                coinsNal %= 100
                rubelsBeznal += coinsBeznal/100
                coinsBeznal %= 100
                rubels += coins/100
                coins %= 100
            }
        }

        totalCost.text = rubelsNal +" руб. "+ coinsNal +" коп."
        totalCost2.text = rubelsBeznal +" руб. "+ coinsBeznal +" коп."

        qmlProperty.rubels = rubels
        qmlProperty.coins = coins
    }
}
