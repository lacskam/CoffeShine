import QtQuick 2.12

Component{
    Rectangle {
        id: mainLayoutForItemHostAndPortList
        color:"transparent"
        width: parent.width
        height: (hostName.contentHeight > portName.contentHeight ? hostName.contentHeight : portName.contentHeight) + mainView.dp(60)

        Rectangle {
            anchors{
                fill: parent
                margins: mainView.dp(5)
            }
            border.width: mainView.dp(2)
            border.color: "#B6B6B6"
            radius: mainView.dp(10)

            Grid {
                id: grdLogin
                anchors.left: parent.left
                anchors.leftMargin: mainView.dp(15)

                rows: 2
                columns: 2

                rowSpacing: mainView.dp(15)
                columnSpacing: mainView.dp(15)

                Text {
                    width: parent*2/3
                    id: userText
                    text: qsTr("Хост:")
                    color: "black"
                    font.pixelSize: mainView.dp(30)
                }
                Text {

                    text: qsTr("Порт:")
                    color: "black"
                    font.pixelSize: mainView.dp(30)
                }

                TextInput {
                    width: mainView.width*2/3
                    id: hostName
                    wrapMode: Text.WrapAnywhere
                    text: host
                    color: "black"
                    font.pixelSize: mainView.dp(30)

                    property bool enterPress

                    onAccepted: {
                        enterPress = true
                    }

                    onTextChanged: {
                        if ( hostName.text.length !== 0 )
                            if(enterPress)
                            {
                                if ((hostName.text.length-1 === host.length) || (hostName.text.length+1 === host.length))
                                {
                                    host = hostName.text
                                    enterPress = false
                                }
                            }
                            else
                            {
                                host = hostName.text
                            }
                    }
                }

                TextInput {
                    width: mainView.width/3 - mainView.dp(70)
                    id: portName
                    wrapMode: Text.WrapAnywhere
                    text: port
                    color: "black"
                    font.pixelSize: mainView.dp(30)

                    property bool enterPress2

                    onAccepted: {
                        enterPress2 = true
                    }

                    onTextEdited: {
                        if ( portName.text.length !== 0 )
                            if(enterPress2)
                            {
                                if ((portName.text.length-1 === port.length) || (portName.text.length+1 === port.length))
                                {                                    
                                    port = portName.text
                                    enterPress2 = false
                                }
                            }
                            else
                            {
                                port = portName.text
                            }
                    }
                }

            }
        }

        Image {
            anchors.top: parent.top
            anchors.topMargin: mainView.dp(10)
            anchors.horizontalCenter: parent.horizontalCenter
            height: mainView.dp(35)
            width: height

            smooth: true
            source: "qrc:/Image/cross-for-del.png"
            fillMode: Image.PreserveAspectFit

            MouseArea {
                id: optionArea
                anchors.fill: parent

                onClicked: {
                    hostAndPortListModel.remove(index)
                }
            }

            scale: optionArea.pressed ? 0.8 : 1
        }
    }
}
