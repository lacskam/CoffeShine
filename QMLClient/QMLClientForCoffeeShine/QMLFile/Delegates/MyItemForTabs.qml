import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {

    Rectangle {
        FontLoader {
            id: appFont
            source: "qrc:/fonst/Roboto/static/Roboto-Regular.ttf"
        }
        id: layoutForOneTabs
        width: viewForTabs.width / 3 < mainView.dp(160) ? viewForTabs.width / 2 : viewForTabs.width / 3
        height: mainView.dp(80)

        color: "#1E90FF";
        border.width: mainView.dp(3)
        border.color: "white"
        radius: mainView.dp(8)




        Text {


            id: nameCategory
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: mainView.dp(5)
            }

            text: name
            scale: oneTabArea.pressed ? 0.8 : 1
            width: parent.width - colorCategoryDrink.width - mainView.dp(10)
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "white"
            font.family: appFont.name


            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 8
            font.pointSize: 20
        }

        Rectangle {
            id: colorCategoryDrink
            anchors {
                verticalCenter: nameCategory.verticalCenter
                left: parent.left
                leftMargin: (nameCategory.width - nameCategory.contentWidth) / 2 + mainView.dp(5)
            }
            height: parent.height / 4
            width: height
            radius: height / 2
            color: colorCategory
            border.width: mainView.dp(1)
            border.color: Qt.lighter(color)
        }

        MouseArea {
            id: oneTabArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {

                console.log("MouseArea clicked!");
                mainView.setCurrentTab(index);  // Используйте ваш index для переключения вкладок
            }
        }



        state: "State1"
        states: [
            State {
                name: "State1"
            },
            State {
                name: "State2"
                PropertyChanges {
                    target: layoutForOneTabs
                    border.color: "orange"
                    color: "orange"


                }
            }
        ]
    }


}
