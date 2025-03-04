import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/QMLFile/Delegates"
import "qrc:/QMLFile/Delegates/Highlighters"
import "qrc:/QMLFile/Styles"

Rectangle {
    id: selectionMenu
    anchors.fill: parent
    color:  qmlProperty.colorForCurrentItem !== undefined ? qmlProperty.colorForCurrentItem : "white"

    border.width: mainView.dp(2)
    border.color: Qt.lighter(color)
    radius: mainView.dp(10)

    property string colorCode
    property string sourceFile

    Connections {
        target: mainView

        onOpenSelectionView:{
            setCurrentVolume(qmlProperty.idVolumeInItemListVolume)
            setCurrentVarietyOfPay(qmlProperty.idPayInItemListOfPay)
        }
    }

    Connections {
           target: interlayerForQML // Указываем целевое соединение

           onCreateVolume_Signal:{
                volumeListModel.append({ volume:      qmlProperty.volume,
                                         nameVolume:  qmlProperty.nameVolume,
                                         idVolume:    qmlProperty.idVolume,
                                         volumeUnits: qmlProperty.volumeUnits})
           }

           onCreateVarietyOfPay_Signal:{
               varietyOfPayListModel.append({ nameVarietyOfPay: qmlProperty.nameVarietyOfPay,
                                              idVarietyOfPay:   qmlProperty.idVarietyOfPay })
           }
    }


    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (currentIdOfPay !== -1 && currentIdVolume !== -1)
            {
                if (qmlProperty.editMode)
                {
                    mainView.editOrderList(volumeListModel.get(currentIdVolume).volume, volumeListModel.get(currentIdVolume).idVolume,
                                           volumeListModel.get(currentIdVolume).nameVolume, volumeListModel.get(currentIdVolume).volumeUnits,
                                           varietyOfPayListModel.get(currentIdOfPay).nameVarietyOfPay,
                                           currentIdVolume, currentIdOfPay, varietyOfPayListModel.get(currentIdOfPay).idVarietyOfPay )
                }
                else
                {
                    mainView.appendToOrderList(volumeListModel.get(currentIdVolume).volume, volumeListModel.get(currentIdVolume).idVolume,
                                               volumeListModel.get(currentIdVolume).nameVolume, volumeListModel.get(currentIdVolume).volumeUnits,
                                               varietyOfPayListModel.get(currentIdOfPay).nameVarietyOfPay,
                                               currentIdVolume, currentIdOfPay, varietyOfPayListModel.get(currentIdOfPay).idVarietyOfPay  )
                }
            }

            loaderSelectionForOrder.state = "State1"
            volumeListModel.clear()
            varietyOfPayListModel.clear()
        }

        onPressAndHold: {                    
            if (!qmlProperty.editMode)
            {
                if (currentIdOfPay !== -1 && currentIdVolume !== -1)
                {
                    for(var i=0;i<2;++i)
                        mainView.appendToOrderList(volumeListModel.get(currentIdVolume).volume, volumeListModel.get(currentIdVolume).idVolume,
                                                   volumeListModel.get(currentIdVolume).nameVolume, volumeListModel.get(currentIdVolume).volumeUnits,
                                                   varietyOfPayListModel.get(currentIdOfPay).nameVarietyOfPay,
                                                   currentIdVolume, currentIdOfPay, varietyOfPayListModel.get(currentIdOfPay).idVarietyOfPay)
                }

                loaderSelectionForOrder.state = "State1"
                volumeListModel.clear()
                varietyOfPayListModel.clear()
            }
        }
    }

    Rectangle {
        id:layoutForListViewVolume
        anchors{
            left: parent.left
            right: mainView.width > mainView.height ? imgSelect.left : parent.right
            top: parent.top
            bottom: mainView.width > mainView.height ? parent.bottom : imgSelect.top
            margins: mainView.dp(10)
        }
        z: 1

        width: mainView.width > mainView.height ? mainView.width/4 : 0
        height: mainView.width > mainView.height ? 0 : mainView.height/4

        color: Qt.rgba(0.0, 0.0, 0.0, 0.7)
        radius: mainView.dp(20)

        ListView {
            id: volumeListView
            anchors{
                fill: parent
                margins: mainView.dp(5)
            }

            boundsBehavior: Flickable.StopAtBounds
            clip: true

            delegate: MyItemForVolumeList { }

            highlight: HighlightForListVolume{}
            highlightFollowsCurrentItem: false

            model: ListModel { id: volumeListModel}
        }
    }

    Rectangle {
        id: imgSelect

        x: mainView.width > mainView.height ? mainView.width/4 : 0
        y: mainView.width > mainView.height ? 0 : mainView.height/4

        width: mainView.width > mainView.height ? mainView.width/2 : mainView.width
        height: mainView.width > mainView.height ? mainView.height : mainView.height/2

        color:"transparent"


        Image {
            anchors.centerIn: parent
            width: parent.width/2.2
            height: parent.height/2.2
            source: qmlProperty.imageForCurrentItem !== undefined ? "image://drinks/" + qmlProperty.imageForCurrentItem : "qrc:/Image/coffeeBASE.png"
            fillMode: Image.PreserveAspectFit
            visible: true

             TextForImage{
                id: someText
                fontSizeMode: Text.HorizontalFit

                text: qmlProperty.nameForCurrentItem !== undefined ? qmlProperty.nameForCurrentItem : ""
                z:1
                size: 50
                color: "white"
            }

            Rectangle {
                anchors.top: parent.top
                anchors.topMargin: mainView.dp(10)
                anchors.horizontalCenter: someText.horizontalCenter
                width: someText.contentWidth + mainView.dp(10)
                height: someText.contentHeight - mainView.dp(5)
                z: 0
                color: Qt.rgba(0.0, 0.0, 0.0, 0.4)
                radius: mainView.dp(20)

            }

        }
    }

    Rectangle {
        id: layoutForPaymentButtons
        anchors{
            left: mainView.width > mainView.height ? imgSelect.right : parent.left
            right: parent.right
            top: mainView.width > mainView.height ? parent.top : imgSelect.bottom
            bottom: parent.bottom
            margins: mainView.dp(10)
        }

        x: mainView.width > mainView.height ? mainView.width*3/4 : 0
        y: mainView.width > mainView.height ? 0 : mainView.height*3/4

        width: mainView.width > mainView.height ? mainView.width/4 : mainView.width
        height: mainView.width > mainView.height ? mainView.height : mainView.height/4

        color: Qt.rgba(0.0, 0.0, 0.0, 0.7)
        radius: mainView.dp(20)

        ListView {
            id: varietyOfPayListView
            anchors{
                fill: parent
                margins: mainView.dp(5)
            }

            boundsBehavior: Flickable.StopAtBounds
            clip: true

            delegate: MyItemForVarietyOfPayList { }

            highlight: HighlightForVarietyOfPayList {}
            highlightFollowsCurrentItem: false

            model: ListModel { id: varietyOfPayListModel}

        }

    }

    Text {
        id: exitText
        anchors.bottom: mainView.width > mainView.height ? parent.bottom : layoutForPaymentButtons.top
        anchors.bottomMargin: mainView.dp(10)
        anchors.horizontalCenter: parent.horizontalCenter
        z:1
        text: (currentIdOfPay > -1 && currentIdVolume > -1) ? "Расчёт" : "Выход"
        font.pointSize: 30
        color: "white"
    }

    Rectangle {
        anchors.horizontalCenter: exitText.horizontalCenter
        anchors.verticalCenter: exitText.verticalCenter
        width: exitText.contentWidth + mainView.dp(15)
        height: exitText.contentHeight + mainView.dp(5)
        z: 0
        color: Qt.rgba(0.0, 0.0, 0.0, 0.4)
        radius: mainView.dp(15)
    }

    property int currentIdVolume

    function setCurrentVolume(index) {
         volumeListView.currentIndex = index
         currentIdVolume = index
    }

    property int currentIdOfPay

    function setCurrentVarietyOfPay(index) {
         varietyOfPayListView.currentIndex = index
         currentIdOfPay = index
    }

}
