import QtQuick 2.12
import "qrc:/QMLFile/Delegates"
import "qrc:/QMLFile/Styles"

Rectangle {
    id: reportView
    anchors.fill: parent    

    Connections {
        target: interlayerForQML

        onCreateReceipt_Signal: {
             listReceiptModel.append({countReceipt:      qmlProperty.countReceipt,
                                      countDrinksString: qmlProperty.countDrinksString,
                                      sumReceiptString:  qmlProperty.sumReceiptString,
                                      dateReceiptString: qmlProperty.dateReceiptString,
                                      idReceipt:         qmlProperty.idReceipt})
        }

        onFillingSomePropertyInReportView_Signal:{
            currentUser.text = "открыта "+qmlProperty.openData
            nalSum.text =    qmlProperty.nalString
            bezNalSum.text = qmlProperty.bezNalString
            freeSum.text =   qmlProperty.freeString
        }

        onCreateSoldPosition_Signal:{
                listSoldPositionModel.append({nameDrinkString: qmlProperty.nameDrinkString,
                                              priceString:     qmlProperty.priceString,
                                              volumeString:    qmlProperty.volumeString,
                                              categoryString:  qmlProperty.categoryString,
                                              idDrink:         qmlProperty.idDrink,
                                              colorCode:       qmlProperty.colorCode})
        }
    }


    Image {
        anchors.fill: parent
        source: "qrc:/Image/backgroundForReport.jpg"
        fillMode: Image.PreserveAspectCrop
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        anchors{
            fill: parent
            margins: mainView.dp(10)
        }

        color: "transparent"

        Text {
            id: reportTitle
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
            }
            z: 1
            height: mainView.dp(30)

            horizontalAlignment: Text.AlignHCenter

            text:qsTr("Отчет по текущей смене")
            color: "white"
            style: Text.Outline
            font.bold: true
            styleColor: Qt.lighter("black")
            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 10
            font.pointSize: 26
        }

        Text {
            id: currentUser
            anchors{
                top: reportTitle.bottom
                topMargin: mainView.dp(10)
                left: parent.left
                right: parent.right
            }
            z: 1
            height: mainView.dp(20)

            horizontalAlignment: Text.AlignHCenter

            color: "white"
            style: Text.Outline
            font.bold: true
            styleColor: Qt.lighter("black")
            fontSizeMode: Text.HorizontalFit
            minimumPointSize: 10
            font.pointSize: 18
        }

        Rectangle {
            id:layForTitleReport
            anchors{
                top: parent.top
                horizontalCenter: reportTitle.horizontalCenter
            }
            width: (reportTitle.contentWidth > currentUser.contentWidth ? reportTitle.contentWidth : currentUser.contentWidth) + mainView.dp(15)
            height: reportTitle.contentHeight + currentUser.contentHeight

            radius: mainView.dp(10)           
            color: Qt.rgba(0,0,0,0.7)
        }

        Rectangle {
            id: layoutForReceiptListView
            anchors{
                left: parent.left
                right: parent.horizontalCenter
                top: layForTitleReport.bottom
                bottom: layoutForTotalSum.top
                margins: mainView.dp(7)
            }

            radius: mainView.dp(10)
            color: Qt.rgba(255,255,255,0.8)

            Text {
                id: listReceiptTitle
                anchors{
                    top: parent.top
                    topMargin: mainView.dp(5)
                    horizontalCenter: parent.horizontalCenter
                }
                text: qsTr("Чеки")
                font.pointSize: 20
            }

              GridView {
                  id: receiptGridView
                  anchors{
                      fill: parent
                      margins: mainView.dp(5)
                      topMargin: listReceiptTitle.contentHeight + mainView.dp(5)
                  }

                  snapMode: GridView.SnapOneRow

                  cellWidth: getCellSizeForGridView()
                  cellHeight: cellWidth

                  clip: true

                  delegate: MyItemForReceiptList{}

                  model: ListModel { id: listReceiptModel }
              }
          }

        Rectangle {
            id: layoutForSoldPositonListView
            anchors{
                left: parent.horizontalCenter
                right: parent.right
                top: layForTitleReport.bottom
                bottom: layoutForTotalSum.top
                margins: mainView.dp(7)
            }

            radius: mainView.dp(10)
            color: Qt.rgba(255,255,255,0.8)

            Text {
                id: soldPositionTitle
                anchors{
                    top: parent.top
                    topMargin: mainView.dp(5)
                    horizontalCenter: parent.horizontalCenter
                }

                horizontalAlignment: Text.AlignHCenter

                text: qsTr("Проданные товары")
                width: parent.width
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 8
                font.pointSize: 20
            }

            GridView {
                id: soldPositionGridView
                anchors{
                    fill: parent
                    margins: mainView.dp(5)
                    topMargin: listReceiptTitle.contentHeight + mainView.dp(5)
                }

                snapMode: GridView.SnapOneRow

                cellWidth: getCellSizeForSoldPosition()
                cellHeight: cellWidth*0.9

                clip: true

                delegate: MyItemForSoldPositionList{}

                model: ListModel { id: listSoldPositionModel }
            }
        }

        Rectangle {
            id: layoutForTotalSum
            anchors{
                left: parent.left
                bottom: parent.bottom
                leftMargin: mainView.dp(7)
            }

            width: layoutForReceiptListView.width
            height: mainView.dp(75)
            color: Qt.rgba(0,0,0,0.8)
            radius: mainView.dp(10)

            Text {
                id: nalSum
                anchors{
                    top: parent.top
                    left: parent.left
                    leftMargin: mainView.dp(10)
                    topMargin: mainView.dp(1)
                }
                width: parent.width

                color: "white"
                style: Text.Outline
                font.bold: true
                styleColor: Qt.lighter("black")
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

            Text {
                id: bezNalSum
                anchors{
                    top: nalSum.bottom
                    left: parent.left
                    leftMargin: mainView.dp(10)
                    topMargin: mainView.dp(1)
                }
                width: parent.width

                color: "white"
                style: Text.Outline
                font.bold: true
                styleColor: Qt.lighter("black")
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

            Text {
                id: freeSum
                anchors{
                    top: bezNalSum.bottom
                    left: parent.left
                    leftMargin: mainView.dp(10)
                    topMargin: mainView.dp(1)
                }
                width: parent.width/2

                color: "white"
                style: Text.Outline
                font.bold: true
                styleColor: Qt.lighter("black")
                fontSizeMode: Text.HorizontalFit
                minimumPointSize: 10
                font.pointSize: 15
            }

        }

        Rectangle {
            id: layoutForOkButton
            anchors{
                right: parent.right
                bottom: parent.bottom
                rightMargin: mainView.dp(7)
            }
            width: layoutForSoldPositonListView.width/2
            height: layoutForTotalSum.height

            border.width: mainView.dp(1)
            border.color: "#B6B6B6"
            radius: mainView.dp(10)

            Text {
                anchors.centerIn: parent
                text: qsTr("Ок")
                color: "white"
                style: Text.Outline
                font.pointSize: 24
            }

            MouseArea {
                id: okOnReportViewMouseArea
                anchors.fill: parent

                onClicked:{
                    loaderReportView.state = "State1"
                    listSoldPositionModel.clear()
                    listReceiptModel.clear()
                    soldPositionTitle.text = "Проданные товары"
                }
            }
            scale: okOnReportViewMouseArea.pressed ? 0.9 : 1.0

            gradient: GradientForButton{}
        }
    }

    function openSoldPosition(numberReceipt, idReceipt)
    {
        soldPositionTitle.text = "Проданные товары по чеку № " + numberReceipt

        listSoldPositionModel.clear()
        interlayerForQML.fillingSoldPositionList_Slot(idReceipt)
    }

    property int countCellGridForOneRow: receiptGridView.width/mainView.dp(140)

    function getCellSizeForGridView()
    {
        return countCellGridForOneRow!=0 ? receiptGridView.width/countCellGridForOneRow : receiptGridView.width
    }

    property int countCellGridForSoldPosition: soldPositionGridView.width/mainView.dp(165)

    function getCellSizeForSoldPosition()
    {
        return countCellGridForSoldPosition!=0 ? soldPositionGridView.width/countCellGridForSoldPosition : soldPositionGridView.width
    }
}
