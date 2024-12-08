import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
    Rectangle {
        width: volumeListView.width
        height: volumeListModel.count > 2 ? volumeListView.height/3 : volumeListView.height/volumeListModel.count
        color: "transparent"

        Rectangle {
            anchors{
                fill: parent
                margins: mainView.dp(5)
            }
            color: "transparent"
            border.width: mainView.dp(2)
            border.color: "#B6B6B6"
            radius: mainView.dp(10)

            TextForButton {
                anchors.fill: parent

                font.bold: false
                fontSizeMode: Text.Fit
                text: nameVolume +"\n" + volume + " " + volumeUnits
                size: 50
            }
        }

        MouseArea {
             id: oneVolumeArea
             anchors.fill: parent

             hoverEnabled: true

             onClicked: {
                  selectionMenu.setCurrentVolume(index)
             }
        }
    }
}
