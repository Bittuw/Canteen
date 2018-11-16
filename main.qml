import QtQuick 2.11
import QtQuick.Dialogs 1.2
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
    modality: Qt.WindowModal

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
        id: escaping
        sequence: "Esc"
        onActivated: applicationWindow.close()
    }

    menuBar: MenuBar {
        id: menuBar

        Menu {
            title: qsTr("Edit")
            MenuItem {
                text: "Изменить стоимость обеда"
                onTriggered: changeDialog.open();
            }
        }

        delegate: MenuBarItem {
            id: menuBarItem

            contentItem: Text {
                text: menuBarItem.text
                font: menuBarItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? "#ffffff" : "#21be2b"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                color: menuBarItem.highlighted ? "#21be2b" : "transparent"
            }
        }

        background: Rectangle {
            implicitWidth: 40
            implicitHeight: 40
            color: "#ffffff"

            Rectangle {
                color: "#21be2b"
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
            }
        }
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
                    height: parent.height
                    width: parent.width
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
        id: alt_tab_fake
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            applicationWindow.update()
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

    Dialog {
        id: changeDialog
        title: "Изменить стоимость обеда"
        contentItem: Rectangle {
            TextField {
                id: inputCost
                placeholderText: qsTr("Number field")
                validator: IntValidator {bottom: 1; top: 500;}
                onAccepted: { changeDialog.accept() }
            }
        }

        standardButtons: StandardButton.Save | StandardButton.Cancel

        onAccepted: {
            var cost = inputCost.text;
            ComplexProvider.set_complex_cost(cost)
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
