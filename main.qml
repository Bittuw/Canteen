import QtQuick 2.11
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import com.bittumworker.org 1.0

ApplicationWindow {

    id: applicationWindow
    visible: true
    width: 650
    height: 300

    minimumWidth: 650
    minimumHeight: 300

    maximumWidth: 650
    maximumHeight: 300

    title: qsTr("Canteen test")

    property int previousX
    property int previousY

    footer: StatusBar {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 279
        RowLayout {
            anchors.left: parent.left
            Label { text: "Internet activity" }

            Rectangle {
                width: 15
                height: 15
                color: "transparent"
                StatusIndicator {
                    anchors.fill: parent
                    id: ethernetStatus
                    objectName: "ethernetStatus"
                    color: "green"
                }
            }
        }
    }



    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Item {
            id: item1
            width: 200
            height: 200
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter


            Label {
                id: outputName

                text: TextProvider.text
                font.family: "Verdana"
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap

                font.pixelSize: 22
                objectName: outputName
            }

            ImageItem {
                id: liveImageItem
                anchors.horizontalCenter: parent.horizontalCenter
                objectName: "liveImageItem"
                scale: 0.5
            }

//            Image {
//                id: nextImage
//                anchors.horizontalCenter: parent.horizontalCenter
//                scale: 0.5
//                source: "/icons/allowed"
//            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            previousX = mouseX
            previousY = mouseY
        }

        onMouseXChanged: {
            var dx = mouseX - previousX
            applicationWindow.setX(applicationWindow.x + dx)
        }

        onMouseYChanged: {
            var dy = mouseY - previousY
            applicationWindow.setY(applicationWindow.y + dy)
        }
    }

    Connections {
        target: DIProvider
        onImageChanged: liveImageItem.image = DIProvider.image
    }
}

