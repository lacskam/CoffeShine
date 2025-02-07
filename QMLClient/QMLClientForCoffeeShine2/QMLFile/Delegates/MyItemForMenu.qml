import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
      Rectangle {
          id: oneButtomFromMenu
          width: gridViewForItemFromMenu.cellWidth - mainView.dp(1)
          height: gridViewForItemFromMenu.cellHeight - mainView.dp(1)

          color: !buttonCoffieArea.pressed ? colorCode : "lightblue"

          border.width: mainView.dp(2)
          border.color: Qt.lighter(color)
          radius: mainView.dp(25)

          opacity: 0.96

          Image {
              anchors.centerIn: parent

              width: gridViewForItemFromMenu.cellWidth - mainView.dp(5)
              height: gridViewForItemFromMenu.cellHeight - mainView.dp(5)
              source: "image://drinks/" + idDrink

              fillMode: Image.PreserveAspectFit

              TextForImage{
                  text: name
                  styleColor: "white"
              }

              TextForImage{
                  text: name
                  verticalAlignment: Text.AlignBottom
                  color: "white"
              }
          }

          MouseArea {
              id: buttonCoffieArea
              anchors.fill: parent

              onClicked: {
                  mainView.prepareToOpenSelectionView(index, idDrink, idCategory, false)
              }
          }         
     }
}
