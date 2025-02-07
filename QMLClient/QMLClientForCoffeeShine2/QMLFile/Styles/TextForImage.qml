import QtQuick 2.12

Text {
    property int size: 15

    anchors{
        fill: parent
        margins: mainView.dp(5)
    }

    horizontalAlignment: Text.AlignHCenter

    font.bold: true    
    style: Text.Outline

    fontSizeMode: Text.HorizontalFit
    minimumPointSize: 8
    font.pointSize: size
}
