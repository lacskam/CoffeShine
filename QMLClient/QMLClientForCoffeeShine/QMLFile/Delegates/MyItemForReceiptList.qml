import QtQuick 2.12

Component {
    Rectangle {
        width: receiptGridView.cellWidth
        height: receiptGridView.cellHeight
        color: "transparent"

        Rectangle {
            anchors{
                fill: parent
                margins: mainView.dp(3)
            }

            radius: mainView.dp(10)
            border.color: "black"
            border.width: mainView.dp(1)

            Text {
                id: title
                anchors{
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                    topMargin: mainView.dp(5)
                }

                text: "№ "+countReceipt
                font.pointSize: 15
            }

            Text {
                id: countDrinks
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(3)
                    top: parent.top
                    topMargin: parent.height/3 - mainView.dp(5)
                }
                width: parent.width - mainView.dp(5)

                text: countDrinksString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

            Text {
                id: totalSumReceipt
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(3)
                    top: countDrinks.bottom
                    topMargin: mainView.dp(5)
                }
                width: parent.width - mainView.dp(5)

                text: sumReceiptString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

            Rectangle {
                id: separate
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(3)
                    top: totalSumReceipt.bottom
                    topMargin: mainView.dp(5)
                }
                width: parent.width*4/5
                height: mainView.dp(1)

                color: "black"
            }

            Text {
                id: dateForReceipt
                anchors{
                    right: parent.right
                    rightMargin: mainView.dp(5)
                    bottom: parent.bottom
                    bottomMargin: mainView.dp(7)
                }
                width: parent.width*4/5

                text: dateReceiptString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 8
                font.pointSize: 13
            }
        }

        MouseArea{
            anchors.fill: parent

            onClicked: {
                reportView.openSoldPosition(index+1, idReceipt)
            }
        }
    }
}
