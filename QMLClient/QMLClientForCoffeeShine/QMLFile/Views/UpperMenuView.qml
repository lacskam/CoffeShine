import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import "qrc:/QMLFile/Styles"

Rectangle {
    id: upperMenu

    signal clearReportView()

    Connections {
        target: interlayerForQML
        onFillOpenDate_Signal:{
            upperMenuText.text = "Смена: "+qmlProperty.patronymicAndName+"\nОткрыта "+qmlProperty.openData
        }
    }

    x: mainView.dp(5)
    y: mainView.dp(5)

    height: mainView.dp(40)
    width: formOrder.width-mainView.dp(10)

    color: Qt.rgba(0.0, 0.0, 0.0, 0.6)

    radius: mainView.dp(15)

    Rectangle {
        id: layoutForUpperMenuText
        x: parent.width/2 - upperMenuText.contentWidth/2
        y: parent.height/2 - upperMenuText.contentHeight/2

        width: parent.width - mainView.dp(10)
        color: "transparent"

        Text {
            id: upperMenuText
            anchors.fill: parent
            width: parent.width

            color: "#B6B6B6"
            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 8
            font.pointSize: 12
        }
    }

    Column {
        id: boxButtonUpperMenu
        spacing: mainView.dp(15)
        x:mainView.dp(30)
        y:0
        z:3
        visible: false

        UpperMenuButton {
            buttonText: qsTr("Закрыть смену")

            MouseArea {
                id: closeShiftButtonArea
                anchors.fill: parent
                onClicked:{
                    closeCurrentShiftMessage.open()
                }
            }
            scale: closeShiftButtonArea.pressed ? 0.8 : 1.0
        }

        UpperMenuButton {
            buttonText: qsTr("Открыть отчёт")

            MouseArea {
                id: reportButtonArea
                anchors.fill: parent
                onClicked:{
                    loaderReportView.state = "State2"                   
                    interlayerForQML.fillingReceiptList_Slot()
                }
            }
            scale: reportButtonArea.pressed ? 0.8 : 1.0
        }
    }

    MouseArea {
        anchors.fill: parent

        onPressed : {
            switch (upperMenu.state){
                case "State1":
                    upperMenu.state ="State2"
                    break;

                case "State3":
                    upperMenu.state ="State1"
                    break;
            }
        }

        onReleased: {
            switch (upperMenu.state){
                case "State2":
                    upperMenu.state="State1"
                    break;
            }
        }

        onPositionChanged: {
            if (upperMenu.state == "State2" && upperMenu.height < mouseY)
            {
                upperMenu.state = "State3"
            }
        }
    }

    state: "State1"
    states: [
        State {
            //Обычное состояние меню
            name: "State1"
        },
        State {
            //Выбрано меню
            name: "State2"
            PropertyChanges {
                target: upperMenu
                height: mainView.dp(90)
                width: formOrder.width-mainView.dp(10)
            }
        },
        State {
            //Развернуто меню с кнопками
            name: "State3"
            PropertyChanges {
                target: upperMenu
                x: 0
                y: 0

                radius: 0

                height: mainView.height - upperMenu.y*2
                width: mainView.width - upperMenu.x*2

                color: Qt.rgba(0.0, 0.0, 0.0, 0.8)
            }
            PropertyChanges {
                target: boxButtonUpperMenu
                visible: true
                x: mainView.width/2 - boxButtonUpperMenu.width/2
                y: mainView.height/2 - boxButtonUpperMenu.height/2
            }
            PropertyChanges {
                target: layoutForUpperMenuText
                x:boxButtonUpperMenu.x + boxButtonUpperMenu.width/2 - upperMenuText.contentWidth/2
                y:boxButtonUpperMenu.y - boxButtonUpperMenu.height/3
            }
        }
    ]

    transitions: [
        Transition {
            from: "State1"
            to: "State2"
            PropertyAnimation {
                target: upperMenu
                properties: "width,height"
                easing.type: Easing.Linear
                duration: 10
            }
        },
        Transition {
            from: "State2"
            to: "State1"
            PropertyAnimation {
                target: upperMenu
                properties: "width,height"
                easing.type: Easing.Linear
                duration: 10
            }
        },
        Transition {
            from: "State2"
            to: "State3"
            PropertyAnimation {
                target: upperMenu
                properties: "width,height"
                easing.type: Easing.InQuad
                duration: 500
            }
            PropertyAnimation {
                target: boxButtonUpperMenu
                properties: "x,y"
                easing.type: Easing.InQuad
                duration: 500
            }
            PropertyAnimation {
                target: layoutForUpperMenuText
                properties: "x,y"
                easing.type: Easing.InQuad
                duration: 500
            }
        },
        Transition {
            from: "State3"
            to: "State1"
            PropertyAnimation {
                target: upperMenu
                properties: "width,height"
                easing.type: Easing.InQuad
                duration: 500
            }
            PropertyAnimation {
                target: layoutForUpperMenuText
                properties: "x,y"
                easing.type: Easing.InQuad
                duration: 500
            }
        }
    ]

    MessageDialog {
        id: closeCurrentShiftMessage
        title: qsTr("Закрыти смены")
        text:  qsTr("Вы уверены в этом?")
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            interlayerForQML.fillingReceiptList_Slot()
            loaderReportView.state = "State2"

            interlayerForQML.getLastLoginFromQSettings_Slot()
            loaderSelectionForOrder.setSource("LoginView.qml")
            loaderSelectionForOrder.state = "State2"

            interlayerForQML.disconnectFromHost_Slot()

            upperMenu.state = "State1"
            orderListModel.clear()
            listMenu.clear()
            listTabs.clear()
        }
    }
}


