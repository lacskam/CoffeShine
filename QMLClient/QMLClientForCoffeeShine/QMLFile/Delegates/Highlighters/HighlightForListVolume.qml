import QtQuick 2.12
import "qrc:/QMLFile/Styles"

Item {
    Rectangle {
        radius: mainView.dp(10)
        height: volumeListView.count > 0 ? volumeListView.currentItem.height - mainView.dp(5)*2 : 0
        width: volumeListView.count > 0 ? volumeListView.currentItem.width - mainView.dp(5)*2 : 0

        x: volumeListView.count > 0 ? volumeListView .currentItem.x + mainView.dp(5) : 0
        y: volumeListView.count > 0 ? volumeListView .currentItem.y + mainView.dp(5) : 0

        Behavior on y { SpringAnimation { spring: 3; damping: 0.3 } }

        gradient: GradientForButton{}
    }
}
