import QtQuick 2.12

Text {
    property int size: 20

    width: parent.width

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    color: "white"
    font.bold: true
    style: Text.Outline
    fontSizeMode: Text.HorizontalFit
    minimumPointSize: 8
    font.pointSize: size
}
