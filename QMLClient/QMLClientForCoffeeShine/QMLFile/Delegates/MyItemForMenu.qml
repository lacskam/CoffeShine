import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Component {
      Rectangle {


          id: oneButtomFromMenu
          width: gridViewForItemFromMenu.cellWidth - mainView.dp(7)
          height: gridViewForItemFromMenu.cellHeight - mainView.dp(7)

          color: !buttonCoffieArea.pressed ? "white" : "lightblue"

          border.width: mainView.dp(1.1)
          border.color: Qt.lighter("gray")
          radius: mainView.dp(10)

          opacity: 0.96

          Image {
              anchors.centerIn: parent

              width: gridViewForItemFromMenu.cellWidth - mainView.dp(70)
              height: gridViewForItemFromMenu.cellHeight - mainView.dp(70)
              source: "image://drinks/" + idDrink

              fillMode: Image.PreserveAspectFit




          }
          TextForImage{
              text: name
              verticalAlignment: Text.AlignBottom
              color: "black"

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
