import QtQuick 2.12
import QtQuick.Dialogs 1.3
import "qrc:/QMLFile/Delegates"
import "qrc:/QMLFile/Delegates/Highlighters"
import "qrc:/QMLFile/Styles"


Rectangle {
    id: optrionView
    anchors.fill: parent

    property bool portHaveError: false
    property int currentIdPoinSale

    Connections {
        target: interlayerForQML

        onCreateHostAndPort_Signal: {
            hostAndPortListModel.append({ host: qmlProperty.host,
                                          port: qmlProperty.port })
        }

        onSomePortNotValid_Signal: {
                portHaveError = true
        }
    }


     color: "white";

    Rectangle {
        id: mainLayForOptions
        anchors{
            fill: parent
            margins: mainView.dp(15)
        }

        color: "transparent"

        Text {
            id: optionTitle
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
            }

            height: optionTitle.contentHeight

            horizontalAlignment: Text.AlignHCenter

            text:qsTr("Настройки программы")
            color: Qt.darker(Qt.darker("white"))

            font.bold: true
            styleColor: Qt.lighter("black")
            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 10
            font.pointSize: 26
        }

        Rectangle {
            id: layoutForOptions
            y: optionTitle.contentHeight+mainView.dp(20)
            width: parent.width
            height: mainView.height - y - layoutForOptionsButton.height - layoutForPointSaleId.height - mainView.dp(40)
            opacity: 0.9
            color: "white"
            radius: mainView.dp(10)

            ListView {
                id: hostAndPortListView
                anchors{
                    fill: parent
                    margins: mainView.dp(5)
                }

                boundsBehavior: Flickable.StopAtBounds
                clip: true

                delegate: MyItemForHostAndPortList { }

                model: ListModel{ id: hostAndPortListModel }
            }

            Component.onCompleted: {
                interlayerForQML.fillingHostAndPortListFromQSettings_Slot()
                interlayerForQML.getIdPointSaleFromQSettings_Slot()
                idPoinSaleName.text = qmlProperty.idPointSaleInOptions
            }
        }

        Row {
            id: layoutForPointSaleId
            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: layoutForOptionsButton.top
            }

            spacing: mainView.dp(5)

            Text {
                id: pointSale

                width: pointSale.contentWidth
                height: mainView.dp(40)

                horizontalAlignment: Text.AlignHCenter

                text:qsTr("Id точки продаж: ")
                color: "black"

                font.bold: true
                styleColor: Qt.lighter("black")
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

            TextInput {
                id: idPoinSaleName
                width: pointSale.width
                wrapMode: Text.WrapAnywhere
                horizontalAlignment: Text.AlignLeft
                color: "black"
                font.bold: true
                font.pointSize: 15

                property bool enterPress

                onAccepted: {
                    enterPress = true
                }

                onTextEdited: {
                    if ( idPoinSaleName.text.length !== 0 )
                        if(enterPress)
                        {
                            if ((idPoinSaleName.text.length-1 === port.length) || (idPoinSaleName.text.length+1 === port.length))
                            {
                                currentIdPoinSale = idPoinSaleName.text
                                enterPress = false
                            }
                        }
                        else
                        {
                            currentIdPoinSale = idPoinSaleName.text
                        }
                }
            }
        }

        Row {
            id: layoutForOptionsButton
            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
            }

            spacing: mainView.dp(50)

            OptionMenuButton {
                textForButton: qsTr("Добавить")

                MouseArea {
                    id: addButtonArea
                    anchors.fill: parent
                    onClicked: {
                        hostAndPortListModel.append({ host: "",
                                                      port: "" })
                    }
                }
                scale: addButtonArea.pressed ? 0.8 : 1.0
            }

            OptionMenuButton {
                textForButton: qsTr("Назад")

                MouseArea {
                    id: resetButtonArea
                    anchors.fill: parent
                    onClicked: {
                        interlayerForQML.getLastLoginFromQSettings_Slot()
                        loaderSelectionForOrder.setSource("LoginView.qml")
                    }
                }
                scale: resetButtonArea.pressed ? 0.8 : 1.0
            }

            OptionMenuButton {
                textForButton: qsTr("Сохранить")

                MouseArea {
                    id: exitButtonArea
                    anchors.fill: parent
                    onClicked: {
                        saveDataMessage.open()
                    }
                }
                scale: exitButtonArea.pressed ? 0.8 : 1.0
            }
        }
    }

    MessageDialog {
        id: errorMessage
        title: qsTr("Ошибка")
        text:  qsTr("В одном из соединений порт введен неправильно")
        standardButtons: StandardButton.Ok
    }

    MessageDialog {
        id: saveDataMessage
        title: qsTr("Сохранение данных")
        text:  qsTr("Вы уверены что хотите сохранить изменения?")
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            saveHostAndPortList()
            if(portHaveError)
            {
                errorMessage.open()
                portHaveError = false
            }
            else
            {
                qmlProperty.idPointSaleInOptions = currentIdPoinSale
                interlayerForQML.saveIdPointSaleInQSettings_Slot()
                interlayerForQML.saveHostAndPortInQSettings_Slot()
            }
        }
    }

    property int i

    function saveHostAndPortList(){
        for(i = 0;i<hostAndPortListModel.count;i++)
        {
            if(!portHaveError)
            {
                qmlProperty.host = hostAndPortListModel.get(i).host
                qmlProperty.port = hostAndPortListModel.get(i).port
                interlayerForQML.saveHostAndPortInInterlayer_Slot()
            }
        }
    }
}
