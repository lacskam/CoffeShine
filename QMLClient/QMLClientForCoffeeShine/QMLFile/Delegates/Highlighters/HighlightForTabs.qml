import QtQuick 2.12

Item {
    Rectangle {
        x: viewForTabs.currentItem.x - viewForTabs.currentItem.width/2
        y: viewForTabs.height/2

        height: viewForTabs.height/2
        width: viewForTabs.currentItem.width*2

        color: "#5D4037"
    }

    Rectangle {
        x: viewForTabs.currentItem.x + viewForTabs.currentItem.width

        width: viewForTabs.currentItem.width
        height: viewForTabs.height

        border.width: mainView.dp(4)
        border.color: "white"
        color: "white"
        radius: mainView.dp(20)
    }
}

