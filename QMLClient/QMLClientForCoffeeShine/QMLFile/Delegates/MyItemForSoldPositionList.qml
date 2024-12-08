import QtQuick 2.12

Component {
    Rectangle {
        width: soldPositionGridView.cellWidth
        height: soldPositionGridView.cellHeight
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
                id: nameDrink
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(3)
                    top: parent.top
                    topMargin: mainView.dp(5)
                }
                width: parent.width - mainView.dp(5)

                text: nameDrinkString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 8
                font.pointSize: 15
            }

            Text {
                id: catrgoryInfo
                anchors{
                    left: parent.left
                    leftMargin: mainView.dp(10)
                    top: nameDrink.bottom
                    bottomMargin: mainView.dp(3)
                }
                width: nameDrink.width

                text: categoryString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 12
            }


            Rectangle {
                id: layForImage
                anchors{
                    top: catrgoryInfo.bottom
                    left: parent.left
                    leftMargin: mainView.dp(3)
                    bottom: parent.bottom
                    bottomMargin: mainView.dp(10)
                }
                width: parent.width/2 - mainView.dp(10)

                radius: mainView.dp(10)
                color: colorCode
                opacity: 0.7

            }

            Image {
                anchors.centerIn: layForImage

                width: layForImage.width
                height: layForImage.height
                source: "image://drinks/" + idDrink

                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: volumeInfo
                anchors{
                    right: parent.right
                    top: layForImage.top
                    topMargin: layForImage.height/6
                }
                width: parent.width/2

                text: volumeString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 12
            }

            Text {
                id: priceInfo
                anchors{
                    right: parent.right
                    top: volumeInfo.bottom
                    topMargin: mainView.dp(4)
                }
                width: parent.width/2

                text: priceString
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 12
            }
        }
    }
}
