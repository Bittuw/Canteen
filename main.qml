import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import com.bittumworker.org 1.0

ApplicationWindow {
    visible: true
    width: 650
    height: 300

    minimumWidth: 650
    minimumHeight: 300

    maximumWidth: 650
    maximumHeight: 300

    title: qsTr("Canteen test")

    ImageItem {
        id: liveImageItem
        anchors.fill: parent
        objectName: "liveImageItem"
    }

    Connections {
        target: DIProvider
        onImageChanged: liveImageItem.image = DIProvider.image
    }
}
