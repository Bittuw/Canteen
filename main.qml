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
//    visibility: "Maximized"

//    width: 650
//    height: 300

    minimumWidth: 700
    minimumHeight: 300

//    maximumWidth: 650
//    maximumHeight: 300

    title: qsTr("Canteen test")

    property int previousX
    property int previousY

    Shortcut {
        sequence: "Esc"
        onActivated: applicationWindow.close()
    }

    footer: StatusBar {
        RowLayout {
            anchors.fill: parent
            spacing: 5

            RowLayout {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

                Label {
                    text: "Последняя дата обновления:"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.pointSize: 8
                }

                Label {
                    id: outputDate
                    text: TextProviderDate.text
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    font.pointSize: 9
                    font.family: "Verdana"
                    wrapMode: Text.WordWrap
                    objectName: "outputDate"
                    font.bold: true
                }
            }

            ToolSeparator {
                id: toolSeparator
                contentItem: Rectangle {
                    implicitHeight: 15
                    implicitWidth: 1
                    color: "#c3c3c3"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Label {
                    text: "Последняя дата-время загрузки:"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.pointSize: 8
                }
                Label {
                    id: outputDateTime
                    text: TextProviderDateTime.text
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.pointSize: 9
                    font.family: "Verdana"
                    wrapMode: Text.wrapMode
                    objectName: "outputDateTime"
                    font.bold: true
                }
            }

            ToolSeparator {
                id: toolSeparator2
                contentItem: Rectangle {
                    implicitHeight: 15
                    implicitWidth: 1
                    color: "#c3c3c3"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                CheckBox {
                    text: "Full Screen"
                    height: showButton.height
                    width: col.cellWidth
//                    Binding on checked { value: applicationWindow.visibility === Window.Windowed }
                    onClicked: {
                        if(checked)
                            applicationWindow.visibility = Window.FullScreen
                        else
                            applicationWindow.visibility = Window.Windowed
                    }
                }
            }
            Item {
                Layout.fillWidth: true
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
                objectName: "outputName"
            }

            ImageItem {
                id: liveImageItem
                anchors.fill: parent
                image: DIProvider.image
                objectName: "liveImageItem"
                scale: 0.6
            }
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
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
