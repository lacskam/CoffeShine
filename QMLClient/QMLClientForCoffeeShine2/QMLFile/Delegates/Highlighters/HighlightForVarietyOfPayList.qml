import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Item {
    Rectangle {
        radius: mainView.dp(10)
        height: varietyOfPayListView.count > 0 ? varietyOfPayListView.currentItem.height - mainView.dp(5)*2 : 0
        width: varietyOfPayListView.count > 0 ? varietyOfPayListView.currentItem.width - mainView.dp(5)*2 : 0

        x: varietyOfPayListView.count > 0 ? varietyOfPayListView .currentItem.x + mainView.dp(5) : 0
        y: varietyOfPayListView.count > 0 ? varietyOfPayListView .currentItem.y + mainView.dp(5) : 0

        Behavior on y { SpringAnimation { spring: 3; damping: 0.3 } }

        gradient: GradientForButton{}
    }
}
