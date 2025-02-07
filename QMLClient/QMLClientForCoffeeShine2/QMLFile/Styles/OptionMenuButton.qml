import QtQuick 2.12

Rectangle {
    property string textForButton

    width: mainLayForOptions.width/4
    height: width/2 > mainView.dp(70) ? mainView.dp(70) : width/2

    border.width: mainView.dp(1)
    border.color: "#B6B6B6"
    radius: mainView.dp(10)
    // Сглаживание включено
    smooth: true

    TextForButton {
        text: textForButton
        height: parent.height
        size: 15
    }

    gradient: GradientForButton{}
}
