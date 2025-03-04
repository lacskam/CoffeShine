import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
    Rectangle {
        width: orderListView.width
        height: mainView.dp(90)
        color: "transparent"

        MouseArea {
            id: mouseItemOrderDelegateChange
            anchors.fill: parent

            onClicked: {
                mainView.prepareToOpenSelectionView(index, idDrink, idCategory, true)
            }
        }

        Rectangle {
            anchors{
                fill: parent
                margins: mainView.dp(2)
            }

            color: "white"
            opacity: 1

            border.width: mainView.dp(0.5)
            border.color: Qt.lighter("gray")
            radius: mainView.dp(8)

            Rectangle {
                id: colorCategoryLay
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(5)
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height/4
                width: height

                radius: height/2

                color: colorCategory
                border.width: mainView.dp(1)
                border.color: Qt.lighter(color)
            }

            Image {
               id:image
               anchors{
                   verticalCenter: parent.verticalCenter
                   right:parent.right
                   rightMargin: mainView.dp(8)
               }

               width: parent.height/1.8
               height: parent.height/1.8

               source: "qrc:/Image/Del_Icon.ico"
               fillMode: Image.PreserveAspectFit
               smooth: true

               visible: true
               scale: mouseItemOrderDelegateDelete.pressed ? 0.8 : 1

               MouseArea {
                   id: mouseItemOrderDelegateDelete
                   anchors.fill: parent

                   onClicked: {
                       orderListModel.remove(index)
                       mainView.totalSumOrder()
                   }
               }
            }

            Rectangle {
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: colorCategoryLay.right
                    right: image.left
                    leftMargin: mainView.dp(5)
                }

                color: "transparent"

                TextForButton {
                    color: "black"
                    anchors.top: parent.top
                    anchors.topMargin: mainView.dp(2)
                    height: mainView.dp(25)
                    id: nameCoffe
                    text: name
                }

                TextForButton {
                      color: "black"
                    anchors.top: nameCoffe.bottom
                    anchors.topMargin: mainView.dp(6)
                    height: mainView.dp(25)

                    id: textVolume

                    text: stringSize
                    size: 15
                }

                TextForButton {
                      color: "black"
                    anchors.top: textVolume.bottom
                    id: textPrice

                    text: stringCost
                    size: 15
                }
            }
        }
    }
}
