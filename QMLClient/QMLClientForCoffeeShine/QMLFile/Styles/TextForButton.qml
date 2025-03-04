import QtQuick 2.12

Text {
    FontLoader {
        id: appFont
        source: "qrc:/fonst/Roboto/static/Roboto-Regular.ttf"
    }
    property int size: 20

    width: parent.width

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    color: "white"

    font.family: appFont.name
    fontSizeMode: Text.HorizontalFit
    minimumPointSize: 8
    font.pointSize: size
}
