import QtQuick 2.12

Text {
    FontLoader {
        id: appFont
        source: "qrc:/fonst/Roboto/static/Roboto_Condensed-Light.ttf"
    }
    property int size: 15

    anchors{
        fill: parent
        margins: mainView.dp(5)
    }

    horizontalAlignment: Text.AlignHCenter

    font.bold: false

    font.family: appFont.name
    fontSizeMode: Text.HorizontalFit
    minimumPointSize: 8
    font.pointSize: size
}
