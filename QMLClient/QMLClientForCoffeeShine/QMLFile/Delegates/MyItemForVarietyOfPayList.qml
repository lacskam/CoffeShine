import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
    Rectangle {
        width: varietyOfPayListView.width
        height: varietyOfPayListView.count > 2 ? varietyOfPayListView.height/3 : varietyOfPayListView.height/varietyOfPayListView.count
        color: "transparent"

        Rectangle {
            anchors{
                fill: parent
                margins: mainView.dp(5)
            }
            color: "transparent"
            border.width: mainView.dp(2)
            border.color: "#B6B6B6"
            radius: mainView.dp(2)

            TextForButton {
                anchors.fill: parent

                font.bold: false
                fontSizeMode: Text.Fit
                text: nameVarietyOfPay
                size: 50
            }
        }

        MouseArea {
             id: oneVarietyOfPayArea
             anchors.fill: parent

             hoverEnabled: true

             onClicked: {
                  selectionMenu.setCurrentVarietyOfPay(index)
             }
        }
    }
}
