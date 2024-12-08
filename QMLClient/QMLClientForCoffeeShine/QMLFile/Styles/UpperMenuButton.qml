import QtQuick 2.12

Rectangle {
    property string buttonText

    width: mainView.dp(140)
    height: mainView.dp(60)

    radius: mainView.dp(10)
    border.width: mainView.dp(3)
    border.color: "#B6B6B6"

    visible: true
    // Сглаживание включено
    smooth: true

    Text {
        text: buttonText
        color: "white"
        anchors.centerIn: parent
        font.pointSize: 12
    }

    gradient: GradientForButton{}
}
