import QtQuick 2.12
import QtQuick.Dialogs 1.3
import "qrc:/QMLFile/Styles"

Rectangle {

    FontLoader {
        id: appFont
        source: "qrc:/fonst/Nunito,Roboto/Nunito/static/Nunito-Regular.ttf"
    }
    id: mainFormLogin
    anchors.fill: parent

    Connections {
           target: interlayerForQML

           onGoodAuthorized_Signal: {
               interlayerForQML.saveLastLoginInQSettings_Slot()
               formConectingToDB.state = "Hidden"
               loaderSelectionForOrder.state = "State1"
               loaderSelectionForOrder.setSource("ItemSelectionForOrderView.qml")

           }

           onBadAuthorized_Signal: {
               formConectingToDB.state = "Hidden"
               errorMessage.setText(qmlProperty.errorString)
               errorMessage.open()
           }

           onSendMessageToSplashScreen_Signal: {
                statusConnectString.text = message
           }
    }

    // Image {
    //     anchors.fill: parent
    //     source: "qrc:/Image/backgroundForLogin.jpg"
    //     fillMode: Image.PreserveAspectCrop
    //     smooth: true
    // }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: formLoginUser
        anchors.centerIn: parent

        width: grdLogin.width + mainView.dp(35)
        height: grdLogin.height + lblLoginHead.height + mainView.dp(35) + formLoginBtn.height

        color: "white"
        opacity: 0.96

        radius: mainView.dp(7)
        border.color: Qt.darker(color)
        border.width: mainView.dp(2)

        Image {
            id: imgLogin            
            x: parent.width/2

            width: parent.width
            height: parent.height

            smooth: true
            source: "qrc:/Image/coffeeBASE.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: lblLoginHead
            anchors{
                top: formLoginUser.top
                horizontalCenter: formLoginUser.horizontalCenter
            }

            text: qsTr("Авторизация")

            font.family: appFont.name
            font.pixelSize: mainView.dp(48)
            style: Text.Outline
            color: "black"

        }

        Grid {
            id: grdLogin
            anchors.centerIn: parent

            rows: 2
            columns: 2

            rowSpacing: mainView.dp(15)
            columnSpacing: mainView.dp(5)

            Text {
                id: userText
                text: qsTr("Пользователь")
                font.family: appFont.name
                color: "black"
                font.pixelSize: mainView.dp(30)
            }

            Rectangle {
                width: parent.width/2
                height: userText.height

                color: Qt.rgba(255, 255, 255, 0.95)

                radius: mainView.dp(5)
                border.width: mainView.dp(2)

                TextInput {
                    id: nameUser
                    anchors.fill: parent

                    verticalAlignment:TextInput.AlignVCenter
                    horizontalAlignment: TextInput.AlignHCenter
                    wrapMode: TextEdit.NoWrap
                    text: qmlProperty.lastLogin

                    font.pixelSize: mainView.dp(28)
                    color: "red"

                    property bool enterPress

                    onAccepted: {
                        enterPress = true
                    }

                    onTextEdited: {
                        if ( nameUser.text.length !== 0 )
                            if(enterPress)
                            {
                                enterPress = false
                            }
                            else
                            {
                                qmlProperty.lastLogin = nameUser.text
                            }
                    }
                }

                Component.onCompleted: {
                    interlayerForQML.getLastLoginFromQSettings_Slot
                    if (qmlProperty.lastLogin !== null)
                        nameUser.text = qmlProperty.lastLogin
                }
            }


            Text {
                text: qsTr("Пароль")
                color: "black"
                font.family: appFont.name
                font.pixelSize: mainView.dp(30)
            }

            Rectangle {
                width: parent.width/2
                height: userText.height

                color: Qt.rgba(255, 255, 255, 0.95)

                radius: mainView.dp(5)
                border.width: mainView.dp(2)

                TextInput {
                    id: password
                    anchors.fill: parent

                    verticalAlignment:TextInput.AlignVCenter
                    horizontalAlignment: TextInput.AlignHCenter

                    text: qsTr("1756")
                    color: "#151515"
                    font.pixelSize: mainView.dp(26)

                    echoMode: TextInput.Password

                    property bool enterPress

                    onAccepted: {
                        enterPress = true
                    }

                    onTextEdited: {
                        if ( nameUser.text.length !== 0 )
                            if(enterPress)
                            {
                                enterPress = false
                            }
                            else
                            {
                                qmlProperty.password = password.text
                            }
                    }

                    Component.onCompleted: {
                        qmlProperty.password = password.text
                    }

                }
            }
        }

        Row {
            id: formLoginBtn
            anchors{
                top: grdLogin.bottom
                horizontalCenter: grdLogin.horizontalCenter
                topMargin: mainView.dp(10)
            }
            spacing: mainView.dp(15)

            Rectangle {
                width: formLoginUser.width/3
                height: width/3

                border.width: mainView.dp(1)
                border.color: "#B6B6B6"
                radius: mainView.dp(7)
                // Сглаживание включено
                smooth: true
                scale: formLoginExitArea.pressed ? 0.8 : 1.0

                Text {
                    anchors.fill: parent

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    text: qsTr("Закрыть")
                    font.family: appFont.name
                    color: "white"
                    font.bold: true

                    fontSizeMode: Text.HorizontalFit
                    minimumPixelSize: mainView.dp(8)
                    font.pixelSize: mainView.dp(28)
                }

                MouseArea {
                    id: formLoginExitArea
                    anchors.fill: parent

                    onClicked:{
                        if (formConectingToDB.state != "Visible")
                        {
                          exitMessage.open()
                        }
                    }
                }

                gradient: GradientForButton{}
            }

            Image {
                height: formLoginBtn.height
                width: height

                smooth: true
                source: "qrc:/Image/Options.png"
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    id: optionArea
                    anchors.fill: parent

                    onClicked: {
                        if (formConectingToDB.state != "Visible")
                        {
                            loaderSelectionForOrder.setSource("OptionView.qml")
                        }
                    }
                }
            }


            Rectangle {
                width: formLoginUser.width/3
                height: width/3

                border.width: mainView.dp(1)
                border.color: "#B6B6B6"
                radius: mainView.dp(7)

                smooth: true
                scale: formLoginOkArea.pressed ? 0.8 : 1.0

                Text {
                    anchors.fill: parent

                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    text: qsTr("Войти")
                    font.family: appFont.name
                    color: "white"
                    font.bold: true
                    fontSizeMode: Text.HorizontalFit
                    minimumPixelSize: mainView.dp(8)
                    font.pixelSize: mainView.dp(28)
                }

                MouseArea {
                    id: formLoginOkArea
                    anchors.fill: parent

                    onClicked: {
                        if (formConectingToDB.state != "Visible")
                        {
                            formConectingToDB.state = "Visible"
                            interlayerForQML.connectToHost_Slot()
                        }
                    }
                }

                gradient: GradientForButton{}
            }
        }
    }

    Rectangle {
        id: formConectingToDB
        anchors.fill: parent
        z: 2
        Text {
            id: connectString
            anchors.centerIn: parent

            text: "Соединение..."
            font.pointSize: 40
            color: "white"
        }

        Text {
            id: statusConnectString
            anchors.top: connectString.bottom
            anchors.topMargin: mainView.dp(25)
            anchors.horizontalCenter: connectString.horizontalCenter
            font.pointSize: 20
            color: "white"
        }

        color: Qt.rgba(0,0,0,0.8)

        state: "Hidden"
        states: [
            State {
                //Обычное состояние меню
                name: "Hidden"
                PropertyChanges {
                    target: formConectingToDB
                    visible: false
                }
            },
            State {
                //Выбрано меню
                name: "Visible"
                PropertyChanges {
                    target: formConectingToDB
                    visible: true
                }
            }
        ]
    }

    MessageDialog {
        id: errorMessage
        title: qsTr("Ошибка")
        standardButtons: StandardButton.Ok
    }

    MessageDialog {
        id: exitMessage
        title: qsTr("Выход")
        text:  qsTr("Точно хотите выйти из приложения?")
        standardButtons: StandardButton.Yes | StandardButton.No
        icon: StandardIcon.Question
        onYes: {
            interlayerForQML.exitApp()
          //  Qt.callLater(Qt.quit)
        }
    }    
}

