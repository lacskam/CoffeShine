import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
    Rectangle {
        id : layoutForOneTabs
        anchors{
            top: parent.top
            bottom: parent.bottom
        }

        width: viewForTabs.width/3 < mainView.dp(160) ? viewForTabs.width/2 : viewForTabs.width/3
        border.width: mainView.dp(4)
        border.color: "white"
        radius: mainView.dp(20)

        Text {
            id: nameCategory
            anchors{
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
            style: Text.Outline
            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 8
            font.pointSize: 25
        }

        Rectangle {
            id: colorCategoryDrink
            anchors{
                verticalCenter: nameCategory.verticalCenter
                left: parent.left
                leftMargin: (nameCategory.width - nameCategory.contentWidth)/2 + mainView.dp(5)
            }
            height: parent.height/4
            width: height

            radius: height/2

            color: colorCategory
            border.width: mainView.dp(1)
            border.color: Qt.lighter(color)
        }

        MouseArea {
             id: oneTabArea
             anchors.fill: parent

             hoverEnabled: true

             onClicked: {                
                 mainView.setCurrentTab(index)
             }
        }

        gradient: Gradient {
                      GradientStop {
                            position: 0.0
                            color: "black"
                      }
                      GradientStop {
                            position: 0.4
                            color: "#5D4037"
                      }
                  }

        state: "State1"
        states: [
            State {
                  //Обычное состояние меню
                name: "State1"
            },
            State {
                  //Выбрана вкладка
                name: "State2"
                PropertyChanges {
                     target: layoutForOneTabs
                     border.color: "#5D4037"
                }
            }
        ]
    }
}
